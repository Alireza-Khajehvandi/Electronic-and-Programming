
#include "main.h"
#include "AKH_Defines.h"
#include "AKH_Font.h"
#include "AKH_SSD1963.h"

//****************************************************************************//
//--------------------------------- Functions --------------------------------//
//****************************************************************************//

void reset (void) // Hardware reseting
{
	RST_HIGH;			
	HAL_Delay(2);
	RST_LOW;
	HAL_Delay(2);
	RST_HIGH;
	HAL_Delay(2);
	CS_HIGH;
    RS_HIGH;
    RD_HIGH;
    WR_HIGH;
}
//************************** Send Command to Driver **************************//
void write_command (uint8_t cmd) 
{
	CS_LOW; 
	RS_LOW; 
	RD_HIGH; 
	WR_HIGH;
    DATA_HIGH(cmd);		// 8 pins for data set with cmd value are going to high 
	WR_LOW;
	WR_HIGH;
    RS_HIGH;
    CS_HIGH;
	DATA_LOW(cmd);		// 8 pins for data set with cmd value are going to low 
}
//**************************** Send Data to Driver ***************************//
void write_data (uint8_t data) 
{
	CS_LOW; 
	RS_HIGH; 
	RD_HIGH; 
	WR_HIGH;
    DATA_HIGH(data)
	WR_LOW;
	WR_HIGH;
    CS_HIGH;
	DATA_LOW(data);
}
//**************************** DATA Format ***************************//
void DATA_HIGH_Color(uint16_t data)
{
	uint8_t data_L;
	uint16_t data_H_10_15, data_H_0_1;
	uint16_t data_H;
	data_L = data & 0x00FF;
	data_H_10_15 = data & 0xFC00;
	data_H_0_1 = data & 0x0003;
	data_H = data_H_10_15 | data_H_0_1;
	HAL_GPIO_WritePin(GPIOA, data_L, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, data_H, GPIO_PIN_SET);
}
//**************************** DATA Format ***************************//
void DATA_LOW_Color(uint16_t data)
{
	uint8_t data_L;
	uint16_t data_H_10_15, data_H_0_1;
	uint16_t data_H;
	data_L = data & 0x00FF;
	data_H_10_15 = data & 0xFC00;
	data_H_0_1 = data & 0x0003;
	data_H = data_H_10_15 | data_H_0_1;
	HAL_GPIO_WritePin(GPIOA, data_L, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, data_H, GPIO_PIN_RESET);
}
//**************************** Send Data Color 565 ***************************//
void write_data_color (uint16_t data) 
{
	CS_LOW; 
	RS_HIGH; 
	RD_HIGH; 
	WR_HIGH;
    DATA_HIGH_Color(data);
	WR_LOW;
	WR_HIGH;
    CS_HIGH;
	DATA_LOW_Color(data);
}
//****************************** turn on display ******************************//
void display_on(void)
{
	write_command(0x29);   //send 0x29 as command to turn off the display
}
//****************************** turn off display ******************************//
void display_off(void)
{
	write_command(0x28);	// send 0x28 as command to turn off the display
}

