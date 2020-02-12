#include "peripheral_setup.h"
#include "uart.h"
#include "interrupts.h"
#include "fft.h"
#include "guitar_tuner.h"
#include "unit_vec.h"
#include "delay.h"
#include "ssd1306.h"
#include "guitar_tuner_bitmap.h"
#include "string_bitmaps.h"
#include "arrow_bitmaps.h"
#include "tunings.h"

#include <stdio.h>
#include <math.h>

#define UART_DEBUG  1

u16 adc_buffer[ADC_BUFF_SIZE];


float last4[4] = {0};

float last4_average(float* last4, int i);
int temp_index = 0;

int main(void)
{
    if(UART_DEBUG) uart_open(USART1, 115200, 0);

    RCC_setup();
    GPIO_setup();
    SPI_setup();
    NVIC_setup();
    TIM_setup();
    ADC1_setup();

    // 3. Configure SysTick timer
    // It is called every 1ms. SystemCoreClock is defined
    // as number of system clock cycles per second
    if(SysTick_Config(SystemCoreClock / 1000))
	while(1);

    ssd1306_init();

    ssd1306_clear_screen();
    Delay(500);
    ssd1306_fill_screen();
    Delay(500);
    ssd1306_clear_screen();
    Delay(500);

    ssd1306_display_buffer(guitar_tuner_bits);
    Delay(1000);

    float top_freq;
    float mag[ADC_BUFF_SIZE];
    float freq[ADC_BUFF_SIZE];

    float real_part[UNIT_VEC_LENGTH];
    float imag_part[UNIT_VEC_LENGTH];
    float mag_part[UNIT_VEC_LENGTH];

    get_freq_bins(freq, ADC_BUFF_SIZE, 0.001);

    if(UART_DEBUG) uart_puts("START:", USART1);
    while(1)
    {
	Delay(10);

	if(buffer_count == ADC_BUFF_SIZE) // Check if adc_buffer is filled with values
	{
	    TIM_ITConfig(TIM2, TIM_IT_CC1, DISABLE); // Disable TIM2 interrupt

	    fft(fft_samples, ADC_BUFF_SIZE, temp);
	    get_magnitudes(mag, fft_samples, ADC_BUFF_SIZE);
	    filter_frequencies(mag, freq, 0.0, 4.0, ADC_BUFF_SIZE);
	    filter_frequencies(mag, freq, 60.0, 5.0, ADC_BUFF_SIZE);

	    // Dot product of unit_vecs with signal fft
	    int i;
	    float dp[6] = {0};
	    for(i=0; i<UNIT_VEC_LENGTH; i++)
	    {
		float f = eigen_frequencies[i];
		int index = freq_to_index(f, 1000, ADC_BUFF_SIZE);
		dp[E] += E_fft_unit_vec[i] * mag[index];
		dp[A] += A_fft_unit_vec[i] * mag[index];
		dp[D] += D_fft_unit_vec[i] * mag[index];
		dp[G] += G_fft_unit_vec[i] * mag[index];
		dp[B] += B_fft_unit_vec[i] * mag[index];
		dp[e] += e_fft_unit_vec[i] * mag[index];
	    }
	    
	    // find max string
	    float max_val = 0.0;
	    int max_index;
	    for(i=0; i<6; i++)
	    {
		if(dp[i] > max_val) 
		{
		    max_val = dp[i];
		    max_index = i;
		}
	    }

	    if(UART_DEBUG)
	    {
		char buffer[20];
		/*
		// Send string prediction over uart
		switch(max_index)
		{
		    case E: sprintf(buffer, "String: %s \n\r", "E");
			break;
		    case A: sprintf(buffer, "String: %s \n\r", "A");
			break;
		    case D: sprintf(buffer, "String: %s \n\r", "D");
			break;
		    case G: sprintf(buffer, "String: %s \n\r", "G");
			break;
		    case B: sprintf(buffer, "String: %s \n\r", "B");
			break;
		    case e: sprintf(buffer, "String: %s \n\r", "e");
			break;
		}
		uart_puts(buffer, USART1);
		*/

		float estimated_freq = estimate_frequency(mag, freq, max_index);
		if(max_index==E) estimated_freq /= 2;
		if(max_index==A) estimated_freq /= 3;
		sprintf(buffer, "%.4f\n\r", estimated_freq);
		uart_puts(buffer, USART1);
	    }

	    last4[temp_index] = estimate_frequency(mag, freq, max_index);

	    // float estimated_freq = estimate_frequency(mag, freq, max_index);
	    float estimated_freq = last4_average(last4, temp_index);
	    temp_index = (temp_index+1)%4;

	    float difference;
	    switch(max_index)
	    {
	        // case E: difference =  estimated_freq -  E_freq;
	        case E: difference =  estimated_freq/2 -  E_freq;
	            break;                                    
	        case A: difference =  estimated_freq/3 -  A_freq;
	            break;                             
	        case D: difference =  estimated_freq -  d_freq;
	            break;                             
	        case G: difference =  estimated_freq -  G_freq;
	            break;                             
	        case B: difference =  estimated_freq -  B_freq;
	            break;                             
	        case e: difference =  estimated_freq -  e_freq;
	            break;
	    }

	    uint8_t display_buff[NUM_BYTES];
	    tuning_instruction(difference, display_buff, max_index);

	    ssd1306_display_buffer(display_buff);
	    Delay(20);

	    buffer_count = 0;
	    TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
	    // break;
	}
    }

    if(UART_DEBUG)
    {
	uart_puts("END", USART1);
	uart_close(USART1);
    }

    while(1);
}

float last4_average(float* last4, int i)
{
    return (last4[i] + last4[(i+1)%4] + last4[(i+2)%4] + last4[(i+3)%4])*0.25;
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line)
{
    while(1);
}
#endif

