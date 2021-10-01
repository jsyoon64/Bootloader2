#ifndef _SHARED_MEM_
#define _SHARED_MEM_
/**
  ******************************************************************************
  * @file    shared_mem.h
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
/* Scheduler include files. */
/* Application include files. */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define SHARED_API_SECTION __attribute__((section(".apishared_section")))
#define SHARED_BOOT_RAM __attribute__((section(".shared_ram")))
/* Exported functions ------------------------------------------------------- */
void sharedmem_init(void);
/*============================================================================*/
#endif  /* _SHARED_MEM_*/