// ********************************************************************************* //
// **************************** akh_put_char *************************************** //
// ********************************************************************************* //
void lcd_init(void)
{
	reset();
	HAL_Delay(10);
    // Set PLL MN ***************************************************************
    // @Parameters : 3
//    write_command(0xE2);    // set_pll_mn
//    write_data(0x23);       // N[7:0] : Multiplier (N) of PLL. (POR = 00101101) b00100011
//    write_data(0x02);       // M[3:0] : Divider (M) of PLL. (POR = 0011)
//    write_data(0x04);       // C[2] : Effectuate MN value (POR = 100) - Effectuate the multiplier and divider value
//    //writeData(0x54);
	// pll = 500MHz
	write_command(0xE2);    
    write_data(0x95);       
    write_data(0x05);       
    write_data(0x04);       
    //write_data(0x54);
    //----------------------------------------------------------------------------
    
	// Set PLL ***************************************************************
    // @Parameters : 1
    write_command(0xE0);
    write_data(0x01);             // Use reference clock as system clock & Enable PLL
    HAL_Delay(1);//wait_us(100);  // Wait 100us to let the PLL stable
    write_command(0xE0);           // Set PLL
    write_data(0x03);              // Use PLL output as system clock & Enable PLL
    HAL_Delay(1);//wait_us(100); 
    //----------------------------------------------------------------------------
	
	// Software Reset ***************************************************************
    write_command(0x01);
    HAL_Delay(1);//wait_us(100);   
	//----------------------------------------------------------------------------

    // Set LSHIFT Frequency ***************************************************************
    // @Parameters : 3
    write_command(0xE6);             // Set LSHIFT Frequency
    write_data(0x00);             // LCDC_FPR[19:16] : The highest 4 bits for the pixel clock frequency settings
    write_data(0xD9);             // LCDC_FPR[15:8]  : The higher byte for the pixel clock frequency settings
    write_data(0x16);             // LCDC_FPR[7:0]   : The low byte for the pixel clock frequency settings
	//----------------------------------------------------------------------------
	
	
    // Set LCD Mode	***************************************************************
    // @Parameters : 7
    write_command(0xB0);
    write_data(0x10);                // set 18-bit for 7" panel TY700TFT800480
    write_data(0x80);//write_data(0x00);                // set TTL mode
    write_data((DISP_HOR_RESOLUTION-1)>>8); //Set panel size
    write_data((uint8_t)DISP_HOR_RESOLUTION-1);
    write_data((DISP_VER_RESOLUTION-1)>>8);
    write_data((uint8_t)DISP_VER_RESOLUTION-1);
    write_data(0x00);
    //----------------------------------------------------------------------------
    
    // Set Horizontal Period ***************************************************************
    // @Parameters : 8
    write_command(0xB4);
    #define HT (DISP_HOR_RESOLUTION+DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH+DISP_HOR_FRONT_PORCH)
    write_data((HT-1)>>8);   
    write_data((uint8_t)HT-1);
    #define HPS (DISP_HOR_PULSE_WIDTH+DISP_HOR_BACK_PORCH)
    write_data((HPS-1)>>8);
    write_data(HPS-1);
    write_data(DISP_HOR_PULSE_WIDTH-1);
    write_data(0x00);
    write_data(0x00);
    write_data(0x00);
    //----------------------------------------------------------------------------
	
    // Set Vertical Period ***************************************************************
    // @Parameters : 7
    write_command(0xB6);
    #define VT (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH+DISP_VER_FRONT_PORCH+DISP_VER_RESOLUTION)
    write_data((VT-1)>>8);
    write_data((uint8_t)VT-1);
    #define VSP (DISP_VER_PULSE_WIDTH+DISP_VER_BACK_PORCH)
    write_data((VSP-1)>>8);
    write_data(VSP-1);
    write_data(DISP_VER_PULSE_WIDTH-1);
    write_data(0x00);
    write_data(0x00);
    //----------------------------------------------------------------------------
	
    //Set pixel format, i.e. the bpp
    write_command(0x3A);
    write_data(0x55);                // set 16bpp

	// from side to another side ***************************************************************
	// from bottom to top
    write_command(0x36);  // Set the read order from frame buffer to the display panel
    write_data(0x09);                // set 16bpp
	//----------------------------------------------------------------------------

    // Set Pixel Data Interface -----------------------------------------------
    // @Parameters : 1
    write_command(0xF0);             // A[2:0] : Pixel Data Interface Format
    //write_data(0x00);         // 8-bits (sendig for 3 times)    
	write_data(0x03);           // 16-bit (565 format) 
    HAL_Delay(1);//wait_us(100);
    
    // set_display_on ***************************************************************
	display_on();	// Show the image on the display device
	//----------------------------------------------------------------------------
}
// ********************************************************************************* //
// **************************** akh_fill_screen ************************************ //
// ********************************************************************************* //
void akh_fill_screen(uint32_t color){
    
    uint16_t start_x = 0;
    uint16_t end_x   = DISP_HOR_RESOLUTION - 1;
    uint16_t start_y = 0;
    uint16_t end_y   = DISP_VER_RESOLUTION - 1;
	uint32_t RGB565;
//	int red      = color >> 16;
//	int green    = color >> 8;
//    int blue     = color;
	
	RGB565 = (((color&0xf80000)>>8) | ((color&0xfc00)>>5) | ((color&0xf8)>>3));
    write_command(0x2A); //Set the column address
    //nCS_LOW;
    write_data(start_x>>8);
    write_data(start_x);
    write_data(end_x>>8);
    write_data(end_x);
    //nCS_HIGH;
    write_command(0x2B); //Set the page address
    //nCS_LOW;
    write_data(start_y>>8);
    write_data(start_y);
    write_data(end_y>>8);
    write_data(end_y);
    //nCS_HIGH;
	
	// send command and data for showing on LCD
    write_command(0x2C); //write_memory_start
    uint16_t i,j;
    for(i=0;i<DISP_HOR_RESOLUTION;i++){
        for (j=0;j<DISP_VER_RESOLUTION;j++){
			//8bit----------------------
//            write_data(red);
//			write_data(green);
//			write_data(blue);
			//16-bits (565)----------------------
			write_data_color(RGB565);
        }
    }
}


