#ifndef _NVCONFIG_H_
#define _NVCONFIG_H_
/**
  ******************************************************************************
  * @file    nvconfig.h
  * @author  jsyoon
  * @date    2021/09/28  
  * @brief   
  ******************************************************************************

Revision History:

Modification Tracking
Author          Date            Number          Description of Changes
--------        --------        -------         ------------------------

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* Standard includes. */
#include <stdint.h>

/* Scheduler include files. */
/* Application include files. */
#include "nvconst.h"

/* Exported types ------------------------------------------------------------*/
//typedef struct __attribute__((__packed__))
typedef struct
{
  uint8_t server_ip[NET_IP_SIZE]; //< Target IP Address
  uint16_t server_port;
} server_info_type;

//typedef struct __attribute__((__packed__))
typedef struct
{
  uint8_t dev_tx_interval;
  uint8_t dev_mode;
  uint16_t dev_site;
  uint32_t dev_id;
  float supply_current; // 4byte
  uint8_t client_id[NET_CLIENTID_SIZE];
  uint8_t rsvd1;
  uint8_t rsvd2;
  uint8_t rsvd3;
  uint8_t rsvd4;
  uint16_t rsvd5;
  uint16_t rsvd6;
  uint16_t rsvd7;
  uint16_t rsvd8;
} device_info_type;

typedef struct
{
  uint8_t wifi_ssid[WIFI_INFO_SIZE]; // wifi ssid info
  uint8_t wifi_pass[WIFI_INFO_SIZE]; // wifi password
  uint32_t rsvd;
} wifi_net_info_type;

typedef struct
{
  uint16_t nv_version;
  device_info_type dinfo;
  server_info_type sinfo;
  wifi_net_info_type wifinfo;
} system_info_data_type;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/*============================================================================*/
uint32_t nvcfg_init_sys(void);
system_info_data_type nvcfg_sys_info_copy(void);
system_info_data_type *nvcfg_sys_info_ptr(void);

#endif  /* _NVCONFIG_H_*/
