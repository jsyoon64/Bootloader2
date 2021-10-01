/**
  ******************************************************************************
  * @file    shared_mem.c
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
#include "shared_mem.h"
#include "shared_mem_api.h"
#include "../LittleFS/App/app_littlefs.h"
#include "gpio.h"

void SHARED_API_SECTION LedBlink(void);
uint16_t SHARED_API_SECTION TurnOn(void);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
//struct BootloaderSharedAPI BootAPI __attribute__((section(".API_SHARED"))) = {
_SharedMemAPI_ BootAPI __attribute__((section(".API_SHARED"))) = {
LedBlink,
TurnOn,
LittleFS_Format,
LittleFS_Mount,
LittleFS_Open,
LittleFS_Read,
LittleFS_Write,
LittleFS_Close,
LittleFS_Seek,
SrecGetLineType,
SrecGetLineLength,
SrecVerifyChecksum,
};

//unsigned char SHARED_BOOT_RAM littlefs[128];
uint16_t SHARED_BOOT_RAM count;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void sharedmem_init(void)
{
  count = 0;
}

void SHARED_API_SECTION LedBlink(void)
{
    HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
    //Application이 동작 할 경우 HAL_Delay가 동작 하지 않는다.
    //Tick value가 update 되지 않으므로    
    //HAL_Delay(dlyticks);
}

uint16_t SHARED_API_SECTION TurnOn(void)
{
    HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_SET);
    count %= 128;
    return count++;
}
/*===========================================================================*/