// ********************************************************************************* //
// **************************** akh_fill_area ************************************** //
// ********************************************************************************* //
void akh_fill_area(akh_struct_fill_area *obj)
{
    
//    uint16_t start_x = start_x_area;
//    uint16_t end_x   = end_x_area;
//    uint16_t start_y = start_y_area;
//    uint16_t end_y   = end_y_area;
    //for 8-bits
//	uint8_t red      = color >> 16;
//	uint8_t green    = color >> 8;
//    uint8_t blue     = color;
	// for 16-pins
	uint32_t RGB565;
	RGB565 = (((obj->color&0xf80000)>>8) | ((obj->color&0xfc00)>>5) | ((obj->color&0xf8)>>3));
    write_command(0x2A);
    write_data(obj->x_start>>8);
    write_data(obj->x_start);
    write_data(obj->x_end>>8);
    write_data(obj->x_end);
    write_command(0x2B);
    write_data(obj->y_start>>8);
    write_data(obj->y_start);
    write_data(obj->y_end>>8);
    write_data(obj->y_end);
	
    write_command(0x2C);

    uint16_t i,j;
    for(i = obj->x_start; i <= obj->x_end; i++){
        for (j=obj->y_start; j <= obj->y_end; j++){
			//8bit
//            write_data(red);
//			write_data(green);
//			write_data(blue);
			//565
			write_data_color(RGB565);
        }
    }
}

// ********************************************************************************* //
// **************************** akh_fill_area ************************************** //
// ********************************************************************************* //
void akh_fill_area_internal(
				uint32_t color,
				uint16_t start_x_area, uint16_t end_x_area,
				uint16_t start_y_area, uint16_t end_y_area
			  )
{
//	uint8_t red      = color >> 16;
//	uint8_t green    = color >> 8;
//    uint8_t blue     = color;
	
	uint32_t RGB565;
	RGB565 = (((color&0xf80000)>>8) | ((color&0xfc00)>>5) | ((color&0xf8)>>3));
    write_command(0x2A);
    write_data(start_x_area>>8);
    write_data(start_x_area);
    write_data(end_x_area>>8);
    write_data(end_x_area);
    write_command(0x2B);
    write_data(start_y_area>>8);
    write_data(start_y_area);
    write_data(end_y_area>>8);
    write_data(end_y_area);
	
    write_command(0x2C);

    uint16_t i,j;
    for(i = start_x_area; i <= end_x_area; i++){
        for (j=start_y_area;j<=end_y_area;j++){
			//8bit
//            write_data(red);
//			write_data(green);
//			write_data(blue);
			//565
			write_data_color(RGB565);
        }
    }
}


