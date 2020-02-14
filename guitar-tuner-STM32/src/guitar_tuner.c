#include "guitar_tuner.h"

float ideal_frequencies[] = {82.0312*2, 109.375*3, 148.437, 195.312, 246.093, 328.125 };
float thresh[6] = {0.5, 0.7, 0.9, 1.0, 1.2, 1.2};

/**
 *  Send all real, or imaginary values of complex value v over UART
 *  @param v: pointer to complex struct
 *  @param len: number of values in v
 *  @param real: if real==1, send real values, else send imaginary component
 *  @param USARTx: pointer to USART channel
 */
void print_coeff_uart(complex* v, int len, int real,  USART_TypeDef* USARTx)
{
    char buffer[10];
    int i;
    if(real == 1)
    {
	for(i=0; i<len; i++)
	{
	    sprintf(buffer, "%.2f ,", v[i].Re);
	    uart_puts(buffer, USARTx);
	}
    }
    else
    {
	for(i=0; i<len; i++)
	{
	    sprintf(buffer, "%.2f ,", v[i].Im);
	    uart_puts(buffer, USARTx);
	}
    }

    uart_puts("\n\r", USART1);
}

/**
 * Fill array of floats 'freq' with the frequency values of each corresponding bin in the DFT
 * @param freq: pointer to array of size N
 * @param N: number of bins in the DFT 
 * @param t_sampling: time between samples (1/sampling_frequency) 
 */
void get_freq_bins(float* freq, int N, float t_sampling)
{
    int i;
    float divisor = N*t_sampling;
    for(i=0; i<(N/2); i++)
    {
	freq[i] = (float)i/divisor;
    }

    for(i=(N/2); i<N; i++)
    {
	freq[i] = (float)-(N-i)/divisor;
    }
}

/**
 * Convert a given FFT frequency bin to it's index in the array
 * @param freq: desired frequency to find index of
 * @param fs: sampling frequency in Hz
 * @param N: FFT size
 */
int freq_to_index(float freq, int fs, int N)
{
    return (int) (freq*N/fs);
}

/**
 * Send all float values of a given array over UART
 * @param float_array: array of float values
 * @param N: Number of values to send
 * @param USARTx: pointer to USART port
 */
void print_float_array(float* float_array, int N, USART_TypeDef* USARTx)
{
    char buffer[10];
    int i;
    for(i=0; i<N; i++)
    {
	sprintf(buffer, "%.4f ,", float_array[i]);
	uart_puts(buffer, USARTx);
    }
    uart_puts("\n\r", USARTx);
}

/**
 * Send all uint16_t values of a given array over UART
 * @param u16_array: array of float values
 * @param N: Number of values to send
 * @param USARTx: pointer to USART port
 */
void print_u16_array(u16* u16_array, int N, USART_TypeDef* USARTx)
{
    char buffer[10];
    int i;
    for(i=0; i<N; i++)
    {
	sprintf(buffer, "%d ,", u16_array[i]);
	uart_puts(buffer, USARTx);
    }
    uart_puts("\n\r", USARTx);
}

/**
 * Calculate the magnitude of complex value from the FFT
 * @param magnitudes: array of floats in which to store magnitude values
 * @param v: array of complex values resulting from FFT
 * @param N: number of points in FFT
 */
void get_magnitudes(float* magnitudes, complex* v, int N)
{
    int i;
    for(i=0; i<N; i++)
    {
	magnitudes[i] = (float)sqrt(v[i].Re*v[i].Re + v[i].Im*v[i].Im);
    }
}

/**
 * Return frequency of the greatest magnitude frequency bin
 * @param magnitudes: array of floats representing FFT coefficients
 * @param freq: array of floats representing FFT frequency bins
 * @param N: length of frequency domain signal
 */
float get_max_frequency_component(float* magnitudes, float* freq, int N)
{
    float max_value = 0.0;
    int max_index = 0;
    int i;

    for(i=0; i<N; i++)
    {
	if(magnitudes[i] > max_value)
	{
	    max_value = magnitudes[i];
	    max_index = i;
	}
    }
    return freq[max_index];
}

