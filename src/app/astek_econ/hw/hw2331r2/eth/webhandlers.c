/**
  * @file    webhandlers.c
  * @brief   Handlers fpr web pages
  * @author  NKP144
  */

#include "hw2331_eth.h"
#include "webhandlers.h"

void make_json_netinfo(uint8_t * buf, uint16_t * len)
{
	wiz_NetInfo netinfo;
	ctlnetwork(CN_GET_NETINFO, (void*) &netinfo);

	// DHCP: 1 - Static, 2 - DHCP
	*len = sprintf((char *)buf, "NetinfoCallback({\"mac\":\"%.2X:%.2X:%.2X:%.2X:%.2X:%.2X\",\
						\"ip\":\"%d.%d.%d.%d\",\
						\"gw\":\"%d.%d.%d.%d\",\
						\"sn\":\"%d.%d.%d.%d\",\
						\"dns\":\"%d.%d.%d.%d\",\
						\"dhcp\":\"%d\"\
						});",
						netinfo.mac[0], netinfo.mac[1], netinfo.mac[2], netinfo.mac[3], netinfo.mac[4], netinfo.mac[5],
						netinfo.ip[0], netinfo.ip[1], netinfo.ip[2], netinfo.ip[3],
						netinfo.gw[0], netinfo.gw[1], netinfo.gw[2], netinfo.gw[3],
						netinfo.sn[0], netinfo.sn[1], netinfo.sn[2], netinfo.sn[3],
						netinfo.dns[0], netinfo.dns[1], netinfo.dns[2], netinfo.dns[3],
						netinfo.dhcp
											);
}