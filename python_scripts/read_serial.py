'''
This script reads from the serial port. 
'''
import serial
import matplotlib.pyplot as plt
import numpy as np
import time

from helper import *

# values_bytestring = []
# values_str = []
# values_int = []
x = []

with serial.Serial('/dev/ttyUSB0', 115200, timeout=20) as ser:
    parse_input_as_float(x, ser)

# write_pickle_file(x, 'guitare_256_4.txt')


'''
mag = x[0:8:2]
freq = x[1:8:2]

fft_list = [ dict(zip(f,m)) for f, m in zip(freq, mag)]
'''

# write_pickle_file(fft_list, 'pickled_waveforms/frequency_domain_waveforms/fft_B_256_4.txt')

'''
fig, a = plt.subplots(2,2)
for i in range(len(mag)):
    a[i//2][i%2].stem(freq[i], mag[i], use_line_collection=True)
    a[i//2][i%2].grid()

plt.show()
'''



'''
wave = []
for i in x:
    for sample in i:
        wave.append(sample)
'''

'''
fig, a = plt.subplots(2,2)
for i in range(len(x)):
    a[i//2][i%2].plot(x[i])
    a[i//2][i%2].grid()

plt.show()
'''


'''
fig, a = plt.subplots(2,2)
for i in range(len(x)):
    a[i//2][i%2].stem(autocorrelate(x[i]), use_line_collection=True)
    a[i//2][i%2].grid()

plt.show()
'''



'''
# fftwaves = [np.abs(np.fft.fft(i)) for i in x]
# fftwave = np.abs(np.fft.fft(wave))
fftwaves = [np.fft.fft(i) for i in x]
freq = np.fft.fftfreq(len(x[0]), 0.001)

filter_frequencies_complex(fftwaves, freq, [0, 60.0], 4)
filtered_signals = [ np.fft.ifft(i) for i in fftwaves ]
# plt.plot(wave)
fig, a = plt.subplots(2,2)
for i in range(len(x)):
    # a[i//2][i%2].stem(freq, fftwaves[i], use_line_collection=True)
    # a[i//2][i%2].plot(filtered_signals[i].real)
    a[i//2][i%2].stem(autocorrelate(filtered_signals[i].real) , use_line_collection=True)
    a[i//2][i%2].grid()

# plt.stem(freq, fftwaves[0], use_line_collection=True)
plt.show()
'''



'''
freq = np.fft.fftfreq(256, 0.001)

real, imag, magnitude = get_dft_coeff(x)
mag_real = np.abs(real)

# filter_60hz([real, imag, magnitude, mag_real], freq)
# filter_frequencies([real, imag, magnitude, mag_real], freq, [60], 5)
# filter_frequencies([real, imag, magnitude, mag_real], freq, [0], 65)


fig, a = plt.subplots(4,1)

a[0].stem(freq, real, use_line_collection=True)
a[0].set_title('Real')

a[1].stem(freq, imag, use_line_collection=True)
a[1].set_title('Imaginary')

a[2].stem(freq, magnitude, use_line_collection=True)
a[2].set_title('Magnitude')

a[3].stem(freq, mag_real, use_line_collection=True)
a[3].set_title('Magnitude of Real part')

for i in a:
    i.grid()

plt.show()
'''
