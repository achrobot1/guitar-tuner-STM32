/*
 * interrupts.h
 * Copyright (C) 2020 antoni <antoni@antoni>
 *
 * Distributed under terms of the MIT license.
 */

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"
#include "interrupts.h"
#include "guitar_tuner.h"

#include "window.h"


u16 buffer_count = 0;
u16 adc_buffer[ADC_BUFF_SIZE];
/*
 * interrupt service routine for TIM2 interrupt.
 */
void TIM2_IRQHandler(void)
{
    TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);

    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));
    u16 adc_value = ADC_GetConversionValue(ADC1);

    if(buffer_count < ADC_BUFF_SIZE)
    {
	fft_samples[buffer_count].Re = 0.0;
	fft_samples[buffer_count].Im = 0.0;
	// fft_samples[buffer_count++].Re = adc_value;
	fft_samples[buffer_count++].Re = adc_value * window[buffer_count] ;
    }
}


