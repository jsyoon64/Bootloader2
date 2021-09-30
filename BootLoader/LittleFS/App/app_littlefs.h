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

/* Exported types ------------------------------------------------------------*/
extern lfs_t lfs;
extern struct lfs_info dir_info;
extern const struct lfs_config cfg;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
int LittleFS_init(void);
int LittleFS_Format(void);
int LittleFS_Mount(void);

#endif /* _APP_LITTLEFS_H_ */

