
/* vim: set ai et ts=4 sw=4: */
#ifndef __ILI9341_TOUCH_H__
#define __ILI9341_TOUCH_H__

#include <stdbool.h>
#include <main.h>

/*** Redefine if necessary ***/

// Warning! Use SPI bus with < 1.3 Mbit speed, better ~650 Kbit to be save.
#define TOUCH_SPI_PORT hspi3
extern  SPI_HandleTypeDef    TOUCH_SPI_PORT;

#define TOUCH_IRQ_Pin       GPIO_PIN_9 // Arduino D5
#define TOUCH_IRQ_GPIO_Port GPIOA
#define TOUCH_CS_Pin        GPIO_PIN_15 // Arduino D2
#define TOUCH_CS_GPIO_Port  GPIOA

// change depending on screen orientation
#define TOUCH_SCALE_X 800
#define TOUCH_SCALE_Y 480

// to calibrate uncomment UART_Printf line in ili9341_touch.c
#define TOUCH_MIN_RAW_X 3500
#define TOUCH_MAX_RAW_X 27000
#define TOUCH_MIN_RAW_Y 2000
#define TOUCH_MAX_RAW_Y 30000

// call before initializing any SPI devices
void akh_touchUnselect();
static void akh_touchSelect();


bool akh_touchPressed();
bool akh_touchGetCoordinates(uint16_t* x, uint16_t* y, uint16_t* rx, uint16_t* ry);

#endif // __ILI9341_TOUCH_H__