/**
 * Remove unwanted frequencies from frequency domain signal by setting undesired frequencies to a FFT coefficient of 0
 * @param magnitudes: array of floats representing FFT coefficients
 * @param freq: array of floats representing FFT frequency bins
 * @param freq_to_remove: desired frequency to be removed
 * @param thresh: Remove all frequencies between freq_to_remove +- thresh
 * @param N: length of frequency domain signal
 */
void filter_frequencies(float* magnitudes, float* freq, float freq_to_remove, float thresh, int N)
{
    int i;

    for(i=0; i<N; i++)
    {
	if( fabs((fabs(freq[i]) - freq_to_remove)) < thresh)
	{
	    magnitudes[i] = 0;
	}
    }
}

/**
 * Estimate the continuous time fundamental frequency using the frequency domain signal
 * and the estimate for the guitar string that was played
 * @param magnitudes: array of floats representing FFT coefficients
 * @param freq: array of floats representing FFT frequency bins
 * @param max_string: estimated guitar string that was played
 */
float estimate_frequency(float* mag, float* freq, guitar_string max_string)
{
    int f = freq_to_index( ideal_frequencies[max_string], 1000, ADC_BUFF_SIZE );
    float weighted_sum = 0;
    float divisor = 0;

    int i;
    for(i=(f-1); i<=(f+2); i++)
    {
	weighted_sum += mag[i] * freq[i];
	divisor += mag[i];
    }

    return weighted_sum/divisor;
}

/**
 * Fill buffer that will be displayed on the OLED with the proper tuning instruction
 * based on the estimated frequency and the nearest guitar string.
 * Tuning instructions can be: tune up (up arrow), tune down (down arrow), and in-tune (circle). The display buffer is filled to display the nearest guitar string and the instruction.
 * @param difference: estimated frequency - ideal frequency
 * @param buff: 512 Byte buffer that will be written/displayed on the OLED screen
 * @param str: predicted nearest guitar string based on frequency domain signal
 */
void tuning_instruction(float difference, uint8_t* buff, guitar_string str)
{
    switch(str)
    {
	case E: 
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, E_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, E_bits, up_bits);
	    if(difference > thresh[E]) ssd1306_combine_graphics(buff, E_bits, down_bits);
	    else if(difference < -thresh[E]) ssd1306_combine_graphics(buff, E_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, E_bits, circle_bits);
	    break;
	case A:
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, A_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, A_bits, up_bits);
	    if(difference > thresh[A]) ssd1306_combine_graphics(buff, A_bits, down_bits);
	    else if(difference < -thresh[A]) ssd1306_combine_graphics(buff, A_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, A_bits, circle_bits);
	    break;
	case D: 
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, D_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, D_bits, up_bits);
	    if(difference > thresh[D]) ssd1306_combine_graphics(buff, D_bits, down_bits);
	    else if(difference < -thresh[D]) ssd1306_combine_graphics(buff, D_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, D_bits, circle_bits);
	    break;
	case G: 
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, G_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, G_bits, up_bits);
	    if(difference > thresh[G]) ssd1306_combine_graphics(buff, G_bits, down_bits);
	    else if(difference < -thresh[G]) ssd1306_combine_graphics(buff, G_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, G_bits, circle_bits);
	    break;
	case B: 
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, B_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, B_bits, up_bits);
	    if(difference > thresh[B]) ssd1306_combine_graphics(buff, B_bits, down_bits);
	    else if(difference < -thresh[B]) ssd1306_combine_graphics(buff, B_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, B_bits, circle_bits);
	    break;
	case e: 
	    // if(difference > THRESH) ssd1306_combine_graphics(buff, e_bits, down_bits);
	    // else if(difference < -THRESH) ssd1306_combine_graphics(buff, e_bits, up_bits);
	    if(difference > thresh[e]) ssd1306_combine_graphics(buff, e_bits, down_bits);
	    else if(difference < -thresh[e]) ssd1306_combine_graphics(buff, e_bits, up_bits);
	    else  ssd1306_combine_graphics(buff, e_bits, circle_bits);
	    break;
    }

}
