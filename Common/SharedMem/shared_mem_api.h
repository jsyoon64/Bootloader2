#ifndef _SHARED_MEM_API_H_
#define _SHARED_MEM_API_H_
/**
  ******************************************************************************
  * @file    shared_mem_api.h
  * @author  jsyoon
  * @date    2021/09/28  
  * @brief   
  ******************************************************************************

Revision History:

Modification Tracking
Author          Date            Number          Description of Changes
--------        --------        -------         ------------------------

------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
/* Standard includes. */
#include <stdint.h>

/* Scheduler include files. */
/* Application include files. */
#include "../LittleFS/Core/lfs.h"
#include "../Srecord/srecord_parse.h"

/* Exported types ------------------------------------------------------------*/
typedef struct BootloaderSharedAPI
{
  void (*LedBlink)(void);
  uint16_t (*TurnOn)(void);
  int (*LittleFS_Format)(void);
  int (*LittleFS_Mount)(void);
  int (*LittleFS_Open)(lfs_file_t *file, const char *path, int flags);
  int (*LittleFS_Read)(lfs_file_t *file, void *buffer, uint32_t size);
  int (*LittleFS_Write)(lfs_file_t *file, const void *buffer, uint32_t size);
  int (*LittleFS_Close)(lfs_file_t *file);
  int (*LittleFS_Seek)(lfs_file_t *file, int32_t off, int whence);
  tSrecLineType (*SrecGetLineType)(const char *line);
  uint16_t (*SrecGetLineLength)(const char *line);
  sRec_ProcessStatus (*SrecVerifyChecksum)(const char *line);
} _SharedMemAPI_;

#endif  /* _SHARED_MEM_API_H_*/
