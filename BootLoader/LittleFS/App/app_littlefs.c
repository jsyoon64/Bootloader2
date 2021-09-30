/**
  ******************************************************************************
  * @file    app_littlefs.c
  * @author  jsyoon
  * @brief   littlefs filesystem interface
  ******************************************************************************
  */
  
/* Includes ------------------------------------------------------------------*/
#include "Core/lfs.h"
#include "app_littlefs.h"
#include "Target/w25x40.h"

/* Private function prototypes -----------------------------------------------*/
static int block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
static int block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
static int block_device_erase(const struct lfs_config *c, lfs_block_t block);
static int block_device_sync(const struct lfs_config *c);
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t lfs_read_buf[256];
uint8_t lfs_prog_buf[256];
uint8_t lfs_lookahead_buf[16];	// 128/8=16
uint8_t lfs_file_buf[256];

// variables used by the filesystem
lfs_t lfs;
struct lfs_info dir_info;

// variables for directory info

// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
    // block device operations
    .read  = block_device_read,
    .prog  = block_device_prog,
    .erase = block_device_erase,
    .sync  = block_device_sync,

    // block device configuration
    .read_size = 256,
    .prog_size = 256,
    .block_size = 4096, // 4k
    // Number of erasable blocks on the device.
    .block_count = 128, // block당 16 sector  * 8 block
    .cache_size = 256,//16,
    .lookahead_size = 16,
    .block_cycles = 500,


	.read_buffer = lfs_read_buf,
	.prog_buffer = lfs_prog_buf,
	.lookahead_buffer = lfs_lookahead_buf,
};

/* Private functions ---------------------------------------------------------*/

//
static int block_device_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    assert(block < c->block_count);
    W25xxx_ReadBytes((uint8_t *)buffer, (block * (c->block_size) + off), size);
    return 0;
}

static int block_device_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    assert(block < c->block_count);
    W25xxx_WritePage((uint8_t *)buffer, (block * (c->block_size)), off, size);
    return 0;
}

static int block_device_erase(const struct lfs_config *c, lfs_block_t block)
{
    assert(block < c->block_count);
    W25xxx_EraseSector(block * c->block_size);
    return 0;
}

static int block_device_sync(const struct lfs_config *c)
{
    return 0;
}

/* Exported functions --------------------------------------------------------*/

/**
  * @brief  Initialize Little FS
  * @param  None.
  * @retval None.
  */
int LittleFS_init(void)
{
    W25xxx_Init();

    // mount the filesystem
    int err = lfs_mount(&lfs, &cfg);
    
    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&lfs, &cfg);
        err = lfs_mount(&lfs, &cfg);
    }
    return err;
}

int LittleFS_Format(void)
{
    lfs_format(&lfs, &cfg);
}

int LittleFS_Mount(void)
{
    lfs_mount(&lfs, &cfg);
}
