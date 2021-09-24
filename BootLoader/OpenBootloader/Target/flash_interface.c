/**
  ******************************************************************************
  * @file    flash_interface.c
  * @author  MCD Application Team
  * @brief   Contains FLASH access functions
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
#include "platform.h"

#include "Mem/openbl_mem.h"

#include "app_openbootloader.h"
#include "common_interface.h"
#include "flash_layout.h"
#include "flash_interface.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
//static void OPENBL_FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data);
static void OPENBL_FLASH_ProgramWord(uint32_t Address, uint32_t Data);
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *ListOfPages, uint32_t Length);
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void);

/* Exported variables --------------------------------------------------------*/
OPENBL_MemoryTypeDef FLASH_Descriptor =
{
  FLASH_START_ADDRESS,
  FLASH_END_ADDRESS,
  (512U * 1024U),
  FLASH_AREA,
  OPENBL_FLASH_Read,
  OPENBL_FLASH_Write,
  OPENBL_FLASH_SetReadOutProtectionLevel,
  OPENBL_FLASH_SetWriteProtection,
  OPENBL_FLASH_JumpToAddress,
  NULL,
  OPENBL_FLASH_Erase
};

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Unlock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Unlock(void)
{
  HAL_FLASH_Unlock();
}

/**
  * @brief  Lock the FLASH control register access.
  * @retval None.
  */
void OPENBL_FLASH_Lock(void)
{
  HAL_FLASH_Lock();
}

/**
  * @brief  Unlock the FLASH Option Bytes Registers access.
  * @retval None.
  */
void OPENBL_FLASH_OB_Unlock(void)
{
  HAL_FLASH_Unlock();

  HAL_FLASH_OB_Unlock();
}

/**
  * @brief  Launch the option byte loading.
  * @retval None.
  */
void OPENBL_FLASH_OB_Launch(void)
{
  HAL_FLASH_OB_Launch();
}

/**
  * @brief  This function is used to read data from a given address.
  * @param  Address The address to be read.
  * @retval Returns the read value.
  */
uint8_t OPENBL_FLASH_Read(uint32_t Address)
{
  return (*(uint8_t *)(Address));
}

/**
  * @brief  This function is used to write data in FLASH memory.
  * @param  Address The address where that data will be written.
  * @param  Data The data to be written.
  * @param  DataLength The length of the data to be written.
  * @retval None.
  */
void OPENBL_FLASH_Write(uint32_t Address, uint8_t *Data, uint32_t DataLength)
{
  uint32_t index  = 0U;
  uint32_t length = DataLength;

  if (length & 7U)
  {
    length = (length & 0xFFFFFFF8U) + 8U;
  }

  /* Unlock the flash memory for write operation */
  OPENBL_FLASH_Unlock();

  #if 0
  for (index = 0U; index < length; (index += 8U))
  {
    OPENBL_FLASH_ProgramDoubleWord((Address + index), (uint64_t)(*((uint64_t *)((uint32_t)Data + index))));
  }
  #else
  for (index = 0U; index < length; (index += 4U))
  {
    OPENBL_FLASH_ProgramWord((Address + index), (uint32_t)(*((uint32_t *)((uint32_t)Data + index))));
  }
  #endif
  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();
}

/**
  * @brief  This function is used to jump to a given address.
  * @param  Address The address where the function will jump.
  * @retval None.
  */
