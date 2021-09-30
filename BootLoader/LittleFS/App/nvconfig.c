/**
  ******************************************************************************
  * @file    nvconfig.c
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
/* Scheduler include files. */
/* Application include files. */
#include "nvconfig.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
const uint16_t nv_version = 0;

const device_info_type deviceinfo =
{
	3, /* default tx interval sec*/
	0, /* mode */
	1, // dev site, Default site 즉 첫 데이터가 0인경우 parsing에 문제 있음
	0, // dev id
	15, // 15A
	{"TEST"}, // client_id
	0, // rsvd1
	0, // rsvd2
	0, // rsvd3
	0, // rsvd4
	0, // rsvd5
	0, // rsvd6
	0, // rsvd7
	0  // rsvd8
};

const server_info_type serverinfo = 
{
	{192,168,0,62},	// server_ip
	#ifdef FEATURE_USE_MQTT_PROTOCOL	
	1883			// server_port
	#else
	3300
	#endif	
} ;

const wifi_net_info_type wifiinfo = 
{
	{"SNK_2G"},			// wifi ssid name
	{"12345678"},		// password		
	0					// rsvd
} ;

static const char *strDefaultServerIP = "192.168.10.10";
static const uint16_t DefaultServerPort = 3000;

/* Exported variables --------------------------------------------------------*/
system_info_data_type sys_data;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
/**
  * @brief  Initialize system parameter
  * @param  None.
  * @retval size of system parameter
  */
uint32_t nvcfg_init_sys(void)
{
	memcpy(&(sys_data.nv_version), &nv_version, sizeof(nv_version));
	memcpy(&(sys_data.dinfo), &deviceinfo, sizeof(device_info_type));
	memcpy(&(sys_data.sinfo), &serverinfo, sizeof(server_info_type));
	memcpy(&(sys_data.wifinfo), &wifiinfo, sizeof(wifi_net_info_type));
	return (sizeof(nv_version) + sizeof(device_info_type) + sizeof(server_info_type) + sizeof(wifi_net_info_type));
}

/**
  * @brief  Initialize xxx
  * @param  None.
  * @retval None.
  */
system_info_data_type nvcfg_sys_info_copy(void)
{
	return sys_data;
}

/**
  * @brief  Initialize xxx
  * @param  None.
  * @retval None.
  */
system_info_data_type *nvcfg_sys_info_ptr(void)
{
	return &sys_data;
}
/*===========================================================================*/