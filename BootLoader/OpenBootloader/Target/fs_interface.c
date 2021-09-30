/**
  ******************************************************************************
  * @file    fs_interface.c
  * @author  
  * @brief   Contains file system configuration
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "platform.h"

#include "Core/openbl_core.h"
#include "Modules/FS/openbl_fs_cmd.h"

#include "fs_interface.h"
#include "iwdg_interface.h"
#include "interfaces_conf.h"
#include "Core/lfs.h"
#include "App/app_littlefs.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/** Firmware filename. */
const char *firmwareFilename = "evse.srec";

//tFatFsObjects fatFsObjects;

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t response;
static uint8_t fs_command_list_index;
uint8_t const fs_command_list[] =
{
    CMD_EXT_ERASE_MEMORY,
    CMD_WRITE_MEMORY,
    CMD_GO
};

static int isFSMounted = -1;
lfs_file_t file;
/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
int openbl_FWfileOpen(void)
{
    int err = lfs_file_open(&lfs, &file, firmwareFilename, LFS_O_RDONLY);
    lfs_file_close(&lfs, &file);
    return err;
}
/* Exported functions --------------------------------------------------------*/

/**
 * @brief  This function is used to configure USART pins and then initialize the used USART instance.
 * @retval None.
 */
void OPENBL_FS_Configuration(void)
{
  response = BUSY_BYTE;
  fs_command_list_index = 0;

  isFSMounted = LittleFS_init();
}

/**
 * @brief  This function is used to detect if there is any activity on USART protocol.
 * @retval None.
 */
uint8_t OPENBL_FS_ProtocolDetection(void)
{
  static uint8_t detected = 0;

  if(isFSMounted < 0) {
    return 0;
  }

  // TODO
  /* Check if the Firmware file is being 
     attempt to obtain a file object for the firmware file 
  */
  if( (!detected) && (openbl_FWfileOpen() >=0 ))
  {
    detected = 1;
  }
  else
  {
    detected = 0;
  }

  return detected;
}

/**
 * @brief  This function is used to get the command opcode from the host.
 * @retval Returns the command.
 */
uint8_t OPENBL_FS_GetCommandOpcode(void)
{
  uint8_t command_opc = 0x0;

  /* Get the command opcode */
  command_opc = fs_command_list[fs_command_list_index];

  return command_opc;
}

/**
  * @brief  This function is used to send one byte through USART pipe.
  * @param  Byte The byte to be sent.
  * @retval None.
  */
void OPENBL_FS_SendByte(uint8_t Byte)
{
  response = Byte;

  // TODO
  // response 에 따라 프로그램 하도록 처리 필요
  // 

  if(response == ACK_BYTE) {
    fs_command_list_index++;
  }
  else {
    // TODO
    ///* close the file */
    //f_close(&fatFsObjects.file);
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
