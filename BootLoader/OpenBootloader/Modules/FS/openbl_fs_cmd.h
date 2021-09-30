/**
  ******************************************************************************
  * @file    openbl_fs_cmd.h
  * @author  
  * @brief   Header for openbl_fs_cmd.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef OPENBL_FS_CMD_H
#define OPENBL_FS_CMD_H

/* Includes ------------------------------------------------------------------*/
#include "Core/openbl_core.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define OPENBL_FS_VERSION                 0x00U  /* Open Bootloader FS protocol V0.0 */

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */
OPENBL_CommandsTypeDef *OPENBL_FS_GetCommandsList(void);

#endif /* OPENBL_FS_CMD_H */