// ********************************************************************************* //
// **************************** akh_set_pixel ************************************** //
// ********************************************************************************* //
void akh_set_pixel (uint16_t x, uint16_t y, uint32_t color)
{
	// for 8-bits
//	uint8_t red      = color >> 16;
//	uint8_t green    = color >> 8;
//    uint8_t blue     = color;
	
	
	uint32_t RGB565;
	RGB565 = (((color&0xf80000)>>8) | ((color&0xfc00)>>5) | ((color&0xf8)>>3));
	
	write_command(0x2A);
    write_data(x>>8);
    write_data(x);
    write_data(x>>8);
    write_data(x);
    write_command(0x2B);
    write_data(y>>8);
    write_data(y);
    write_data(y>>8);
    write_data(y);
	write_command(0x2C);
	//8bit 24
//	write_data(red);
//	write_data(green);
//	write_data(blue);
	write_data_color(RGB565);
	
}
// ********************************************************************************* //
// **************************** font_info ****************************************** //
// ********************************************************************************* //
// get width and height and first char and byte per char and char number and number of char (all og charachters)
// get all of them and save in output as array and use it in akh_put_char information
uint16_t * font_info (uint8_t *font, uint8_t ch)
{
    //uint8_t ch = '#';
    //uint8_t *font = times_new_roman_bold;
    //uint16_t x = 0x10;
    uint8_t font_height;
    uint8_t font_width;
    uint8_t first_char;
    uint8_t num_of_char;
    uint8_t char_number;
    uint8_t char_number_0 = 4;
    uint8_t byte_per_char = 0;
    uint16_t go_byte = 0;
    font_height = font[1];
    first_char  = font[2];
    num_of_char = font[3];
    uint16_t static information[4]; /**< height, width, first,  byte per char*/

    for (int i = first_char; i < ch; i++)
    {
        char_number = (i - first_char) + 4;
        font_width  = font[char_number];
        if (font_width > 24)
        {
            go_byte = go_byte + (font_height*4); // any row = 2bytes => 19*2
        }
        else if (font_width > 16)
        {
            go_byte = go_byte + (font_height*3); // any row = 2bytes => 19*2
        }
        else if (font_width > 8)
        {
            go_byte = go_byte + (font_height*2); // any row = 2bytes => 19*2
        }
        else
        {
            go_byte = go_byte + font_height; // any row = 1bytes => 19
        }

    }
    char_number = (ch - first_char) + 4;
    font_width  = font[char_number];
    if (font_width > 24)
    {
        byte_per_char = (font_height*4); // any row = 2bytes => 19*2
    }
    else if (font_width > 16)
    {
        byte_per_char = (font_height*3); // any row = 2bytes => 19*2
    }
    else if (font_width > 8)
    {
        byte_per_char = (font_height*2); // any row = 2bytes => 19*2
    }
    else
    {
        byte_per_char = font_height; // any row = 1bytes => 19
    }

    go_byte = go_byte+100;

    information[0] = font_height;   // height
    information[1] = font_width;    // width
    information[2] = go_byte;       // first bytes for processing
    information[3] = byte_per_char; // number of bytes for char choosen



    return information;
    /*printf("font_height = %d\n", font_height);
    printf("font_width  = %d\n", font_width);
    printf("first_char  = %d\n", first_char);
    printf("char_number = %d\n", char_number);
    printf("num_of_char  = %d\n", num_of_char);
    printf("go_byte  = %d\n", go_byte);
    printf("byte_per_char  = %d\n", byte_per_char);*/
}

