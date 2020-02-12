#ifndef FFT_H
#define FFT_H 
/* Factored discrete Fourier transform, or FFT, and its inverse iFFT */

#include <math.h>
#include <stdio.h>

typedef float real;
typedef struct{real Re; real Im;} complex;

#ifndef PI
# define PI	3.14159265358979323846264338327950288
#endif


/* Print a vector of complexes as ordered pairs. */

void fft( complex *v, int n, complex *tmp );
void ifft( complex *v, int n, complex *tmp );

#endif /* ifndef FFT_H */
