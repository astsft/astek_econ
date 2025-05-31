/**
  * @file    task_net.c
  * @brief   Networking
  * @author  nkp144
  */


/*******************************************************************************
*
*******************************************************************************/
#include <stdbool.h>
#include "cmsis_os.h"
#include "stm32.h"
#include "config.h"
#include "dev\dev.h"
#include "os\os_user.h"
#include "app_pipe.h"
#include "hw2331_eth.h"
#include "modbus.h"
#include "webpages.h"

#define _MAIN_DEBUG_

#define SOCK_TCPS        0


#define DATA_BUF_SIZE   2048
uint8_t gDATABUF[DATA_BUF_SIZE];

#define HTTP_DATA_BUF_SIZE   2048
uint8_t gHTTPDATABUFRX[HTTP_DATA_BUF_SIZE];
uint8_t gHTTPDATABUFTX[HTTP_DATA_BUF_SIZE];
#define MAX_HTTPSOCK	1
#define SOCK_HTTP       1
#define SOCK_CONFIG	0
uint8_t http_socknumlist[] = {SOCK_HTTP};

static  uint8_t         modbus_tcp_adu[ MDBS_TCP_ADU_SIZEOF ];

uint8_t socket_state;

/*******************************************************************************
*
*******************************************************************************/
extern  QueueHandle_t   que_net_hndl;
extern  dev_t           dev;

void send_cmd_to_net_task (os_user_tag_t tag)
{
   BaseType_t rtos_result;
   app_pipe_t      queue_data = 
   {
     .cnt = 0, 
     .data = NULL,
     .param_data = 0,
     .param_id = 0,
     .tag = 0
   };    
   queue_data.tag = tag;
   
   rtos_result = xQueueSend( que_net_hndl, &queue_data, NULL );
   if (rtos_result != pdTRUE) 
   {
     xQueueReset(que_net_hndl);
     xQueueSend( que_net_hndl, &queue_data, NULL );
   }   
}

static int32_t open_and_listen_socket (uint8_t sn, uint8_t* buf, uint16_t port);
static int32_t fsm_tcp_socket_process (uint8_t sn, uint8_t* buf, uint16_t port);
static int32_t start_http_server(void);

/*******************************************************************************
*
*******************************************************************************/
void
task_net(                               const   void *          argument )
{
    bool            received;
    os_user_tag_t   tag;
    app_pipe_t      queue_data = 
    {
      .cnt = 0, 
      .data = NULL,
      .param_data = 0,
      .param_id = 0,
      .tag = 0
    };
        
    osDelay( 2000 );
    eth_init ();       
    if (dev.net->net_status)
    {      
      eth_network_init();
      send_cmd_to_net_task(OS_USER_TAG_ETH_START);
    }
    
    while( true )
    {
        received    =   xQueueReceive( que_net_hndl, &queue_data,  pdMS_TO_TICKS(1000));
        tag = queue_data.tag;
        if (received == pdFALSE)
        {
          httpServer_time_handler();
          tag = OS_USER_TAG_ETH_QUEUE_TIMEOUT;
        }
                
        switch( tag )
        {
          case OS_USER_TAG_ETH_START:
            fsm_tcp_socket_process(SOCK_TCPS, gDATABUF, dev.net->port_modbus);
            //start_http_server();
            //for(int i = 0; i < MAX_HTTPSOCK; i++)	httpServer_run(i);	// HTTP server handler
            break;
          
          case OS_USER_TAG_ETH_INT: 
              if (dev.net->net_status)
              {
                int16_t sock_num;
                sock_num = eth_get_int_sn();
                switch (sock_num)
                {
                case SOCK_TCPS: fsm_tcp_socket_process(SOCK_TCPS, gDATABUF, dev.net->port_modbus);
                  break;
                //case SOCK_HTTP:
                //  for(int i = 0; i < MAX_HTTPSOCK; i++)	httpServer_run(i);	// HTTP server handler
                //  break;
                  
                default: 
                  break;
                }

              }
            break;
            
          case OS_USER_TAG_ETH_QUEUE_TIMEOUT:
            fsm_tcp_socket_process(SOCK_TCPS, gDATABUF, dev.net->port_modbus);
            //for(int i = 0; i < MAX_HTTPSOCK; i++)	httpServer_run(i);	// HTTP server handler
            break;
            
          case OS_USER_TAG_ETH_CHANGE_PARAM:
              // close sockets
              close(SOCK_TCPS);
              //close(SOCK_HTTP);
              if (!dev.net->net_status)
              {
                eth_deinit();
              }
              else
              {
                eth_network_init ();    
                send_cmd_to_net_task(OS_USER_TAG_ETH_INT);               
              }
            break;
          
          default:
            break;
        }
        
        TRACE("Wait queue\r\n");
    }
}

