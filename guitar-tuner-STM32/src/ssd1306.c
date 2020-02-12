#include "ssd1306.h"

/**
 * Datasheet: https://cdn-shop.adafruit.com/datasheets/SSD1306.pdf
 */

/**
 * Reset the OLED screen
 */
void ssd1306_reset()
{
    // Reset OLED display
    GPIO_WriteBit(SSD1306_RESET_PORT, SSD1306_RESET, 1);
    Delay(1);
    GPIO_WriteBit(SSD1306_RESET_PORT, SSD1306_RESET, 0);
    Delay(10);
    GPIO_WriteBit(SSD1306_RESET_PORT, SSD1306_RESET, 1);
}

/**
 * Startup routine for the OLED screen. Sends commands to configure the OLED.
 * All data is sent over SPI
 * See https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.cpp
 */
void ssd1306_init()
{
    ssd1306_reset();

    ssd1306_select_chip(SELECT);

    ssd1306_command_mode();

    // Send command to turn display on
    // See https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.cpp
    // begin() function
    ssd1306_write_byte(SPI2, SSD1306_DISPLAYOFF);                                  
    ssd1306_write_byte(SPI2, SSD1306_SETDISPLAYCLOCKDIV);                                  
    ssd1306_write_byte(SPI2, 0x80);                                  
    ssd1306_write_byte(SPI2, SSD1306_SETMULTIPLEX);                                  

    ssd1306_write_byte(SPI2, 0x1F); // height - 1

    ssd1306_write_byte(SPI2, SSD1306_SETDISPLAYOFFSET);                                  
    ssd1306_write_byte(SPI2, 0x00);                      
    ssd1306_write_byte(SPI2, SSD1306_SETSTARTLINE | 0x0);                                  
    ssd1306_write_byte(SPI2, SSD1306_CHARGEPUMP);

    ssd1306_write_byte(SPI2, 0x14);

    ssd1306_write_byte(SPI2, SSD1306_MEMORYMODE);
    ssd1306_write_byte(SPI2, 0x01);
    ssd1306_write_byte(SPI2, SSD1306_SEGREMAP | 0x1);
    ssd1306_write_byte(SPI2, SSD1306_COMSCANDEC);

    ssd1306_write_byte(SPI2, SSD1306_SETCOMPINS);
    ssd1306_write_byte(SPI2, 0x02);
    ssd1306_write_byte(SPI2, SSD1306_SETCONTRAST);
    ssd1306_write_byte(SPI2, 0x47);

    ssd1306_write_byte(SPI2, SSD1306_SETPRECHARGE);
    ssd1306_write_byte(SPI2, 0xF1);

    ssd1306_write_byte(SPI2, SSD1306_SETVCOMDETECT);
    ssd1306_write_byte(SPI2, 0x40);
    ssd1306_write_byte(SPI2, SSD1306_DISPLAYALLON_RESUME);
    ssd1306_write_byte(SPI2, SSD1306_NORMALDISPLAY);
    ssd1306_write_byte(SPI2, SSD1306_DEACTIVATE_SCROLL);
    ssd1306_write_byte(SPI2, SSD1306_DISPLAYON);
}

/**
 * Drive the Chip Select GPIO pin to desired state.
 * @param state: SELECT or DESELECT
 */
void ssd1306_select_chip(chip_select state)
{
    if(state == SELECT)
    {
	GPIO_WriteBit(SSD1306_CS_PORT, SSD1306_CS, 0);
    }
    else if(state == DESELECT)
    {
	GPIO_WriteBit(SSD1306_CS_PORT, SSD1306_CS, 1);
    }
}

/**
 * Drive the Data/Command GPIO pin to enter command mode
 */
void ssd1306_command_mode()
{
    // Go into command mode
    GPIO_WriteBit(SSD1306_DC_PORT, SSD1306_DC, 0);
    Delay(10);
}

/**
 * Drive the Data/Command GPIO pin to enter data mode
 */
void ssd1306_data_mode()
{
    // Go into data mode
    GPIO_WriteBit(SSD1306_DC_PORT, SSD1306_DC, 1);
    Delay(10);
}

/**
 * Send commands to OLED screen to enable sending of bytes that will be displayed.
 * You must call this function before sending bytes to the OLED that will be displayed.
 */
void ssd1306_display_begin()
{
    ssd1306_select_chip(SELECT);
    ssd1306_command_mode();
    // See SSD1306::display function in github source code from comment above
    ssd1306_write_byte(SPI2, SSD1306_PAGEADDR);
    ssd1306_write_byte(SPI2, 0x00);
    // ssd1306_write_byte(SPI2, 0xFF);
    ssd1306_write_byte(SPI2, 0x03);
    ssd1306_write_byte(SPI2, SSD1306_COLUMNADDR);
    ssd1306_write_byte(SPI2, 0x00);
    ssd1306_write_byte(SPI2, 0x7F); // width - 1

    Delay(100);

    ssd1306_data_mode();
}

/**
 * You must call this function after writing bytes to be displayed to the OLED.
 */
void ssd1306_display_end()
{
    ssd1306_select_chip(DESELECT);
}

/**
 * Turn off all pixels on the OLED screen
 */
void ssd1306_clear_screen()
{
    ssd1306_display_begin();
    
    int i;
    for(i=0; i<NUM_BYTES; i++)
    {
        ssd1306_write_byte(SPI2, 0x00); 
    }

    ssd1306_write_byte(SPI2, 0x00); 
    ssd1306_write_byte(SPI2, 0x00); 

    ssd1306_display_end();
}

/**
 * Turn on all pixels on the OLED screen
 */
void ssd1306_fill_screen()
{
    ssd1306_display_begin();
    
    int i;
    for(i=0; i<NUM_BYTES; i++)
    {
        ssd1306_write_byte(SPI2, 0xFF); 
    }

    ssd1306_write_byte(SPI2, 0x00); 
    ssd1306_write_byte(SPI2, 0x00); 
    
    ssd1306_display_end();
}

/**
 * Write a byte to the OLED screen over SPI
 * @param SPIx: pointer to SPI peripheral driving the OLED screen
 * @param data: byte to be sent to the OLED screen
 */
void ssd1306_write_byte(SPI_TypeDef* SPIx, uint16_t data)
{
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);
    SPI_I2S_SendData(SPIx, data);
}

/**
 * Write the contents of the given 512 byte buffer to the OLED screen
 * @param buff: byte array of length 512
 */
void ssd1306_display_buffer(uint8_t* buff)
{
    ssd1306_display_begin();

    int i;
    for(i=0; i<NUM_BYTES; i++)
    {
	ssd1306_write_byte(SPI2, buff[i]);
    }
    ssd1306_write_byte(SPI2,0x00);
    ssd1306_write_byte(SPI2,0x00);

    ssd1306_display_end();
}

/**
 * Combine two 512 byte bitmaps by bitwise or-ing each of their bytes
 * This is used when combining bitmaps for estimated string and tuning instruction.
 * For example, first could be bitmap for 'A' and second could be bitmap for tune down
 * @param buff: byte array to write results too
 * @param first: byte array of 512 bytes
 * @param second: byte array of 512 bytes
 */
void ssd1306_combine_graphics(uint8_t* buff, uint8_t* first, uint8_t* second)
{
    int i;
    for(i=0; i<NUM_BYTES; i++)
    {
	buff[i] = first[i] | second[i];
    }

}
