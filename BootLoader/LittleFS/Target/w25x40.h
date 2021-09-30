/**
  ******************************************************************************
  * @file    w25x40.h
  * @author  jsyoon
  * @brief   
  ******************************************************************************
  */
#ifndef _W25X40_H_
#define _W25X40_H_
  
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
typedef struct
{
	//W25QXX_ID_t ID;
	uint8_t UniqID[8];
	uint16_t PageSize;
	uint32_t PageCount;
	uint32_t SectorSize;
	uint32_t SectorCount;
	uint32_t BlockSize;
	uint32_t BlockCount;
	uint32_t CapacityInKiloByte;
	uint8_t StatusRegister;
	uint8_t Lock;
} w25xxx_t;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int W25xxx_Init(void);
void W25xxx_EraseChip(void);
void W25xxx_EraseSector(uint32_t address);
void W25xxx_EraseBlock(uint32_t BlockAddr);
void W25xxx_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes);
void W25xxx_WritePage(uint8_t *pBuffer, uint32_t Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize);
void W25xxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address);
void W25xxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead);
#endif
