/**
  ******************************************************************************
  * @file    w25x40.c
  * @author  jsyoon
  * @brief   
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "w25x40Conf.h"
#if (_W25XXX_USE_FREERTOS == 1)
#include "cmsis_os.h"
#endif
#include "w25x40.h"
#include "gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define W25XXX_DUMMY_BYTE	0xA5

#define W25X_WRENA	      	0x06
#define W25X_WRDIS		  	0x04
#define W25X_RDSTATUSREG    0x05
#define W25X_WRSTATUSREG   	0x01
#define W25X_READDATA       0x03
#define W25X_FASTREADDATA   0x0B
#define W25X_FASTREADDUAL   0x3B
#define W25X_PAGEPROGRAM    0x02
#define W25X_BLOCKERASE     0xD8
#define W25X_SECTORERASE    0x20
#define W25X_CHIPERASE      0xC7
#define W25X_POWERDOWN      0xB9
#define W25X_RELPOWERDOWN 	0xAB

#define W25X_UNIQUEID       0x4B
#define W25X_DEVICEID 		0x90
#define W25X_DEVICEIDDUAL	0x92
#define W25X_JEDECDEVICEID  0x9F


/* Private macro -------------------------------------------------------------*/
#define W25X_NSS_H	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET)
#define W25X_NSS_L	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET)

#define W25XXX_PAGETOSECTOR(x) ((x * w25xxx.PageSize) / w25xxx.SectorSize)
#define W25XXX_PAGETOBLOCK(x) ((x * w25xxx.PageSize) / w25xxx.BlockSize)
#define W25XXX_SECTORTOBLOCK(x) ((x * w25xxx.SectorSize) / w25xxx.BlockSize)
#define W25XXX_SECTORTOPAGE(x) ((x * w25xxx.SectorSize) / w25xxx.PageSize)
#define W25XXX_BLOCKTOPAGE(x) ((x * w25xxx.BlockSize) / w25xxx.PageSize)

/* Private variables ---------------------------------------------------------*/
w25xxx_t w25xxx;

#if (_W25xxx_USE_FREERTOS == 1)
#define W25xxx_Delay(delay) osDelay(delay)
#else
#define W25xxx_Delay(delay) HAL_Delay(delay)
#endif

/* Exported variables --------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Read/Write 1 byte 
  * @param  uint8_t
  * @retval uint8_t
  */
uint8_t W25xxx_Spi(uint8_t Data)
{
	uint8_t ret;
	HAL_SPI_TransmitReceive(&_W25XXX_SPI, &Data, &ret, 1, 100);
	return ret;
}

/**
  * @brief  Read W25xxx Jedec ID
  * @param  None
  * @retval uint32_t
  */
uint32_t W25xxx_ReadID(void)
{
	uint32_t id;
	uint8_t Temp;

	W25X_NSS_L;
	W25xxx_Spi(W25X_JEDECDEVICEID);

	Temp = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	id = Temp;
	id <<= 8;

	Temp = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	id |= Temp;
	id <<= 8;

	Temp = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	id |= Temp;
	W25X_NSS_H;

	return id;
}

/**
  * @brief  Read W25xxx unique ID
  * @param  None
  * @retval void
  */
void W25xxx_ReadUniqID(void)
{
	uint8_t i;

	W25X_NSS_L;
	W25xxx_Spi(W25X_UNIQUEID);
	for (i = 0; i < 4; i++)
		W25xxx_Spi(W25XXX_DUMMY_BYTE);

	for (i = 0; i < 8; i++)
		w25xxx.UniqID[i] = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	W25X_NSS_H;
}

/**
  * @brief  W25xxx write enable
  * @param  None
  * @retval void
  */
void W25xxx_WriteEnable(void)
{
	W25X_NSS_L;
	W25xxx_Spi(W25X_WRENA);
	W25X_NSS_H;
	W25xxx_Delay(1);
}

/**
  * @brief  W25xxx write disable
  * @param  None
  * @retval void
  */
