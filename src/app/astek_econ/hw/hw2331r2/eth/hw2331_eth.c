#include "stm32.h"
#include "os\os_user.h"
#include "app_pipe.h"
#include "cmsis_os.h"
#include "hw2331_eth.h"
#include "dev\dev.h"

extern  QueueHandle_t   que_net_hndl;
extern  dev_t dev;

///////////////////////////////////
// Default Network Configuration //
///////////////////////////////////
wiz_NetInfo gWIZNETINFO = { .mac = {0x00, 0x08, 0xdc, 0x00, 0xab, 0xcd},
                            .ip = {192, 168, 1, 100},
                            .sn = {255,255,255,0},
                            .gw = {192, 168, 1, 1},
                            .dns = {0,0,0,0},
                            .dhcp = NETINFO_STATIC };

static void network_init(void);

void eth_init (void)
{
  uint8_t memsize[2][8] = {{2,2,2,2,2,2,2,2},{2,2,2,2,2,2,2,2}}; // 16KB for receive, 16KB for transmit. 8 socets, 2KB for every socet. 
  uint8_t tmp;
  
  wizchip_init_io();
  
  reg_wizchip_cris_cbfunc(wizchip_crit_section_enter, wizchip_crit_section_exit); 
  
  reg_wizchip_cs_cbfunc(wizchip_select, wizchip_deselect);
  
  reg_wizchip_spi_cbfunc(wizchip_spi_read_byte, wizchip_spi_write_byte);
  
  reg_wizchip_spiburst_cbfunc(wizchip_spi_read_burst, wizchip_spi_write_burst);
    
  /* WIZCHIP SOCKET Buffer initialize */
  if(ctlwizchip(CW_INIT_WIZCHIP, (void*)memsize) == -1)
  {
     TRACE("WIZCHIP Initialized fail.\r\n");
     while(1);
  }  
  
  /* PHY link status check */
  do
  {
     if(ctlwizchip(CW_GET_PHYLINK, (void*)&tmp) == -1)
        TRACE("Unknown PHY Link stauts.\r\n");
  } while(tmp == PHY_LINK_OFF);  
 
}

void eth_deinit (void)
{
  uint8_t tmp;
  //w5500 software reset
 ctlwizchip(CW_RESET_WIZCHIP, (void*)&tmp);
}

