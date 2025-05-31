/**
  * @file    task_root.c
  * @brief   Task Root
  * @author  Igor T. <research.tahoe@gmail.com>
  */


#include "cmsis_os.h"
#include "app_pipe.h"
#include "os\os_user.h"
#include "config.h"

/*******************************************************************************
* PRIVATE CONSTANTS
*******************************************************************************/
#define TASK_STACK_SIZE_IBUS    5120
#define TASK_STACK_SIZE_M2M     1024
#define TASK_STACK_SIZE_HMI     4096
#define TASK_STACK_SIZE_NET     2048

#define TASK_PRIO_IBUS          osPriorityNormal
#define TASK_PRIO_M2M           osPriorityNormal
#define TASK_PRIO_HMI           osPriorityNormal
#define TASK_PRIO_NET           osPriorityNormal

#define QUE_SIZE_HMI            16
#define QUE_SIZE_IBUS           16
#define QUE_SIZE_M2M            16
#define QUE_SIZE_MDBS_CLNT      16
#define QUE_SIZE_NET            16

/*******************************************************************************
*
*******************************************************************************/
#pragma data_alignment = 8
static  uint32_t                task_stck_ibus[ TASK_STACK_SIZE_IBUS ];

#pragma data_alignment = 8
static  uint32_t                task_stck_m2m[ TASK_STACK_SIZE_M2M ];

#pragma data_alignment = 8
static  uint32_t                task_stck_hmi[ TASK_STACK_SIZE_HMI ];

#pragma data_alignment = 8
static  uint32_t                task_stck_net[ TASK_STACK_SIZE_HMI ];

        osThreadId              task_hndl_ibus;
        osThreadId              task_hndl_m2m;
        osThreadId              task_hndl_hmi;
        osThreadId              task_hndl_net;        

        osStaticThreadDef_t     tcb_ibus;
        osStaticThreadDef_t     tcb_m2m;
        osStaticThreadDef_t     tcb_hmi;
        osStaticThreadDef_t     tcb_net;        

#pragma data_alignment = 8
static  uint8_t                 que_hmi_alloc[ QUE_SIZE_HMI * sizeof( app_pipe_t ) ];
static  StaticQueue_t           que_hmi_qcb;
        QueueHandle_t           que_hmi_hndl;

#pragma data_alignment = 8
static  uint8_t                 que_ibus_alloc[ QUE_SIZE_IBUS * sizeof( app_pipe_t ) ];
static  osStaticMessageQDef_t   que_ibus_qcb;
        QueueHandle_t           que_ibus_hndl;

#pragma data_alignment = 8
static  uint8_t                 que_m2m_alloc[ QUE_SIZE_M2M * sizeof( app_pipe_t ) ];
static  osStaticMessageQDef_t   que_m2m_qcb;
        QueueHandle_t           que_m2m_hndl;

#pragma data_alignment = 8
static  uint8_t                 que_mdbs_clnt_alloc[ QUE_SIZE_MDBS_CLNT * sizeof( app_pipe_t ) ];
static  osStaticMessageQDef_t   que_mdbs_clnt_qcb;
        QueueHandle_t           que_mdbs_clnt_hndl;
        
#pragma data_alignment = 8
static  uint8_t                 que_net_alloc[ QUE_SIZE_NET * sizeof( app_pipe_t ) ];
static  StaticQueue_t           que_net_qcb;
        QueueHandle_t           que_net_hndl;
        
__weak  void    send_cmd_to_net_task( os_user_tag_t tag ) {};       


/*******************************************************************************
*
*******************************************************************************/
/**
  * @brief  Start Thread
  * @param  argument not used
  * @retval None
  */
void
task_start(                     void    const *         argument )
{
    (void) argument;


    que_hmi_hndl        = xQueueCreateStatic(   QUE_SIZE_HMI,
                                                sizeof(app_pipe_t),
                                                que_hmi_alloc,
                                                &que_hmi_qcb );

    que_ibus_hndl       = xQueueCreateStatic(   QUE_SIZE_IBUS,
                                                sizeof(app_pipe_t),
                                                que_ibus_alloc,
                                                &que_ibus_qcb );

    que_m2m_hndl        = xQueueCreateStatic(   QUE_SIZE_M2M,
                                                sizeof(app_pipe_t),
                                                que_m2m_alloc,
                                                &que_m2m_qcb );

    que_mdbs_clnt_hndl  = xQueueCreateStatic(   QUE_SIZE_MDBS_CLNT,
                                                sizeof(app_pipe_t),
                                                que_mdbs_clnt_alloc,
                                                &que_mdbs_clnt_qcb );
#if CONFIG_HARDWARE_ID == 0x2331    
    que_net_hndl        = xQueueCreateStatic(   QUE_SIZE_NET,
                                                sizeof(app_pipe_t),
                                                que_net_alloc,
                                                &que_net_qcb );    
#endif

    osThreadStaticDef( IBUS,    task_ibus,  TASK_PRIO_IBUS, 0, TASK_STACK_SIZE_IBUS, task_stck_ibus, &tcb_ibus );
    task_hndl_ibus  =   osThreadCreate( osThread( IBUS ),   NULL );

    osThreadStaticDef( M2M,     task_m2m,  TASK_PRIO_M2M, 0, TASK_STACK_SIZE_M2M, task_stck_m2m, &tcb_m2m );
    task_hndl_m2m  =   osThreadCreate( osThread( M2M ),   NULL );

    osThreadStaticDef( HMI,      task_hmi,    TASK_PRIO_HMI,    0, TASK_STACK_SIZE_HMI,  task_stck_hmi,   &tcb_hmi   );
    task_hndl_hmi   =   osThreadCreate( osThread( HMI ), NULL );
    
#if CONFIG_HARDWARE_ID == 0x2331    
    osThreadStaticDef( NET,      task_net,    TASK_PRIO_NET,    0, TASK_STACK_SIZE_NET,  task_stck_net,   &tcb_net   );
    task_hndl_net   =   osThreadCreate( osThread( NET ), NULL ); 
#endif    
      
    while( 1 )
    {
        osDelay( 1000 );
    }
}