// ********************************************************************************* //
// **************************** akh_put_char *************************************** //
// ********************************************************************************* //
void akh_put_char(uint8_t *font, uint8_t ch, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk)
{
    uint16_t *char_info;
    uint16_t char_height, char_width, char_first_byte, char_num_bytes;
    char_info = font_info (font, ch);
    char_height     = char_info[0];
    char_width      = char_info[1];
    char_first_byte = char_info[2];
    char_num_bytes  = char_info[3];
    uint32_t row_bytes[char_height];
	
    for (int r = 0; r < char_height; r++)
    {
		if (char_width > 24)
        {
            //row_bytes[r] = (font[char_first_byte+(2*r)] << 8) + font[char_first_byte+(2*r+1)];
            row_bytes[r] = (font[char_first_byte+(4*r+3)] << 24) + (font[char_first_byte+(4*r+2)] << 16) + (font[char_first_byte+(4*r+1)] << 8) + font[char_first_byte+(4*r)];
            for(int c = 0; c < char_width; c++)
            {

                if ((row_bytes[r] >> c) & 0x0001)
                {
					//akh_set_pixel (c+x, r+y, color);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, color);// bottom to top
                }
                else
                {
					//akh_set_pixel (c+x, r+y, colorbk);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, colorbk);// bottom to top
                }
            }
        }
        else if (char_width > 16)
        {
            //row_bytes[r] = (font[char_first_byte+(2*r)] << 8) + font[char_first_byte+(2*r+1)];
            row_bytes[r] = (font[char_first_byte+(3*r+2)] << 16) + (font[char_first_byte+(3*r+1)] << 8) + font[char_first_byte+(3*r)];
            for(int c = 0; c < char_width; c++)
            {
                if ((row_bytes[r] >> c) & 0x0001)
                {

					//akh_set_pixel (c+x, r+y, color);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, color);// bottom to top
                }
                else
                {
					//akh_set_pixel (c+x, r+y, colorbk);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, colorbk);// bottom to top
                }
            }
        }
        else if (char_width > 8)
        {
            //row_bytes[r] = (font[char_first_byte+(2*r)] << 8) + font[char_first_byte+(2*r+1)];
            row_bytes[r] = (font[char_first_byte+(2*r+1)] << 8) + font[char_first_byte+(2*r)];
            for(int c = 0; c < char_width; c++)
            {

                if ((row_bytes[r] >> c) & 0x0001)
                {
					//akh_set_pixel (c+x, r+y, color);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, color);// bottom to top
                }
                else
                {
					//akh_set_pixel (c+x, r+y, colorbk);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, colorbk);// bottom to top
                }
            }
        }
        else
        {
            row_bytes[r] = font[char_first_byte+r];
            for(int c = 0; c < char_width; c++)
            {
                if ((row_bytes[r] >> c) & 0x0001)
                {
					//akh_set_pixel (c+x, r+y, color);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, color);// bottom to top
                }
                else
                {
					//akh_set_pixel (c+x, r+y, colorbk);//top to bottom
					akh_set_pixel (c+x, char_height-r+y, colorbk);// bottom to top
                }
            }
        }
		////////////////////////////////////////////////////////////////////////////////////////////
    }
}

