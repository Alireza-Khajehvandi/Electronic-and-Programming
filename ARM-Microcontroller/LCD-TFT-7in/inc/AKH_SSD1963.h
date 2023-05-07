#include "main.h"
#include "AKH_Defines.h"


// LCD Reset
void reset (void);


// send command to lcd
void write_command (uint8_t cmd);


// send 8 bits data to lcd when mode is 8 pins
void write_data (uint8_t data);


// (SET DATA in 16 bits mode)
// this function use in write_data_color function
void DATA_HIGH_Color(uint16_t data);


// (RESET DATA in 16 bits mode)
// this function use in write_data_color function
void DATA_LOW_Color(uint16_t data);


// send 16 bits data to lcd when mode is 16 pins (this is for set color of pixels not command)
void write_data_color (uint16_t data) ;


// set display on with function 
void display_on(void);


// set display off with function
void display_off(void);


// initialize lcd for use it
void lcd_init();


// setting color for one pixel 
void akh_set_pixel (uint16_t x, uint16_t y, uint32_t color);


// this is for a basic test 
void my_font (unsigned char *font, uint8_t ch, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk);


// this is for extracting information from the font 
// this function used in put_char function
uint16_t * font_info (uint8_t *font, uint8_t ch);


//*************************************************************************//
// putting one character on any place on the LCD
// inputs 
// 1 - font type
// 2 - character
// 3 - x position
// 4 - y position
// 5 - text color
// 6 - background of text color
void akh_put_char (uint8_t *font, uint8_t ch, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk);
//-------------------------------------------------------------------------//

//*************************************************************************//
// putting string on any place on the LCD
// inputs 
// 1 - font type
// 2 - string
// 3 - x position
// 4 - y position
// 5 - text color
// 6 - background of text color
void akh_put_string (uint8_t *font, char *str, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk);
//-------------------------------------------------------------------------//


// putting image on any place on the LCD (in processing)
void akh_put_image(uint16_t x, uint16_t y, uint8_t *image);


// setting the color of all pixels on the LCD (background))
void akh_fill_screen(uint32_t color);

//***************************************************************//
// first button with xy and whitout width and height (basicly)
typedef struct 
{ 
	uint16_t x_start; 
	uint16_t y_start; 
	uint16_t x_end; 
	uint16_t y_end;
	uint32_t color;
}akh_struct_fill_area;

// setting the color for the area of the LCD
void akh_fill_area(akh_struct_fill_area *obj);
//***************************************************************//

// putting a particular point on the LCD (for Calibration or any work)
void akh_point_cal(uint16_t x, uint16_t y, uint32_t color);


//***************************************************************//
void akh_fill_area_internal(
				uint32_t color,
				uint16_t start_x_area, uint16_t end_x_area,
				uint16_t start_y_area, uint16_t end_y_area
			  );

//***************************************************************//
// button with x and y position for starting and with height and width and x, y for text position
typedef struct 
{ 
	char *text;
	uint8_t *font;            // font
	uint16_t x_txt;           // x position of text from (0, 0) position of btn
	uint16_t y_txt;           // y position of text from (0, 0) position of btn
	uint16_t x;               // btn started from x
	uint16_t y;               // btn started from y
	uint16_t width;           // btn width
	uint16_t height;          // btn height
	uint16_t edge;            // btn edge
	uint16_t border;          // btn border
	uint32_t btnColor;        // btn color
	uint32_t borderColor;     // btn border color
	uint32_t edgeColor;       // btn edge color
	uint32_t textColor;       // text color on btn
}akh_struct_button;

// button function
void akh_button(akh_struct_button *obj);
//***************************************************************//





// unused
void btn_fill_area(
				uint16_t start_x_area, uint16_t start_y_area,
				uint16_t end_x_area,   uint16_t end_y_area,
				uint16_t edge, uint16_t border,
				uint32_t btnColor, uint32_t borderColor, uint32_t edgeColor
			  );
