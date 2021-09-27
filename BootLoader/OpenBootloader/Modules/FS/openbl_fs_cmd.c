/**
  ******************************************************************************
  * @file    openbl_usart_cmd.c
  * @author  MCD Application Team
  * @brief   Contains USART protocol commands
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "Mem/openbl_mem.h"
#include "openbl_fs_cmd.h"

#include "openbootloader_conf.h"
#include "fs_interface.h"
#include "srecord_parse.h"

/* Private typedef -----------------------------------------------------------*/

/** \brief Structure type with information for the memory erase opeartion. */
typedef struct
{
  uint32_t  start_address;                      /**< erase start address              */
  uint32_t  total_size;                         /**< total number of bytes to erase   */
} tFileEraseInfo;

/* Private define ------------------------------------------------------------*/
#define OPENBL_FS_COMMANDS_NB             3U       /* Number of supported commands */

#define FS_RAM_BUFFER_SIZE                300U     /* Size of data buffer used to store readed data from S_RECORD FILE */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Buffer used to store received data from the host */
static uint8_t FS_RAM_Buf[FS_RAM_BUFFER_SIZE];

/** Local variable for storing S-record line parsing results. */
static tSrecLineParseObject lineParseObject;
/** Local variable for storing information regarding the memory erase operation.*/
static tFileEraseInfo       eraseInfo;

/* Private function prototypes -----------------------------------------------*/
static void OPENBL_FS_WriteMemory(void);
static void OPENBL_FS_Go(void);
static void OPENBL_FS_EraseMemory(void);

/* Exported variables --------------------------------------------------------*/
OPENBL_CommandsTypeDef OPENBL_FS_Commands =
{
  NULL,
  NULL,
  NULL,
  NULL,
  OPENBL_FS_WriteMemory,
  OPENBL_FS_Go,
  NULL,
  NULL,
  OPENBL_FS_EraseMemory,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};

/* Exported functions---------------------------------------------------------*/
/**
  * @brief  This function is used to get a pointer to the structure that contains the available USART commands.
  * @return Returns a pointer to the OPENBL_USART_Commands struct.
  */
OPENBL_CommandsTypeDef *OPENBL_FS_GetCommandsList(void)
{
  return (&OPENBL_FS_Commands);
}

/* Private functions ---------------------------------------------------------*/

/**
 * @brief  This function is used to write in to device memory.
 * @retval None.
 */
static void OPENBL_FS_WriteMemory(void)
{
  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FS_SendByte(NACK_BYTE);
  }
  else
  {
    #if 0
	uint32_t address;
	uint32_t tmpXOR;
	uint32_t counter;
	uint32_t codesize;
	uint32_t mem_area;
	uint8_t *ramaddress;
	uint8_t data;
	char   *read_line_ptr;

    eof = 0;
    while(!eof)
    {
      /* 1. read a line from the file */
      read_line_ptr = f_gets(lineParseObject.line, sizeof(lineParseObject.line), &fatFsObjects.file);

      /* 2. check if an error occurred */
      if (f_error(&fatFsObjects.file) > 0)
      {}

      /* 3. parse the S-Record line */
      if (read_line_ptr != NULL)
      {
        parse_result = FileSrecParseLine(&lineParseObject);
        /* check parsing result */
        if (parse_result == ERROR_SREC_INVALID_CHECKSUM) 
        {
          OPENBL_FS_SendByte(NACK_BYTE);
          //eof = 1;// ???
        }
        else if(parse_result > 0)
        {
          /* Write data to memory */
          OPENBL_MEM_Write(lineParseObject.address, (uint8_t *)lineParseObject.data, lineParseObject.data_len);
        }
      }
      /* check if the end of the file was reached */
      if (f_eof(&fatFsObjects.file) > 0)      
      {
        eof = 1;

        /* close the file */
        f_close(&fatFsObjects.file);

        /* finish the programming by writing the checksum */        
        // TODO
      }
    }
    #endif
  }
}

/**
  * @brief  This function is used to jump to the user application.
  * @retval None.
  */