// ********************************************************************************* //
// **************************** akh_put_string ************************************* //
// ********************************************************************************* //
void akh_put_string (uint8_t *font, char *str, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk)
{
	uint8_t ch = 0;
	uint8_t first_char;
	uint8_t font_width;
	uint8_t font_height;
	uint8_t char_number;
	font_height = font[1];
	first_char  = font[2];
	char_number = (ch - first_char) + 4;
    font_width  = font[char_number];
	
	while (*(str+ch) != '\0')
	{
		char_number = (*(str+ch) - first_char) + 4;
		font_width  = font[char_number];
		akh_put_char(font, *(str+ch), x, y, color, colorbk);
		x = x + font_width;
		ch++;
		
	}
}
// ********************************************************************************* //
// **************************** akh_put_image ************************************** //
// ********************************************************************************* //
void akh_put_image(uint16_t x, uint16_t y, uint8_t *image)
{
	uint16_t first_x, first_y;
    uint16_t row;
	uint16_t column;
	uint32_t pixel;
	row = image[0];
	column = image[1];
	first_x = x;
	first_y = y;
	pixel = ((image[2]) << 16) + ((image[3]) << 8) + (image[4]);
	akh_set_pixel(x, y, pixel);
	
	
	for(int i = 0; i < row*column; i++)
	{
		pixel = ((image[3*i+2]) << 16) + ((image[3*i+3]) << 8) + (image[3*i+4]);
		akh_set_pixel(x, y, pixel);
		x++;
		if (x == (first_x + column))
		{
			x = first_x;
			y++;
		}
	}
		
			
}
// ********************************************************************************* //
// **************************** akh_point_cal ************************************** //
// ********************************************************************************* //
void akh_point_cal(uint16_t x, uint16_t y, uint32_t color)
{
	akh_set_pixel(x+1, y-1, color);
	akh_set_pixel(x-1, y-1, color);
	akh_set_pixel(x  , y  , color);
	akh_set_pixel(x-1, y+1, color);
	akh_set_pixel(x+1, y+1, color);
}
// ********************************************************************************* //
// **************************** akh_my_font **************************************** //
// ********************************************************************************* //
void akh_my_font (unsigned char *font, uint8_t ch, uint16_t x, uint16_t y, uint32_t color, uint32_t colorbk)
{
	uint8_t font_width;
	uint8_t font_height       = font[1];
	uint8_t first_char        = font[2];
	uint8_t number_characters = font[3];
	uint8_t num_of_byte_per_ch;
	uint16_t first_of_row, row;
	
	uint8_t character_number;
	character_number = ch - first_char;
	font_width = font[character_number + 4]; // 4 -> number of headers
	
	num_of_byte_per_ch	 	  = (((uint8_t)(font_width/8)) + 1) * 19; // each row contains number of width pixels
	
	if (font_width > 8)
	{
		first_of_row  = (font[(character_number + 100)] << 8) + font[(character_number + 101)];
	}
	else 
	{
		first_of_row  = font[(character_number + 100)];
	}
//	first_of_row  = character_number + 100; // first row of character
//	first_of_row_ = ((uint8_t)(font_width/8)) * (first_of_row + 1); // first row of character
	for (int r = 0; r < font_height; r++)
	{
		row = first_of_row;
		for (int c = 0; c < font_width; c++)
		{
			if ((first_of_row >> c) & 0x0001)
				akh_set_pixel (c + x, r + y, color);
		}
	}
}

// ********************************************************************************* //
// **************************** akh_my_font **************************************** //
// ********************************************************************************* //
void akh_button(akh_struct_button *obj)
{
	//button 
	
	akh_fill_area_internal(obj->btnColor, obj->x + obj->border + obj->edge, 
				(obj->x + obj->width) - obj->border - obj->edge, obj->y + obj->border + obj->edge, (obj->y + obj->height) - obj->border - obj->edge);
	akh_put_string(obj->font, obj->text, 
				obj->x + obj->border + obj->edge + obj->x_txt, 
				obj->y + obj->border + obj->edge + obj->y_txt, obj->textColor, obj->btnColor);
	// border						(obj->y + obj->height)
	akh_fill_area_internal(obj->borderColor, obj->x + obj->edge, (obj->x + obj->width) - obj->edge, obj->y + obj->edge, obj->y + obj->edge + obj->border - 1);
	akh_fill_area_internal(obj->borderColor, obj->x + obj->edge, (obj->x + obj->width) -obj->edge, (obj->y + obj->height) - obj->edge - obj->border + 1, (obj->y + obj->height) - obj->edge);
	akh_fill_area_internal(obj->borderColor, obj->x + obj->edge, obj->x +obj->edge + obj->border - 1, obj->y + obj->edge, (obj->y + obj->height) - obj->edge);
	akh_fill_area_internal(obj->borderColor, (obj->x + obj->width) - obj->edge - obj->border+1, (obj->x + obj->width) - obj->edge, obj->y + obj->edge, (obj->y + obj->height) - obj->edge);
	//edge
	akh_fill_area_internal(obj->edgeColor, obj->x, (obj->x + obj->width), obj->y, obj->y + obj->edge - 1);
	akh_fill_area_internal(obj->edgeColor, obj->x, (obj->x + obj->width), (obj->y + obj->height) - obj->edge + 1, (obj->y + obj->height));
	akh_fill_area_internal(obj->edgeColor, obj->x, obj->x + obj->edge - 1, obj->y, (obj->y + obj->height));
	akh_fill_area_internal(obj->edgeColor, (obj->x + obj->width) - obj->edge + 1, (obj->x + obj->width), obj->y, (obj->y + obj->height));
}


