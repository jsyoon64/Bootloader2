/**
  ******************************************************************************
  * @file    fs_interface.h
  * @author  
  * @brief   Header for fs_interface.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FS_INTERFACE_H
#define FS_INTERFACE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/** Structure type for grouping FATFS related objects used by this module. */
//typedef struct
//{
//  FATFS fs;                                      /**< file system object for mouting   */
//  FIL   file;                                    /**< file object for firmware file    */
//} tFatFsObjects;

//extern tFatFsObjects fatFsObjects;

/* Exported constants --------------------------------------------------------*/
#define OPEN_BL_FS_JUMP_APP_ADDR 0x08008000

/* Exported macro ------------------------------------------------------------*/
extern const char *firmwareFilename;

/* Exported functions ------------------------------------------------------- */
void OPENBL_FS_Configuration(void);
uint8_t OPENBL_FS_ProtocolDetection(void);
uint8_t OPENBL_FS_GetCommandOpcode(void);
void OPENBL_FS_SendByte(uint8_t Byte);

#endif /* FS_INTERFACE_H */