void OPENBL_FLASH_JumpToAddress(uint32_t Address)
{
  Function_Pointer jump_to_address;

  /* Deinitialize all HW resources used by the Bootloader to their reset values */
  OpenBootloader_DeInit();

  /* Enable IRQ */
  Common_EnableIrq();

  jump_to_address = (Function_Pointer)(*(__IO uint32_t *)(Address + 4U));

  /* Initialize user application's stack pointer */
  Common_SetMsp(*(__IO uint32_t *) Address);

  jump_to_address();
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @retval The return value can be one of the following values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  */
uint32_t OPENBL_FLASH_GetReadOutProtectionLevel(void)
{
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Get the Option bytes configuration */
  HAL_FLASHEx_OBGetConfig(&flash_ob);

  return flash_ob.RDPLevel;
}

/**
  * @brief  Return the FLASH Read Protection level.
  * @param  Level Can be one of these values:
  *         @arg OB_RDP_LEVEL_0: No protection
  *         @arg OB_RDP_LEVEL_1: Read protection of the memory
  *         @arg OB_RDP_LEVEL_2: Full chip protection
  * @retval None.
  */
void OPENBL_FLASH_SetReadOutProtectionLevel(uint32_t Level)
{
  #if 0
  FLASH_OBProgramInitTypeDef flash_ob;

  if (Level != OB_RDP_LEVEL2)
  {
    flash_ob.OptionType = OPTIONBYTE_RDP;
    flash_ob.RDPLevel   = Level;

    /* Unlock the FLASH registers & Option Bytes registers access */
    OPENBL_FLASH_OB_Unlock();

    /* Clear error programming flags */
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

    /* Change the RDP level */
    HAL_FLASHEx_OBProgram(&flash_ob);

    /* Reset PCROP registers if RDP level 0 is set */
    if (Level == OB_RDP_LEVEL0)
    {
      flash_ob.OptionType      = OPTIONBYTE_PCROP;
      flash_ob.PCROPConfig     = FLASH_BANK_1;
      flash_ob.PCROPStartAddr  = 0x08007FFFU;
      flash_ob.PCROPEndAddr    = 0x08000000U;

      /* Change PCROP1 registers */
      HAL_FLASHEx_OBProgram(&flash_ob);

      flash_ob.PCROPStartAddr  = 0x08047FFFU;
      flash_ob.PCROPEndAddr    = 0x08040000U;
      flash_ob.PCROPConfig = FLASH_BANK_2;

      /* Change PCROP2 registers */
      HAL_FLASHEx_OBProgram(&flash_ob);
    }
    else
    {
      /* Nothing to do */
    }
  }
  else
  {
    /* Nothing to do */
  }
  #endif
}

/**
  * @brief  This function is used to enable or disable write protection of the specified FLASH areas.
  * @param  State Can be one of these values:
  *         @arg DISABLE: Disable FLASH write protection
  *         @arg ENABLE: Enable FLASH write protection
  * @param  ListOfPages Contains the list of pages to be protected.
  * @param  Length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
ErrorStatus OPENBL_FLASH_SetWriteProtection(FunctionalState State, uint8_t *ListOfPages, uint32_t Length)
{
  ErrorStatus status = SUCCESS;

  if (State == ENABLE)
  {
    OPENBL_FLASH_EnableWriteProtection(ListOfPages, Length);
  }
  else if (State == DISABLE)
  {
    OPENBL_FLASH_DisableWriteProtection();
  }
  else
  {
    status = ERROR;
  }

  return status;
}

/**
  * @brief  This function is used to start FLASH mass erase operation.
  * @param  *p_Data Pointer to the buffer that contains mass erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Mass erase operation done
  *          - ERROR:   Mass erase operation failed or the value of one parameter is not ok
  */
ErrorStatus OPENBL_FLASH_MassErase(uint8_t *p_Data, uint32_t DataLength)
{
#if 0
  uint32_t page_error  = 0U;
  uint16_t bank_option = 0U;
  ErrorStatus status   = SUCCESS;
  FLASH_EraseInitTypeDef erase_init_struct;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  erase_init_struct.TypeErase = FLASH_TYPEERASE_MASSERASE;

  if (DataLength >= 2)
  {
    bank_option = *(uint16_t *)(p_Data);

    if (bank_option == FLASH_MASS_ERASE)
    {
      erase_init_struct.Banks = 0U;
    }
    else if (bank_option == FLASH_BANK1_ERASE)
    {
      erase_init_struct.Banks = FLASH_BANK_1;
    }
    else if (bank_option == FLASH_BANK2_ERASE)
    {
      erase_init_struct.Banks = FLASH_BANK_2;
    }
    else
    {
      status = ERROR;
    }

    if (status == SUCCESS)
    {
      if (HAL_FLASHEx_Erase(&erase_init_struct, &page_error) != HAL_OK)
      {
        status = ERROR;
      }
      else
      {
        status = SUCCESS;
      }
    }
    else
    {
      /* Nothing to do */
    }
  }
  else
  {
    status = ERROR;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  return status;
#else
  return ERROR;
#endif
}

/**
  * @brief  This function is used to erase the specified FLASH pages.
  * @param  *p_Data Pointer to the buffer that contains erase operation options.
  * @param  DataLength Size of the Data buffer.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Erase operation done
  *          - ERROR:   Erase operation failed or the value of one parameter is not ok
  */
ErrorStatus OPENBL_FLASH_Erase(uint8_t *p_Data, uint32_t DataLength)
{
  uint32_t counter      = 0U;
  uint32_t pages_number = 0U;
  uint32_t page_error   = 0U;
  uint32_t errors       = 0U;
  ErrorStatus status    = SUCCESS;

  uint32_t targetPage;

  FLASH_EraseInitTypeDef erase_init_struct;

  /* Unlock the flash memory for erase operation */
  OPENBL_FLASH_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  pages_number  = (uint32_t)(*(uint16_t *)(p_Data));
  p_Data       += 2;

  erase_init_struct.TypeErase = FLASH_TYPEERASE_PAGES;
  erase_init_struct.NbPages   = 1U;

  for (counter = 0U; ((counter < pages_number) && (counter < (DataLength / 2U))) ; counter++)
  {
    #if 0
    erase_init_struct.Page = ((uint32_t)(*(uint16_t *)(p_Data)));
    if (erase_init_struct.Page <= 127)
    {
      erase_init_struct.Banks = FLASH_BANK_1;
    }
    else if (erase_init_struct.Page <= 255)
    {
      erase_init_struct.Banks = FLASH_BANK_2;
    }
    else
    {
      status = ERROR;
    }
    #else
    targetPage = ((uint32_t)(*(uint16_t *)(p_Data)));
    erase_init_struct.PageAddress = FLASHLAYOUT_GetStartAddress(targetPage);
    if(erase_init_struct.PageAddress == 0)
    {
       status = ERROR;
    }
    #endif
    if (status != ERROR)
    {
      if (HAL_FLASHEx_Erase(&erase_init_struct, &page_error) != HAL_OK)
      {
        errors++;
      }
      else
      {
        /* Nothing to do */
      }
    }
    else
    {
      /* Reset the status for next erase operation */
      status = SUCCESS;
    }

    p_Data += 2;
  }

  /* Lock the Flash to disable the flash control register access */
  OPENBL_FLASH_Lock();

  if (errors > 0)
  {
    status = ERROR;
  }
  else
  {
    status = SUCCESS;
  }

  return status;
}

/* Private functions ---------------------------------------------------------*/
#if 0
/**
  * @brief  Program double word at a specified FLASH address.
  * @param  Address specifies the address to be programmed.
  * @param  Data specifies the data to be programmed.
  * @retval None.
  */
static void OPENBL_FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data)
{
  /* Clear all FLASH errors flags before starting write operation */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, Address, Data);
}
#else
/**
  * @brief  Program double word at a specified FLASH address.
  * @param  Address specifies the address to be programmed.
  * @param  Data specifies the data to be programmed.
  * @retval None.
  */
