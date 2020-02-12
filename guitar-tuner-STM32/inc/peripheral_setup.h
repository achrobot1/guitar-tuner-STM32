#ifndef PERIPHERAL_SETUP_H
#define PERIPHERAL_SETUP_H

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_spi.h"
#include "misc.h"

#include "ssd1306.h" // contains pin defines for SSD1306 OLED screen

#define LED2_PIN	GPIO_Pin_5
#define LED2_PORT	GPIOA

#define ADC_PIN		GPIO_Pin_0
#define ADC_PORT	GPIOC

#define TIM_PERIOD	(36000-1)

void GPIO_setup();
void RCC_setup();
void NVIC_setup();
void TIM_setup();
void ADC1_setup();
void SPI_setup();

#endif /* !PERIPHERAL_SETUP_H */
