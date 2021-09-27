/**
  ******************************************************************************
  * @file    srecord_parse.h
  * @author  jsyoon
  * @brief   Header for srecord_parse.c module
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef SRECORD_PARSE_H
#define SRECORD_PARSE_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/* Exported types ------------------------------------------------------------*/
/** Maximum number of characters that can be on a line in the firmware file. */
#define MAX_CHARS_PER_LINE                  (256)

/** Maximum number of data bytes that can be on a line in the firmware file
 *         (S-record).
 */
#define MAX_DATA_BYTES_PER_LINE             (MAX_CHARS_PER_LINE/2)

/** Return code in case an invalid checksum was detected on an S-record line. */
#define ERROR_SREC_INVALID_CHECKSUM         (-1)

/** Enumeration for the different S-record line types. */
typedef enum
{
  LINE_TYPE_S1,                                  /**< 16-bit address line              */
  LINE_TYPE_S2,                                  /**< 24-bit address line              */
  LINE_TYPE_S3,                                  /**< 32-bit address line              */
  LINE_TYPE_UNSUPPORTED                          /**< unsupported line                 */
} tSrecLineType;

typedef enum
{
  SREC_SUCCESS = 0,
  SREC_ERROR = !SREC_SUCCESS
} sRec_ProcessStatus;

/** Structure type for grouping the parsing results of an S-record line. */
typedef struct
{
  char      line[MAX_CHARS_PER_LINE];            /**< string buffer for the line chars */
  uint8_t   data[MAX_DATA_BYTES_PER_LINE];       /**< array for S1, S2 or S3 data bytes*/
  uint32_t  address;                             /**< address on S1, S2 or S3 line     */
} tSrecLineParseObject;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* functions for reading data from a Motorola S-record file. */
tSrecLineType FileSrecGetLineType(const char *line);
sRec_ProcessStatus FileSrecVerifyChecksum(const char *line);
int16_t FileSrecParseLine(const char *line, uint32_t *address, uint8_t *data);

#endif /* SRECORD_PARSE_H */
