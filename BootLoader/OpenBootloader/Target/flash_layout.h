/**
  ******************************************************************************
  * @file    flash_layout.h
  * @author  jsyoon
  * @brief   Header for flash_layout.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef FLASH_LAYOUT_H
#define FLASH_LAYOUT_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
uint16_t FLASHLAYOUT_GetSectorIdx(uint32_t Address);
uint32_t FLASHLAYOUT_GetPages(uint32_t start_address, uint32_t total_size, uint8_t *RAM_Buf, uint16_t RAM_BUFFER_SIZE);
uint32_t FLASHLAYOUT_GetStartAddress(uint16_t page_number);
#endif /* FLASH_LAYOUT_H */