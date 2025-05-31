#include "stm32.h"
#include "os\os_user.h"
#include "app_pipe.h"
#include "cmsis_os.h"
#include "w5500_work.h"

//extern QueueHandle_t            que_hmi_hndl;


void eth_init (void);
void eth_deinit (void);
void eth_network_init(void);
void eth_clear_int(void);
int16_t eth_get_int_sn (void);
uint8_t eth_get_socket_int_type (uint8_t sn);