void W25xxx_WriteDisable(void)
{
	W25X_NSS_L;
	W25xxx_Spi(W25X_WRDIS);
	W25X_NSS_H;
	W25xxx_Delay(1);
}

/**
  * @brief  Read W25xxx status register
  * @param  None
  * @retval uint8_t status value
  */
uint8_t W25xxx_ReadStatusRegister(void)
{
	uint8_t status = 0;

	W25X_NSS_L;
	W25xxx_Spi(W25X_RDSTATUSREG);
	status = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	w25xxx.StatusRegister = status;
	W25X_NSS_H;

	return status;
}

/**
  * @brief  Write W25xxx status register
  * @param  data
  * @retval void
  */
void W25xxx_WriteStatusRegister(uint8_t Data)
{
	W25X_NSS_L;
	W25xxx_Spi(W25X_WRSTATUSREG);
	w25xxx.StatusRegister = Data;
	W25xxx_Spi(Data);
	W25X_NSS_H;
}

/**
  * @brief  Wait for write complete
  * @param  None
  * @retval void
  */
void W25xxx_WaitForWriteEnd(void)
{
	W25xxx_Delay(1);
	W25X_NSS_L;
	W25xxx_Spi(W25X_RDSTATUSREG);
	do
	{
		w25xxx.StatusRegister = W25xxx_Spi(W25XXX_DUMMY_BYTE);
		W25xxx_Delay(1);
	} while ((w25xxx.StatusRegister & 0x01) == 0x01);
	W25X_NSS_H;
}


/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize
  * @param  None
  * @retval void
  */
int W25xxx_Init(void)
{
	uint32_t id;

	w25xxx.Lock = 1;

	//while (HAL_GetTick() < 100)
	//	W25xxx_Delay(1);

	W25X_NSS_H;
	W25xxx_Delay(100);

	id = W25xxx_ReadID();

	//id & 0xFF == 0x12;
	//w25xxx.ID = W25X40CL;
	w25xxx.BlockCount = 8;
	w25xxx.PageSize = 256;
	w25xxx.SectorSize = 0x1000;
	w25xxx.SectorCount = w25xxx.BlockCount * 16;

	w25xxx.PageCount = (w25xxx.SectorCount * w25xxx.SectorSize) / w25xxx.PageSize;
	w25xxx.BlockSize = w25xxx.SectorSize * 16;

	w25xxx.CapacityInKiloByte = (w25xxx.SectorCount * w25xxx.SectorSize) / 1024;
	
	W25xxx_ReadUniqID();
	W25xxx_ReadStatusRegister();

#if (0)
	printf("W25xxx Page Size: %d Bytes\r\n", W25xxx.PageSize);
	printf("W25xxx Page Count: %d\r\n", W25xxx.PageCount);
	printf("W25xxx Sector Size: %d Bytes\r\n", W25xxx.SectorSize);
	printf("W25xxx Sector Count: %d\r\n", W25xxx.SectorCount);
	printf("W25xxx Block Size: %d Bytes\r\n", W25xxx.BlockSize);
	printf("W25xxx Block Count: %d\r\n", W25xxx.BlockCount);
	printf("W25xxx Capacity: %d KiloBytes\r\n", W25xxx.CapacityInKiloByte);
	printf("W25xxx Init Done\r\n");
#endif
	w25xxx.Lock = 0;

	(void)id;
	//return true;
	return 0;
}

/**
  * @brief  Chip Erase
  * @param  None
  * @retval void
  */
void W25xxx_EraseChip(void)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);

	w25xxx.Lock = 1;

	W25xxx_WriteEnable();

	W25X_NSS_L;
	W25xxx_Spi(W25X_CHIPERASE);
	W25X_NSS_H;
	W25xxx_WaitForWriteEnd();

	W25xxx_Delay(10);
	w25xxx.Lock = 0;
}

/**
  * @brief  Sector Erase
  * @param  address to be erase
  * @retval void
  */
