/**
  ******************************************************************************
  * @file    app_littlefs.h
  * @author  jsyoon
  * @brief   
  ******************************************************************************
  */

#ifndef _APP_LITTLEFS_H_
#define _APP_LITTLEFS_H_

/* Includes ------------------------------------------------------------------*/
#include "Core/lfs.h"
#include "shared_mem.h"

/* Exported types ------------------------------------------------------------*/
extern lfs_t lfs;
extern struct lfs_info dir_info;
extern const struct lfs_config cfg;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int LittleFS_init(void);
int SHARED_API_SECTION LittleFS_Format(void);
int SHARED_API_SECTION LittleFS_Mount(void);
int SHARED_API_SECTION LittleFS_Open(lfs_file_t *file, const char *path, int flags);
int SHARED_API_SECTION LittleFS_Read(lfs_file_t *file, void *buffer, uint32_t size);
int SHARED_API_SECTION LittleFS_Write(lfs_file_t *file, const void *buffer, uint32_t size);
int SHARED_API_SECTION LittleFS_Close(lfs_file_t *file);
int SHARED_API_SECTION LittleFS_Seek(lfs_file_t *file, int32_t off, int whence);
#endif /* _APP_LITTLEFS_H_ */