static void OPENBL_FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  /* Clear all FLASH errors flags before starting write operation */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address, Data);
}
#endif


/**
  * @brief  This function is used to enable write protection of the specified FLASH areas.
  * @param  ListOfPages Contains the list of pages to be protected.
  * @param  Length The length of the list of pages to be protected.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_EnableWriteProtection(uint8_t *ListOfPages, uint32_t Length)
{
  #if 0
  uint8_t wrp_start_offset = 0x7F;
  uint8_t wrp_end_offset   = 0x00;
  ErrorStatus status       = SUCCESS;
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Write protection of bank 1 area WRPA 1 area */
  if (Length >= 2)
  {
    wrp_start_offset = *(ListOfPages);
    wrp_end_offset   = *(ListOfPages + 1);

    flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAA;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    HAL_FLASHEx_OBProgram(&flash_ob);
  }

  /* Write protection of bank 1 area WRPA 2 area */
  if (Length >= 4)
  {
    wrp_start_offset = *(ListOfPages + 2);
    wrp_end_offset   = *(ListOfPages + 3);

    flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAB;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    HAL_FLASHEx_OBProgram(&flash_ob);
  }

  /* Write protection of bank 2 area WRPB 1 area */
  if (Length >= 6)
  {
    wrp_start_offset = *(ListOfPages + 4);
    wrp_end_offset   = *(ListOfPages + 5);

    flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAA;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    HAL_FLASHEx_OBProgram(&flash_ob);
  }

  /* Write protection of bank 2 area WRPB 2 area */
  if (Length >= 8)
  {
    wrp_start_offset = *(ListOfPages + 6);
    wrp_end_offset   = *(ListOfPages + 7);

    flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAB;
    flash_ob.WRPStartOffset = wrp_start_offset;
    flash_ob.WRPEndOffset   = wrp_end_offset;

    HAL_FLASHEx_OBProgram(&flash_ob);
  }

  return status;
  #else
  return ERROR;
  #endif
}

/**
  * @brief  This function is used to disable write protection.
  * @retval An ErrorStatus enumeration value:
  *          - SUCCESS: Enable or disable of the write protection is done
  *          - ERROR:   Enable or disable of the write protection is not done
  */
static ErrorStatus OPENBL_FLASH_DisableWriteProtection(void)
{
  #if 0
  uint8_t wrp_start_offset = 0x7F;
  uint8_t wrp_end_offset   = 0x00;
  ErrorStatus status       = SUCCESS;
  FLASH_OBProgramInitTypeDef flash_ob;

  /* Unlock the FLASH registers & Option Bytes registers access */
  OPENBL_FLASH_OB_Unlock();

  /* Clear error programming flags */
  __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

  flash_ob.OptionType = OPTIONBYTE_WRP;

  /* Disable write protection of bank 1 area WRPA 1 area */
  flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAA;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;

  HAL_FLASHEx_OBProgram(&flash_ob);

  /* Disable write protection of bank 1 area WRPA 2 area */
  flash_ob.WRPArea        = OB_WRPAREA_BANK1_AREAB;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;

  HAL_FLASHEx_OBProgram(&flash_ob);

  /* Disable write protection of bank 2 area WRPB 1 area */
  flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAA;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;

  HAL_FLASHEx_OBProgram(&flash_ob);

  /* Disable write protection of bank 2 area WRPB 2 area */
  flash_ob.WRPArea        = OB_WRPAREA_BANK2_AREAB;
  flash_ob.WRPStartOffset = wrp_start_offset;
  flash_ob.WRPEndOffset   = wrp_end_offset;

  HAL_FLASHEx_OBProgram(&flash_ob);

  return status;
  #else
  return ERROR;
  #endif  
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