//void W25xxx_EraseSector(uint32_t SectorAddr)
void W25xxx_EraseSector(uint32_t address)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	W25xxx_WaitForWriteEnd();

	//address = SectorAddr * w25xxx.SectorSize;
	W25xxx_WriteEnable();
	W25X_NSS_L;
	W25xxx_Spi(W25X_SECTORERASE);
	W25xxx_Spi((address & 0xFF0000) >> 16);
	W25xxx_Spi((address & 0xFF00) >> 8);
	W25xxx_Spi(address & 0xFF);
	W25X_NSS_H;
	W25xxx_WaitForWriteEnd();

	W25xxx_Delay(1);
	w25xxx.Lock = 0;
}

/**
  * @brief  Sector Erase
  * @param  BlockAddr
  * @retval void
  */
void W25xxx_EraseBlock(uint32_t BlockAddr)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	W25xxx_WaitForWriteEnd();

	BlockAddr = BlockAddr * w25xxx.SectorSize * 16;
	W25xxx_WriteEnable();
	W25X_NSS_L;
	W25xxx_Spi(W25X_BLOCKERASE);
	W25xxx_Spi((BlockAddr & 0xFF0000) >> 16);
	W25xxx_Spi((BlockAddr & 0xFF00) >> 8);
	W25xxx_Spi(BlockAddr & 0xFF);
	W25X_NSS_H;
	W25xxx_WaitForWriteEnd();

	W25xxx_Delay(1);
	w25xxx.Lock = 0;
}

/**
  * @brief  Write 1 byte
  * @param  target address
  * @retval void
  */
void W25xxx_WriteByte(uint8_t pBuffer, uint32_t WriteAddr_inBytes)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	W25xxx_WaitForWriteEnd();
	W25xxx_WriteEnable();
	W25X_NSS_L;
	W25xxx_Spi(W25X_PAGEPROGRAM);
	W25xxx_Spi((WriteAddr_inBytes & 0xFF0000) >> 16);
	W25xxx_Spi((WriteAddr_inBytes & 0xFF00) >> 8);
	W25xxx_Spi(WriteAddr_inBytes & 0xFF);
	W25xxx_Spi(pBuffer);
	W25X_NSS_H;
	W25xxx_WaitForWriteEnd();

	w25xxx.Lock = 0;
}

/**
  * \brief  Program a page with data
  * \param pBuffer pointer to data
  * \param Address, should be aligned on page (256 bytes) if programming 256 bytes
  * \param OffsetInByte, offet in page (256 bytes)
  * \param dataSize size of data in bytes, max 256
  * \return void
  */
void W25xxx_WritePage(uint8_t *pBuffer, uint32_t Address, uint32_t OffsetInByte, uint32_t NumByteToWrite_up_to_PageSize)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	if (((NumByteToWrite_up_to_PageSize + OffsetInByte) > w25xxx.PageSize) || (NumByteToWrite_up_to_PageSize == 0))
		NumByteToWrite_up_to_PageSize = w25xxx.PageSize - OffsetInByte;
	if ((OffsetInByte + NumByteToWrite_up_to_PageSize) > w25xxx.PageSize)
		NumByteToWrite_up_to_PageSize = w25xxx.PageSize - OffsetInByte;

	W25xxx_WaitForWriteEnd();
	W25xxx_WriteEnable();

	W25X_NSS_L;
	Address = Address + OffsetInByte;
	W25xxx_Spi(W25X_PAGEPROGRAM);
	W25xxx_Spi((Address & 0xFF0000) >> 16);
	W25xxx_Spi((Address & 0xFF00) >> 8);
	W25xxx_Spi(Address & 0xFF);
	HAL_SPI_Transmit(&_W25XXX_SPI, pBuffer, NumByteToWrite_up_to_PageSize, 100);
	W25X_NSS_H;
	W25xxx_WaitForWriteEnd();

	W25xxx_Delay(1);
	w25xxx.Lock = 0;
}