static int32_t fsm_tcp_socket_process (uint8_t sn, uint8_t* buf, uint16_t port)
{
  int32_t ret;
  uint16_t size = 0, sentsize=0;
  uint16_t received_size = 0, mdbs_tcp_packet_len = 0;
  
  switch(getSn_SR(sn))
  {
      case SOCK_CLOSED:
         open_and_listen_socket(sn, buf, port);
         break;
                  
      case SOCK_ESTABLISHED :
         if(getSn_IR(sn) & Sn_IR_CON)
         {
            TRACE("%d:Connected\r\n",sn);
            setSn_IR(sn,Sn_IR_CON);
         }
         //if (getSn_IR(sn) & Sn_IR_RECV) // НЕОБХОДИМО ЕЩЁ ПРОВЕРИТЬ НЕ ПРИШЁЛ ЛИ ПАКЕТ, ПОКА ОТПРАВЛЯЛИ ОТВЕТ!!!
         while (getSn_IR(sn) & Sn_IR_RECV) // Пакет мог прийти, когда мы отправляли пакет. Проверять!         
         {
            TRACE("%d:Received data\r\n", sn);
            setSn_IR(sn, Sn_IR_RECV);
            if((size = getSn_RX_RSR(sn)) > 0)      //receive process
            {
                TRACE("Data size=%d\r\n", size);
                if(size > DATA_BUF_SIZE) size = DATA_BUF_SIZE;
                received_size = recv(sn, buf, size);
                if(received_size <= 0) return received_size;
                
                while (received_size)  // В считанных данных может быть несколько запросов. Обработать все данные.
                {
                  mdbs_tcp_packet_len = calc_mdbs_tcp_packet_len(buf);
                  if ((mdbs_tcp_packet_len > received_size) || (mdbs_tcp_packet_len > MDBS_TCP_ADU_SIZEOF)) return -1;
                  memcpy(modbus_tcp_adu, buf, mdbs_tcp_packet_len);
                  sentsize = mdbs_tcp_slave(dev.cfg.ext_mdbs_cfg.tcp_addr, modbus_tcp_adu);
                  TRACE("Send size=%d\r\n", sentsize);
                  if (sentsize != 0)
                  {
                    ret = SOCK_BUSY;
                    while (ret == SOCK_BUSY)
                    {
                      ret = send(sn, modbus_tcp_adu, sentsize);   // Отправить ответ
                      if(ret < 0)
                      {
                          close(sn);
                          return ret;
                      }
                    }
                  }
                  
                  received_size -= mdbs_tcp_packet_len;
                  buf += mdbs_tcp_packet_len;
                  TRACE("Remaining data=%d\r\n", received_size);
                }                         
            }
            //break;            
         }
         
         if (getSn_IR(sn) & Sn_IR_SENDOK)
         {
           TRACE("Send OK\r\n");
           clear_socket_sending_status(sn);  
           TRACE("Clear status OK\r\n");
           setSn_IR(sn, Sn_IR_SENDOK);
         }
         break;         
         
      case SOCK_CLOSE_WAIT :
         TRACE("%d:CloseWait\r\n",sn);
         if((ret=disconnect(sn)) != SOCK_OK) return ret;
         TRACE("%d:Closed\r\n",sn);
         if (getSn_IR(sn) & Sn_IR_DISCON)
         {
           setSn_IR(sn, Sn_IR_DISCON);
         }
         while(getSn_SR(sn)!=SOCK_CLOSED)
         {
           TRACE("%d:Wait for closed status\r\n",sn);
         }
         send_cmd_to_net_task(OS_USER_TAG_ETH_INT); // Start socket again
         break; 
         
      default:
         break;         
  }
  return 1;
}

static int32_t open_and_listen_socket (uint8_t sn, uint8_t* buf, uint16_t port)
{
  int32_t ret;  
  TRACE("%d:Open socket\r\n", sn);  
  if((ret = socket(sn, Sn_MR_TCP, port, 0x00)) != sn)
      return ret;
  TRACE("%d:Opened\r\n",sn);
  
  while (getSn_SR(sn) != SOCK_INIT)
  {
  }
  
  TRACE("%d:Listen, port [%d]\r\n",sn, port);
  if( (ret = listen(sn)) != SOCK_OK) return ret;  
  
  return 1;
}

static void http_mcu_reset (void)
{
}

