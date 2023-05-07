/* vim: set ai et ts=4 sw=4: */

#include "stm32f4xx_hal.h"
#include "AKH_XPT2046_TouchScreen.h"

#define READ_X 0xD0
#define READ_Y 0x90
#define avg_cnt 16

static void akh_touchSelect() {
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_RESET);
}

void akh_touchUnselect() {
    HAL_GPIO_WritePin(TOUCH_CS_GPIO_Port, TOUCH_CS_Pin, GPIO_PIN_SET);
}

bool akh_touchPressed() {
    return HAL_GPIO_ReadPin(TOUCH_IRQ_GPIO_Port, TOUCH_IRQ_Pin) == GPIO_PIN_RESET;
}

bool akh_touchGetCoordinates(uint16_t* x, uint16_t* y, uint16_t* rx, uint16_t* ry) {
    static const uint8_t cmd_read_x[] = { READ_X };
    static const uint8_t cmd_read_y[] = { READ_Y };
    static const uint8_t zeroes_tx[] = { 0x00, 0x00 };
    akh_touchSelect();

    uint32_t avg_x = 0;
    uint32_t avg_y = 0;
	uint32_t xph = 0;
    uint32_t yph = 0;
    uint16_t nsamples = 0;
//    for(uint8_t i = 0; i < 100; i++) {
	for(uint16_t i = avg_cnt; i > 0; i--) {
        if(!akh_touchPressed())
            break;

        nsamples++;
		
        HAL_SPI_Transmit(&TOUCH_SPI_PORT, (uint8_t*)cmd_read_y, sizeof(cmd_read_y), HAL_MAX_DELAY);
		uint8_t y_raw[2];
        HAL_SPI_TransmitReceive(&TOUCH_SPI_PORT, (uint8_t*)zeroes_tx, y_raw, sizeof(y_raw), HAL_MAX_DELAY);

        HAL_SPI_Transmit(&TOUCH_SPI_PORT, (uint8_t*)cmd_read_x, sizeof(cmd_read_x), HAL_MAX_DELAY);
		uint8_t x_raw[2];
        HAL_SPI_TransmitReceive(&TOUCH_SPI_PORT, (uint8_t*)zeroes_tx, x_raw, sizeof(x_raw), HAL_MAX_DELAY);
		
		xph = (((uint16_t)x_raw[0]) << 8) | ((uint16_t)x_raw[1]);
		yph = (((uint16_t)y_raw[0]) << 8) | ((uint16_t)y_raw[1]);
		
        avg_x += xph;
        avg_y += yph;

    }
    akh_touchUnselect();

    if(nsamples < avg_cnt)
        return false;

    uint32_t raw_x = (avg_x / avg_cnt);
	*ry = raw_x;
//    if(raw_x < TOUCH_MIN_RAW_X) raw_x = TOUCH_MIN_RAW_X;
//    if(raw_x > TOUCH_MAX_RAW_X) raw_x = TOUCH_MAX_RAW_X;

    uint32_t raw_y = (avg_y / avg_cnt);
	*rx = raw_y;

    // Uncomment this line to calibrate touchscreen:
    // UART_Printf("raw_x = %d, raw_y = %d\r\n", x, y);
	*x = (*rx * 0.0255167682) - (*ry * 0.0000430662) - (15.0175510355);
	*y = (*ry * 0.0167636298) - (*rx * 0.0002382754) - (19.3701631453);
    return true;
}