void eth_network_init(void)
{
   uint8_t tmpstr[6];
   memcpy(gWIZNETINFO.mac, dev.net->mac.u8, sizeof(gWIZNETINFO.mac));
   memcpy(gWIZNETINFO.ip, dev.net->ip.u8, sizeof(gWIZNETINFO.ip));   
   memcpy(gWIZNETINFO.sn, dev.net->sn.u8, sizeof(gWIZNETINFO.sn));      
   memcpy(gWIZNETINFO.gw, dev.net->gw.u8, sizeof(gWIZNETINFO.gw));         
          
   ctlnetwork(CN_SET_NETINFO, (void*)&gWIZNETINFO);
   ctlnetwork(CN_GET_NETINFO, (void*)&gWIZNETINFO);
   
   // Display Network Information
   ctlwizchip(CW_GET_ID,(void*)tmpstr);
   TRACE("\r\n=== %s NET CONF ===\r\n",(char*)tmpstr);
   TRACE("MAC: %02X:%02X:%02X:%02X:%02X:%02X\r\n",gWIZNETINFO.mac[0],gWIZNETINFO.mac[1],gWIZNETINFO.mac[2],
             gWIZNETINFO.mac[3],gWIZNETINFO.mac[4],gWIZNETINFO.mac[5]);
   TRACE("SIP: %d.%d.%d.%d\r\n", gWIZNETINFO.ip[0],gWIZNETINFO.ip[1],gWIZNETINFO.ip[2],gWIZNETINFO.ip[3]);
   TRACE("GAR: %d.%d.%d.%d\r\n", gWIZNETINFO.gw[0],gWIZNETINFO.gw[1],gWIZNETINFO.gw[2],gWIZNETINFO.gw[3]);
   TRACE("SUB: %d.%d.%d.%d\r\n", gWIZNETINFO.sn[0],gWIZNETINFO.sn[1],gWIZNETINFO.sn[2],gWIZNETINFO.sn[3]);
   TRACE("DNS: %d.%d.%d.%d\r\n", gWIZNETINFO.dns[0],gWIZNETINFO.dns[1],gWIZNETINFO.dns[2],gWIZNETINFO.dns[3]);
   TRACE("======================\r\n");  
   
   // Set interrupt for socket 0
   intr_kind int_status  = IK_SOCK_0 | IK_SOCK_1;    
   ctlwizchip(CW_SET_INTRMASK, (void*)&int_status);  
   
   int_status = 0;
   ctlwizchip(CW_GET_INTRMASK, (void*)&int_status);  
    
   TRACE("\r\n=== INT MASK ===\r\n");    
   
   uint32_t sts_reg = 0;
   TRACE( "WOL=%c, PPPOE_TERMINATED=%c, DEST_UNREACH=%c, IP_CONFLICT=%c\r\n",
            int_status & IK_WOL ? '1' : '0',
            int_status & IK_PPPOE_TERMINATED ? '1' : '0',
            int_status & IK_DEST_UNREACH ? '1' : '0',
            int_status & IK_IP_CONFLICT ? '1' : '0'  );   
     
    TRACE("SOCK_0=%c, SOCK_1=%c, SOCK_2=%c, SOCK_3=%c, SOCK_4=%c, SOCK_5=%c, SOCK_6=%c, SOCK_7=%c\r\n",
         int_status & IK_SOCK_0 ? '1' : '0',
         int_status & IK_SOCK_1 ? '1' : '0',
         int_status & IK_SOCK_2 ? '1' : '0',
         int_status & IK_SOCK_3 ? '1' : '0',
         int_status & IK_SOCK_4 ? '1' : '0',
         int_status & IK_SOCK_5 ? '1' : '0',
         int_status & IK_SOCK_6 ? '1' : '0',
         int_status & IK_SOCK_7 ? '1' : '0'  
         );     
   
   // Set interrupt timeout
   uint16_t intrtime = 100;
   ctlwizchip(CW_SET_INTRTIME, (void*)&intrtime); 
   
   intrtime = 0;
   ctlwizchip(CW_GET_INTRTIME, (void*)&intrtime); 
   
   TRACE("\r\n=== INT MASK ===\r\n");
   TRACE("INTRTIME = %d\r\n", intrtime);   
   
}

void eth_clear_int(void)
{
  // Get int status
   intr_kind int_status  = 0;    
   ctlwizchip(CW_GET_INTERRUPT, (void*)&int_status);   
   
   TRACE("\r\n=== INT STATUS ===\r\n");
   //TRACE("IK_WOL = %d\r\n", ((int_status&IK_WOL) == 0) ? 0 : 1);
   //TRACE("IK_PPPOE_TERMINATED = %d\r\n", ((int_status&IK_PPPOE_TERMINATED) == 0) ? 0 : 1); 
   //TRACE("IK_DEST_UNREACH = %d\r\n", ((int_status&IK_DEST_UNREACH) ==0) ? 0 : 1);  
   //TRACE("IK_IP_CONFLICT = %d\r\n", ((int_status&IK_IP_CONFLICT) ==0) ? 0 : 1);
   //TRACE("IK_SOCK_0 = %d\r\n", ((int_status&IK_SOCK_0) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_1 = %d\r\n", ((int_status&IK_SOCK_1) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_2 = %d\r\n", ((int_status&IK_SOCK_2) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_3 = %d\r\n", ((int_status&IK_SOCK_3) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_4 = %d\r\n", ((int_status&IK_SOCK_4) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_5 = %d\r\n", ((int_status&IK_SOCK_5) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_6 = %d\r\n", ((int_status&IK_SOCK_6) == 0) ? 0 : 1);
   //TRACE("IK_SOCK_7 = %d\r\n", ((int_status&IK_SOCK_7) == 0) ? 0 : 1);   
   //TRACE("IK_SOCK_ALL = 0x%02X\r\n", int_status&IK_SOCK_ALL);      
   
    TRACE("SOCK_0=%c, SOCK_1=%c, SOCK_2=%c, SOCK_3=%c, SOCK_4=%c, SOCK_5=%c, SOCK_6=%c, SOCK_7=%c\r\n",
         int_status & IK_SOCK_0 ? '1' : '0',
         int_status & IK_SOCK_1 ? '1' : '0',
         int_status & IK_SOCK_2 ? '1' : '0',
         int_status & IK_SOCK_3 ? '1' : '0',
         int_status & IK_SOCK_4 ? '1' : '0',
         int_status & IK_SOCK_5 ? '1' : '0',
         int_status & IK_SOCK_6 ? '1' : '0',
         int_status & IK_SOCK_7 ? '1' : '0'  
         );     
   
   // Clear int
   ctlwizchip(CW_CLR_INTERRUPT, (void*)&int_status);    
}

