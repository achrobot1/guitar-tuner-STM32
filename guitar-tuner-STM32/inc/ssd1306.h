#ifndef SSD1306_H
#define SSD1306_H

#include "stm32f10x_spi.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

/******************************************************/

#define SSD1306_RESET		GPIO_Pin_8
#define SSD1306_DC		GPIO_Pin_0
#define SSD1306_CS		GPIO_Pin_1

#define SSD1306_RESET_PORT	GPIOA		
#define SSD1306_DC_PORT		GPIOA
#define SSD1306_CS_PORT		GPIOA

#define SSD1306_SCK		GPIO_Pin_13 
#define SSD1306_MOSI		GPIO_Pin_15

#define SSD1306_SCK_PORT	GPIOB
#define SSD1306_MOSI_PORT	GPIOB	

#define NUM_BYTES		512

typedef	enum { SELECT, DESELECT } chip_select;

void ssd1306_reset();
void ssd1306_init();
void ssd1306_select_chip(chip_select state);
void ssd1306_command_mode();
void ssd1306_data_mode();
void ssd1306_display_begin();
void ssd1306_display_end();
void ssd1306_clear_screen();
void ssd1306_fill_screen();
void ssd1306_write_byte(SPI_TypeDef* SPIx, uint16_t data);
void ssd1306_display_buffer(uint8_t* buff);
void ssd1306_combine_graphics(uint8_t* buff, uint8_t* first, uint8_t* second);

#endif /* !SSD1306_H */
