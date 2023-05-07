#include "main.h"
// ================================= Defines =============================== //
/*********************************************************************
* Overview: Image orientation (can be 0, 90, 180, 270 degrees).
*********************************************************************/  
#define DISP_ORIENTATION    0
/*********************************************************************
* Overview: Panel Data Width (R,G,B) in (6,6,6)
*********************************************************************/
#define DISP_DATA_WIDTH                 18
/*********************************************************************
* Overview: Horizontal and vertical display resolution
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_RESOLUTION 800
#define DISP_VER_RESOLUTION 480
/*********************************************************************
* Overview: Horizontal synchronization timing in pixels
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_HOR_PULSE_WIDTH        1
#define DISP_HOR_BACK_PORCH         210
#define DISP_HOR_FRONT_PORCH        45  
/*********************************************************************
* Overview: Vertical synchronization timing in lines
*                  (from the glass datasheet).
*********************************************************************/
#define DISP_VER_PULSE_WIDTH        1
#define DISP_VER_BACK_PORCH         34  
#define DISP_VER_FRONT_PORCH        10


//*********************** Control Pins were defined here ***********************//
// LED For Testing 
#define		LED_HIGH				HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_SET)
#define		LED_LOW		    		HAL_GPIO_WritePin(GPIOC, LED_Pin, GPIO_PIN_RESET)
// RS Pin that is LRS on driver 	
#define		RS_HIGH					HAL_GPIO_WritePin(GPIOC, LRS_Pin, GPIO_PIN_SET)
#define		RS_LOW					HAL_GPIO_WritePin(GPIOC, LRS_Pin, GPIO_PIN_RESET)
// CS Pin that is LCS on driver 	
#define		CS_HIGH					HAL_GPIO_WritePin(GPIOC, LCS_Pin, GPIO_PIN_SET)
#define		CS_LOW					HAL_GPIO_WritePin(GPIOC, LCS_Pin, GPIO_PIN_RESET)
// RD Pin that is LRD on driver 	
#define		RD_HIGH					HAL_GPIO_WritePin(GPIOC, LRD_Pin, GPIO_PIN_SET)
#define		RD_LOW					HAL_GPIO_WritePin(GPIOC, LRD_Pin, GPIO_PIN_RESET)
// WR Pin that is LWR on driver 
#define		WR_HIGH					HAL_GPIO_WritePin(GPIOC, LWR_Pin, GPIO_PIN_SET)
#define		WR_LOW					HAL_GPIO_WritePin(GPIOC, LWR_Pin, GPIO_PIN_RESET)
// RST Pin that is RST on driver 	
#define		RST_HIGH				HAL_GPIO_WritePin(GPIOA, RST_Pin, GPIO_PIN_SET)
#define		RST_LOW					HAL_GPIO_WritePin(GPIOA, RST_Pin, GPIO_PIN_RESET)
// Data Pins that are D0-D7 in 8 pins mode and D0-D15 in 16 pins mode 	(here 8 pins)
#define     DATA_HIGH(data)			HAL_GPIO_WritePin(GPIOA, data, GPIO_PIN_SET);
#define     DATA_LOW(data)			HAL_GPIO_WritePin(GPIOA, data, GPIO_PIN_RESET);
 




