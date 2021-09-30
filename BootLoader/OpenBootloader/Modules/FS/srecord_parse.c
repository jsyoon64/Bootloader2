/**
  ******************************************************************************
  * @file    srecord_parse.c
  * @author  jsyoon
  * @brief   Contains Flash layout
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "srecord_parse.h"
#include <ctype.h>

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

/**
 * @brief     Helper function to convert a sequence of 2 characters that represent
 *            a hexadecimal value to the actual byte value.
 *              Example: srecHexStringToByte("2f")  --> returns 47.
 * @param     hexstring String beginning with 2 characters that represent a hexa-
 *                      decimal value.
 * @return    The resulting byte value.
 */
static uint8_t srecHexStringToByte(const char *hexstring)
{
  uint8_t result = 0;
  char  c;
  uint8_t counter;

  /* a hexadecimal character is 2 characters long (i.e 0x4F minus the 0x part) */
  for (counter=0; counter < 2; counter++)
  {
    /* read out the character */
    c = toupper((int16_t)(hexstring[counter]));
    /* check that the character is 0..9 or A..F */
    if ((c < '0') || (c > 'F') || ((c > '9') && (c < 'A')))
    {
      /* character not valid */
      return 0;
    }
    /* convert character to 4-bit value (check ASCII table for more info) */
    c -= '0';
    if (c > 9)
    {
      c -= 7;
    }
    /* add it to the result */
    result = (result << 4) + c;
  }
  /* return the results */
  return result;
} /*** end of srecHexStringToByte ***/

/**
 * @brief     Inspects a line from a Motorola S-Record file to determine its type.
 * @param     line A line from the S-Record.
 * @return    the S-Record line type.
 */
tSrecLineType SrecGetLineType(const char *line)
{
  /* check if the line starts with the 'S' character, followed by a digit */
  if ((toupper((uint16_t)(line[0])) != 'S') || (isdigit((uint16_t)(line[1])) == 0))
  {
    /* not a valid S-Record line type */
    return LINE_TYPE_UNSUPPORTED;
  }
  /* determine the line type */
  if (line[1] == '1')
  {
    return LINE_TYPE_S1;
  }
  if (line[1] == '2')
  {
    return LINE_TYPE_S2;
  }
  if (line[1] == '3')
  {
    return LINE_TYPE_S3;
  }
  if (line[1] == '7')
  {
    return LINE_TYPE_S7;
  }  
  /* still here so not a supported line type found */
  return LINE_TYPE_UNSUPPORTED;
}

uint16_t SrecGetLineLength(const char *line)
{
  /* read out the number of byte values that follow on the line */
  return srecHexStringToByte(line);
}

/**
 * @brief     Inspects an S1, S2 or S3 line from a Motorola S-Record file to
 *            determine if the checksum at the end is corrrect.
 * @param     line An S1, S2 or S3 line from the S-Record.
 * @return    TRUE if the checksum is correct, BLT_FALSE otherwise.
 */
sRec_ProcessStatus SrecVerifyChecksum(const char *line)
{
  uint16_t bytes_on_line;
  uint8_t  checksum = 0;

  /* adjust pointer to point to byte count value */
  line += 2;
  /* read out the number of byte values that follow on the line */
  bytes_on_line = srecHexStringToByte(line);
  /* byte count is part of checksum */
  checksum += bytes_on_line;
  /* adjust pointer to the first byte of the address */
  line += 2;
  /* add byte values of address and data, but not the final checksum */
  do
  {
    /* add the next byte value to the checksum */
    checksum += srecHexStringToByte(line);
    /* update counter */
    bytes_on_line--;
    /* point to next hex string in the line */
    line += 2;
  }
  while (bytes_on_line > 1);
  /* the checksum is calculated by summing up the values of the byte count, address and
   * databytes and then taking the 1-complement of the sum's least signigicant byte */
  checksum = ~checksum;
  /* finally verify the calculated checksum with the one at the end of the line */
  if (checksum != srecHexStringToByte(line))
  {
    /* checksum incorrect */
    return SREC_ERROR;
  }
  /* still here so the checksum was correct */
  return SREC_SUCCESS;
}

