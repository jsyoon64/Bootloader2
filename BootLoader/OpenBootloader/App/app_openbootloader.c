/**
  ******************************************************************************
  * @file    app_openbootloader.c
  * @author  MCD Application Team
  * @brief   OpenBootloader application entry point
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Image license SLA0044,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                       www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_openbootloader.h"
#include "Target/usart_interface.h"
#include "Target/fs_interface.h"
//#include "fdcan_interface.h"
#include "Target/flash_interface.h"
#include "Target/ram_interface.h"
#include "Target/optionbytes_interface.h"
#include "Target/iwdg_interface.h"
#include "Target/otp_interface.h"
#include "Modules/USART/openbl_usart_cmd.h"
#include "Modules/FS/openbl_fs_cmd.h"
//#include "openbl_fdcan_cmd.h"
#include "Target/systemmemory_interface.h"

#include "Core/openbl_core.h"
#include "Modules/Mem/openbl_mem.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OPENBL_HandleTypeDef USART_Handle;
static OPENBL_HandleTypeDef FS_Handle;
//static OPENBL_HandleTypeDef FDCAN_Handle;
//static OPENBL_HandleTypeDef IWDG_Handle;

static OPENBL_OpsTypeDef USART_Ops =
{
  OPENBL_USART_Configuration,
  NULL,
  OPENBL_USART_ProtocolDetection,
  OPENBL_USART_GetCommandOpcode,
  OPENBL_USART_SendByte
};

#if 0
static OPENBL_OpsTypeDef FDCAN_Ops =
{
  OPENBL_FDCAN_Configuration,
  NULL,
  OPENBL_FDCAN_ProtocolDetection,
  OPENBL_FDCAN_GetCommandOpcode,
  OPENBL_FDCAN_SendByte
};
#endif

static OPENBL_OpsTypeDef FS_Ops =
{
  OPENBL_FS_Configuration,
  NULL,
  OPENBL_FS_ProtocolDetection,
  OPENBL_FS_GetCommandOpcode,
  OPENBL_FS_SendByte
};

#if 0
static OPENBL_OpsTypeDef IWDG_Ops =
{
  OPENBL_IWDG_Configuration,
  NULL,
  NULL,
  NULL,
  NULL
};
#endif

/* External variables --------------------------------------------------------*/
extern OPENBL_MemoryTypeDef FLASH_Descriptor;
extern OPENBL_MemoryTypeDef RAM_Descriptor;
extern OPENBL_MemoryTypeDef OB1_Descriptor;
extern OPENBL_MemoryTypeDef OB2_Descriptor;
extern OPENBL_MemoryTypeDef OTP_Descriptor;
extern OPENBL_MemoryTypeDef ICP1_Descriptor;
extern OPENBL_MemoryTypeDef ICP2_Descriptor;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_Init(void)
{
  /* Register USART interfaces */
  USART_Handle.p_Ops = &USART_Ops;
  USART_Handle.p_Cmd = OPENBL_USART_GetCommandsList();

  OPENBL_RegisterInterface(&USART_Handle);

  /* Register FS interfaces */
  FS_Handle.p_Ops = &FS_Ops;
  FS_Handle.p_Cmd = OPENBL_FS_GetCommandsList();

  OPENBL_RegisterInterface(&FS_Handle);

  
  /* Register FDCAN interfaces */
  /*
  FDCAN_Handle.p_Ops = &FDCAN_Ops;
  FDCAN_Handle.p_Cmd = OPENBL_FDCAN_GetCommandsList();

  OPENBL_RegisterInterface(&FDCAN_Handle);
  */

  /* Register IWDG interfaces */
  /*
  IWDG_Handle.p_Ops = &IWDG_Ops;
  IWDG_Handle.p_Cmd = NULL;

  OPENBL_RegisterInterface(&IWDG_Handle);
  */

  /* Initialise interfaces */
  OPENBL_Init();

  /* Initialise memories */
  OPENBL_MEM_RegisterMemory(&FLASH_Descriptor);
  OPENBL_MEM_RegisterMemory(&RAM_Descriptor);
  OPENBL_MEM_RegisterMemory(&OB1_Descriptor);
  //OPENBL_MEM_RegisterMemory(&OB2_Descriptor);
  OPENBL_MEM_RegisterMemory(&OTP_Descriptor);
  OPENBL_MEM_RegisterMemory(&ICP1_Descriptor);
  OPENBL_MEM_RegisterMemory(&ICP2_Descriptor);
}

/**
  * @brief  DeInitialize open Bootloader.
  * @param  None.
  * @retval None.
  */
void OpenBootloader_DeInit(void)
{
  //System_DeInit();
}

/**
  * @brief  This function is used to select which protocol will be used when communicating with the host.
  * @param  None.
  * @retval None.
  */
uint32_t OpenBootloader_ProtocolDetection(void)
{
  static uint32_t interface_detected = 0;

  if (interface_detected == 0)
  {
    interface_detected = OPENBL_InterfaceDetection();
  }

  if (interface_detected == 1)
  {
    OPENBL_CommandProcess();
  }
  return interface_detected;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