int16_t eth_get_int_sn (void)
{
  intr_kind int_status  = 0;
  int16_t sn_int = 0;
  ctlwizchip(CW_GET_INTERRUPT, (void*)&int_status);
  
   TRACE("\r\n=== INT STATUS ===\r\n");   
   //TRACE( "WOL=%c, PPPOE_TERMINATED=%c, DEST_UNREACH=%c, IP_CONFLICT=%c\r\n",
   //         int_status & IK_WOL ? '1' : '0',
   //         int_status & IK_PPPOE_TERMINATED ? '1' : '0',
   //         int_status & IK_DEST_UNREACH ? '1' : '0',
   //         int_status & IK_IP_CONFLICT ? '1' : '0'  );     
   
    TRACE("SOCK_0=%c, SOCK_1=%c, SOCK_2=%c, SOCK_3=%c, SOCK_4=%c, SOCK_5=%c, SOCK_6=%c, SOCK_7=%c\r\n",
         int_status & IK_SOCK_0 ? '1' : '0',
         int_status & IK_SOCK_1 ? '1' : '0',
         int_status & IK_SOCK_2 ? '1' : '0',
         int_status & IK_SOCK_3 ? '1' : '0',
         int_status & IK_SOCK_4 ? '1' : '0',
         int_status & IK_SOCK_5 ? '1' : '0',
         int_status & IK_SOCK_6 ? '1' : '0',
         int_status & IK_SOCK_7 ? '1' : '0'  
         );     
    
   if (int_status & (IK_SOCK_0|IK_SOCK_1|IK_SOCK_2|IK_SOCK_3|IK_SOCK_4|IK_SOCK_5|IK_SOCK_6|IK_SOCK_7))
   {
      if (int_status & IK_SOCK_0) sn_int = 0;
      else if (int_status & IK_SOCK_1) sn_int = 1;
      else if (int_status & IK_SOCK_2) sn_int = 2;
      else if (int_status & IK_SOCK_3) sn_int = 3;
      else if (int_status & IK_SOCK_4) sn_int = 4;
      else if (int_status & IK_SOCK_5) sn_int = 5;
      else if (int_status & IK_SOCK_6) sn_int = 6;
      else if (int_status & IK_SOCK_7) sn_int = 7;      
   }
   else
     sn_int = -1;
   
   if (int_status)
   {
     TRACE("\r\n=== INTERRUPT\r\n");
   }
   
   if (sn_int >= 0)
    eth_get_socket_int_type(sn_int);
    
   return (sn_int);
}

uint8_t eth_get_socket_int_type (uint8_t sn)
{
   uint8_t soc_int_type = 0;   
   TRACE("%d:INT SOCKET:\r\n", sn);
   soc_int_type = getSn_IR(sn);       
   
   TRACE( "CON=%c, DISCON=%c, RECV=%c, TIMEOUT=%c, SEND_OK=%c\r\n",
            soc_int_type & Sn_IR_CON ? '1' : '0',
            soc_int_type & Sn_IR_DISCON ? '1' : '0',
            soc_int_type & Sn_IR_RECV ? '1' : '0',
            soc_int_type & Sn_IR_TIMEOUT ? '1' : '0',
            soc_int_type & Sn_IR_SENDOK ? '1' : '0'
         );   
   
   return soc_int_type;
}

void stm32_exti0_isr(void)
{
   //static uint32_t eth_int_counter = 0;
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
   BaseType_t rtos_result;
   app_pipe_t      result = 
   {
     .cnt = 0, 
     .data = NULL,
     .param_data = 0,
     .param_id = 0,
     .tag = 0
   };  
   //eth_int_counter++;
   //TRACE("Eth int counter = %d\r\n", eth_int_counter);
  
   result.tag    = OS_USER_TAG_ETH_INT;  
 
   rtos_result = xQueueSendFromISR( que_net_hndl, &result, &xHigherPriorityTaskWoken );  
   if (rtos_result != pdTRUE )
   {
     TRACE("Queue FULL!!!\r\n");
   }
   
   portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}