static void OPENBL_FS_Go(void)
{
  uint32_t address = OPEN_BL_FS_JUMP_APP_ADDR;
  uint32_t mem_area;

  /* Check memory protection then send adequate response */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FS_SendByte(NACK_BYTE);
  }
  else
  {
    /* Check if received address is valid or not */
    mem_area = OPENBL_MEM_GetAddressArea(address);

    if ((mem_area != FLASH_AREA) && (mem_area != RAM_AREA))
    {
      OPENBL_FS_SendByte(NACK_BYTE);
    }
    else
    {
      /* If the jump address is valid then send ACK */
      OPENBL_FS_SendByte(ACK_BYTE);

      OPENBL_MEM_JumpToAddress(address);
    }
  }
}

/**
 * @brief  This function is used to erase a memory.
 * @retval None.
 */
static void OPENBL_FS_EraseMemory(void)
{

  /* prepare data objects for the erasing state */
  eraseInfo.start_address = 0;
  eraseInfo.total_size = 0;

  /* Check if the memory is not protected */
  if (OPENBL_MEM_GetReadOutProtectionStatus() != RESET)
  {
    OPENBL_FS_SendByte(NACK_BYTE);
  }
  else
  {
    #if 0
	uint32_t counter;
	uint32_t numpage;
	uint16_t data;
	ErrorStatus error_value;
	uint8_t status = ACK_BYTE;
	uint8_t *ramaddress;
    eof = 0;    
    while(!eof)
    {
      /* 1. read a line from the file */
      read_line_ptr = f_gets(lineParseObject.line, sizeof(lineParseObject.line), &fatFsObjects.file);

      /* 2. check if an error occurred */
      if (f_error(&fatFsObjects.file) > 0)
      {}

      /* 3. parse the S-Record line */
      if (read_line_ptr != NULL)
      {
        parse_result = FileSrecParseLine(&lineParseObject);
        if(parse_result > 0)
        {
          /* is this the first address/data info we encountered? */
          if (eraseInfo.total_size == 0)
          {
            /* store the start_address and byte count */
            eraseInfo.start_address = lineParseObject.address;
            eraseInfo.total_size = parse_result;
          }
          else
          {
            /* does this data fit at the end of the previously detected program block? */
            if (lineParseObject.address == (eraseInfo.start_address + eraseInfo.total_size))
            {
              /* update the byte count */
              eraseInfo.total_size += parse_result;
            }
            else
            {
              /* data does not belong to the previously detected block so there must be a
              * gap in the data. first erase the currently detected block and then start
              * tracking a new block.
              */

              // erase operation
              // TODO

              erased_size = eraseInfo.total_size;

              tFileEraseInfo  eraseOper;
              eraseOper.total_size = 0;
              eraseOper.start_address = eraseInfo.start_address;

              do {
                erased_size = FLASHLAYOUT_GetPages(eraseOper.start_address, erased_size, FS_RAM_Buf, FS_RAM_BUFFER_SIZE);
                error_value = OPENBL_MEM_Erase(FLASH_START_ADDRESS, (uint8_t *) FS_RAM_Buf, FS_RAM_BUFFER_SIZE);
                eraseOper.total_size += erased_size;
                eraseOper.start_address += erased_size;
              }while(eraseOper.total_size < eraseInfo.total_size)

              eraseInfo.start_address = 0;
              eraseInfo.total_size = 0;

              /* Errors from memory erase are not managed, always return ACK */
              if (error_value != SUCCESS)
              {
                OPENBL_FS_SendByte(NACK_BYTE);
                //eof = 1;// ???
              }
            }
          }
        }
        else 
        {
          OPENBL_FS_SendByte(NACK_BYTE);
          //eof = 1;// ???
        }
      }

      /* check if the end of the file was reached */
      if (f_eof(&fatFsObjects.file) > 0)      
      {
        eof = 1;

        /* close the file */
        f_close(&fatFsObjects.file);

        OPENBL_FS_SendByte(ACK_BYTE);

        /* finish the programming by writing the checksum */        
        // TODO
      }
    }
    #endif
  }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