/**
  * @brief  Read 1 byte
  * @param  target address
  * @retval void
  */
void W25xxx_ReadByte(uint8_t *pBuffer, uint32_t Bytes_Address)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	W25X_NSS_L;
	#if 1
	W25xxx_Spi(W25X_FASTREADDATA);
	W25xxx_Spi((Bytes_Address & 0xFF0000) >> 16);
	W25xxx_Spi((Bytes_Address & 0xFF00) >> 8);
	W25xxx_Spi(Bytes_Address & 0xFF);
	W25xxx_Spi(0);
	#else
	W25xxx_Spi(W25X_READDATA);
	W25xxx_Spi((Bytes_Address & 0xFF0000) >> 16);
	W25xxx_Spi((Bytes_Address & 0xFF00) >> 8);
	W25xxx_Spi(Bytes_Address & 0xFF);
	#endif
	*pBuffer = W25xxx_Spi(W25XXX_DUMMY_BYTE);
	W25X_NSS_H;

	w25xxx.Lock = 0;
}

/**
  * \brief  Read bytes
  * \param  ReadAddr address
  * \param  NumByteToRead size of data in bytes
  * \retval void
  */
void W25xxx_ReadBytes(uint8_t *pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
	while (w25xxx.Lock == 1)
		W25xxx_Delay(1);
	w25xxx.Lock = 1;

	W25X_NSS_L;
	#if 1
	W25xxx_Spi(W25X_FASTREADDATA);
	W25xxx_Spi((ReadAddr & 0xFF0000) >> 16);
	W25xxx_Spi((ReadAddr & 0xFF00) >> 8);
	W25xxx_Spi(ReadAddr & 0xFF);
	W25xxx_Spi(0);
	#else
	W25xxx_Spi(W25X_READDATA);
	W25xxx_Spi((ReadAddr & 0xFF0000) >> 16);
	W25xxx_Spi((ReadAddr & 0xFF00) >> 8);
	W25xxx_Spi(ReadAddr & 0xFF);
	#endif
	HAL_SPI_Receive(&_W25XXX_SPI, pBuffer, NumByteToRead, 2000);
	W25X_NSS_H;

	W25xxx_Delay(1);
	w25xxx.Lock = 0;
}

#if 0
/**
  * \brief  Read a page
  * \param pBuffer pointer to data
  * \param Page_Address, should be aligned on page (256 bytes) if programming 256 bytes
  * \param OffsetInByte, offet in page (256 bytes)
  * \param dataSize size of data in bytes, max 256
  */
void W25xxx_ReadPage(uint8_t *pBuffer, uint32_t Page_Address, uint32_t OffsetInByte, uint32_t NumByteToRead_up_to_PageSize)
{
	while (W25xxx.Lock == 1)
		W25xxx_Delay(1);
	W25xxx.Lock = 1;
	if ((NumByteToRead_up_to_PageSize > W25xxx.PageSize) || (NumByteToRead_up_to_PageSize == 0))
		NumByteToRead_up_to_PageSize = W25xxx.PageSize;
	if ((OffsetInByte + NumByteToRead_up_to_PageSize) > W25xxx.PageSize)
		NumByteToRead_up_to_PageSize = W25xxx.PageSize - OffsetInByte;

	Page_Address = Page_Address * W25xxx.PageSize + OffsetInByte;
	W25X_NSS_L;
		W25xxx_Spi(W25X_FASTREADDATA);
	W25xxx_Spi((Page_Address & 0xFF0000) >> 16);
	W25xxx_Spi((Page_Address & 0xFF00) >> 8);
	W25xxx_Spi(Page_Address & 0xFF);
	W25xxx_Spi(0);
	HAL_SPI_Receive(&_W25XXX_SPI, pBuffer, NumByteToRead_up_to_PageSize, 100);
	W25X_NSS_H;

	W25xxx_Delay(1);
	W25xxx.Lock = 0;
}
#endif