/**
 * @brief     Parses a line from a Motorola S-Record file and looks for S1, S2 or S3
 *            lines with data. Note that if a null pointer is passed as the data
 *            parameter, then no data is extracted from the line.
 * @param     line    A line from the S-Record.
 * @param     address Address found in the S-Record data line.
 * @param     data    Byte array where the data bytes from the S-Record data line
 *                    are stored.
 * @return    The number of data bytes found on the S-record data line, 0 in case
 *            the line is not an S1, S2 or S3 line or ERROR_SREC_INVALID_CHECKSUM
 *            in case the checksum validation failed.
 */
//int16_t SrecParseLine(const char *line, uint32_t *address, uint8_t *data)
int16_t SrecParseLine(tSrecLineParseObject *lineObj)
{
  tSrecLineType lineType;
  int16_t   data_byte_count = 0;
  uint16_t  bytes_on_line;
  uint16_t  i;

  uint32_t *address = &lineObj->address;
  uint8_t *data = lineObj->data;
  char *line = lineObj->line;

  /* check pointers and not that data can be a null pointer */
  ASSERT(line != NULL);
  /* figure out what type of line we are dealing with */
  lineType = SrecGetLineType(line);
  /* make sure it is one that we can parse */
  if ((lineType == LINE_TYPE_UNSUPPORTED) || (lineType == LINE_TYPE_S7))
  {
    /* not a parsing error, but simply no data on this line */
    return 0;
  }
  /* verify the checksum */
  if (SrecVerifyChecksum(line) != SREC_SUCCESS)
  {
    /* error on data line encountered */
    return ERROR_SREC_INVALID_CHECKSUM;
  }
  /* all good so far, now read out the address and databytes for the line */
  switch (lineType)
  {
    /* ---------------------------- S1 line type ------------------------------------- */
    case LINE_TYPE_S1:
      /* adjust pointer to point to byte count value */
      line += 2;
      /* read out the number of byte values that follow on the line */
      bytes_on_line = srecHexStringToByte(line);
      /* read out the 16-bit address */
      line += 2;
      *address = srecHexStringToByte(line) << 8;
      line += 2;
      *address += srecHexStringToByte(line);
      /* adjust pointer to point to the first data byte after the address */
      line += 2;
      /* determine how many data bytes are on the line */
      data_byte_count = bytes_on_line - 3; /* -2 bytes address, -1 byte checksum */
      /* read and store data bytes if requested */
      if (data != NULL)
      {
        for (i=0; i<data_byte_count; i++)
        {
          data[i] = srecHexStringToByte(line);
          line += 2;
        }
      }
      break;

    /* ---------------------------- S2 line type ------------------------------------- */
    case LINE_TYPE_S2:
      /* adjust pointer to point to byte count value */
      line += 2;
      /* read out the number of byte values that follow on the line */
      bytes_on_line = srecHexStringToByte(line);
      /* read out the 32-bit address */
      line += 2;
      *address = srecHexStringToByte(line) << 16;
      line += 2;
      *address += srecHexStringToByte(line) << 8;
      line += 2;
      *address += srecHexStringToByte(line);
      /* adjust pointer to point to the first data byte after the address */
      line += 2;
      /* determine how many data bytes are on the line */
      data_byte_count = bytes_on_line - 4; /* -3 bytes address, -1 byte checksum */
      /* read and store data bytes if requested */
      if (data != NULL)
      {
        for (i=0; i<data_byte_count; i++)
        {
          data[i] = srecHexStringToByte(line);
          line += 2;
        }
      }
      break;

    /* ---------------------------- S3 line type ------------------------------------- */
    case LINE_TYPE_S3:
      /* adjust pointer to point to byte count value */
      line += 2;
      /* read out the number of byte values that follow on the line */
      bytes_on_line = srecHexStringToByte(line);
      /* read out the 32-bit address */
      line += 2;
      *address = srecHexStringToByte(line) << 24;
      line += 2;
      *address += srecHexStringToByte(line) << 16;
      line += 2;
      *address += srecHexStringToByte(line) << 8;
      line += 2;
      *address += srecHexStringToByte(line);
      /* adjust pointer to point to the first data byte after the address */
      line += 2;
      /* determine how many data bytes are on the line */
      data_byte_count = bytes_on_line - 5; /* -4 bytes address, -1 byte checksum */
      /* read and store data bytes if requested */
      if (data != NULL)
      {
        for (i=0; i<data_byte_count; i++)
        {
          data[i] = srecHexStringToByte(line);
          line += 2;
        }
      }
      break;

    default:
      break;
  }

  return data_byte_count;
} /*** end of SrecParseLine ***/