static int32_t start_http_server(void)
{
  /* HTTP Server Initialization  */
  httpServer_init(gHTTPDATABUFTX, gHTTPDATABUFRX, MAX_HTTPSOCK, http_socknumlist);   // Tx/Rx buffers (2kB) / The number of W5500 chip H/W sockets in use
  
  reg_httpServer_cbfunc(http_mcu_reset, NULL); // Callback: STM32 MCU Reset
  
  {
 	/* Web content registration (web content in webpage.h, Example web pages) */
 
 	// Index page and netinfo / base64 image demo
 	//reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)index_page, strlen(index_page));			// index.html 		: Main page example
 	reg_httpServer_webContent((uint8_t *)"index.html", (uint8_t *)data__index_html, sizeof(data__index_html));			// index.html 		: Main page example        
 	reg_httpServer_webContent((uint8_t *)"style.css", (uint8_t *)data__style_css, strlen(data__style_css));			// index.html 		: Main page example            
 	reg_httpServer_webContent((uint8_t *)"img/favicon.ico", (uint8_t *)data__img_favicon_ico, sizeof(data__img_favicon_ico));			// index.html 		: Main page example                            
 	//reg_httpServer_webContent((uint8_t *)"IMG/bg02.jpg", (uint8_t *)data__IMG_bg02_jpg, sizeof(data__IMG_bg02_jpg));			// index.html 		: Main page example                    
 	//reg_httpServer_webContent((uint8_t *)"netinfo.html", (uint8_t *)netinfo_page, strlen(netinfo_page));			// netinfo.html 	: Network information example page
 	//reg_httpServer_webContent((uint8_t *)"netinfo.js", (uint8_t *)wiz550web_netinfo_js, strlen(wiz550web_netinfo_js));	        // netinfo.js 		: JavaScript for Read Network configuration 	(+ ajax.js)
// 	reg_httpServer_webContent((uint8_t *)"img.html", (uint8_t *)img_page);				// img.html 		: Base64 Image data example page
        //
 	//// Example #1
 	reg_httpServer_webContent((uint8_t *)"measure.html", (uint8_t *)data__measure_html, sizeof(data__measure_html));			// index.html 		: Main page example                
 	reg_httpServer_webContent((uint8_t *)"js/measure.js", (uint8_t *)data__js_measure_js, sizeof(data__js_measure_js));			// index.html 		: Main page example                
        
 	//reg_httpServer_webContent((uint8_t *)"dio.html", (uint8_t *)dio_page);				// dio.html 		: Digital I/O control example page
 	//reg_httpServer_webContent((uint8_t *)"dio.js", (uint8_t *)wiz550web_dio_js);			// dio.js 			: JavaScript for digital I/O control 	(+ ajax.js)
        //
 	//// Example #2
        reg_httpServer_webContent((uint8_t *)"netinfo.html", (uint8_t *)data__netinfo_html, sizeof(data__netinfo_html));			// index.html 		: Main page example                
 	reg_httpServer_webContent((uint8_t *)"js/netinfo.js", (uint8_t *)data__js_netinfo_js, sizeof(data__js_netinfo_js));			// index.html 		: Main page example                        
 	//reg_httpServer_webContent((uint8_t *)"ain.html", (uint8_t *)ain_page);				// ain.html 		: Analog input monitor example page
 	//reg_httpServer_webContent((uint8_t *)"ain.js", (uint8_t *)wiz550web_ain_js);			// ain.js 			: JavaScript for Analog input monitor	(+ ajax.js)
        //
 	//// Example #3
 	//reg_httpServer_webContent((uint8_t *)"ain_gauge.html", (uint8_t *)ain_gauge_page);		// ain_gauge.html 	: Analog input monitor example page; using Google Gauge chart
 	//reg_httpServer_webContent((uint8_t *)"ain_gauge.js", (uint8_t *)ain_gauge_js);			// ain_gauge.js 	: JavaScript for Google Gauge chart		(+ ajax.js)
        //
 	// AJAX JavaScript functions
 	//reg_httpServer_webContent((uint8_t *)"ajax.js", (uint8_t *)wiz550web_ajax_js, strlen(wiz550web_ajax_js));			// ajax.js			: JavaScript for AJAX request transfer
 	reg_httpServer_webContent((uint8_t *)"js/ajax.js", (uint8_t *)data__js_ajax_js, sizeof(data__js_ajax_js));			// ajax.js			: JavaScript for AJAX request transfer        
 
#ifdef _MAIN_DEBUG_
	display_reg_webContent_list();
#endif
  }  
  
  return 0;
}
