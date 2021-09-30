#ifndef _W25X40CONF_H_
#define _W25X40CONF_H_
#include "spi.h"

#define _W25XXX_SPI                   hspi2
#define _W25XXX_CS_GPIO               SPI2_CS_GPIO_Port
#define _W25XXX_CS_PIN                SPI2_CS_Pin
#define _W25XXX_USE_FREERTOS          0//1

#endif // _W25X40CONF_H_
