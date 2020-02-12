#ifndef GUITAR_TUNER_H
#define GUITAR_TUNER_H

#include "stm32f10x.h"
#include "fft.h"
#include "uart.h"
#include "ssd1306.h"
#include "string_bitmaps.h"
#include "arrow_bitmaps.h"

#define ADC_BUFF_SIZE 256

#define REAL_PART 1
#define IMAG_PART 0

// #define THRESH 0.7
float thresh[6];

typedef enum {E, A, D, G, B, e} guitar_string;

extern float ideal_frequencies[6];
extern u16 buffer_count;

complex fft_samples[ADC_BUFF_SIZE], temp[ADC_BUFF_SIZE];

void print_coeff_uart(complex* v, int len, int real,  USART_TypeDef* USARTx);
void get_freq_bins(float* freq, int N, float f_sampling);
int freq_to_index(float freq, int fs, int N);
void print_float_array(float* freq, int N, USART_TypeDef* USARTx);
void get_magnitudes(float* magnitudes, complex* v, int N);
float get_max_frequency_component(float* magnitudes, float* freq, int N);
void filter_frequencies(float* magnitudes, float* freq, float freq_to_remove, float thresh, int N);
void print_u16_array(u16* arr, int N, USART_TypeDef* USARTx);
float estimate_frequency(float* mag, float* freq, guitar_string max_string);
void tuning_instruction(float difference, uint8_t* buff, guitar_string str);


#endif /* !GUITAR_TUNER_H */
