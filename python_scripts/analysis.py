from helper import *
import numpy as np
import os

WAVEFORM_DIR = 'pickled_waveforms/raw_waveforms'
FFT_DIR = 'pickled_waveforms/frequency_domain_waveforms'
ROWS = 2
COLUMNS = 4
SIGNAL_LENGTH = 256


strings = ['E', 'A', 'D', 'G', 'B', 'e']

raw_signals = dict()
fft_signals = dict()
for each in strings:
    raw_signals[each] = read_pickle_file(os.path.join(WAVEFORM_DIR, 'guitar%s_256_4.txt'%each))
    fft_signals[each] = read_pickle_file(os.path.join(FFT_DIR, 'fft_%s_256_4.txt'%each))


freq = np.fft.fftfreq(SIGNAL_LENGTH, 0.001)


##################################################################
'''
Raw signal in time domain analysis
'''

'''
# Perform fft on the first sample of each signal and filter DC component
fft_waves = dict().fromkeys(raw_signals.keys())
for k in raw_signals.keys():
    fft_waves[k] = np.abs(np.fft.fft(raw_signals[k][0] ))
    filter_frequencies([fft_waves[k]], freq, [0.0], 5.0) # Filter out DC component
'''

'''
# Perform fft on all D string samples and filter DC component
fft_D = dict()
for i, sig in enumerate(raw_signals['D']):
    fft_D[i] = np.abs(np.fft.fft(sig))
    filter_frequencies([fft_D[i]], freq, [0.0], 5.0) # Filter out DC component

plt.plot(raw_signals['D'][0])
for offset in [7, 14]:
    plt.plot(raw_signals['D'][0][offset:offset+SIGNAL_LENGTH//2])
plt.show()
'''

'''
# Estimate frequency from autocorrelation for all samples
estimated_D = [ estimate_freq_from_ar(autocorrelate(i), 1000) for i in raw_signals['D'] ]
fig, a = plt.subplots(2,2)
for i in range(2*2):
    a[i//2][i%2].stem(freq, fft_D[i], use_line_collection=True)
    a[i//2][i%2].stem([estimated_D[i]], [2500], 'r', use_line_collection=True)
    a[i//2][i%2].grid()
    a[i//2][i%2].set_title(i)
plt.show()
'''


# stem plots of frequency domain signals
# sub_stemplot_signals(freq, fft_waves, ROWS, COLUMNS)

'''
autocorr_signals = dict().fromkeys(raw_signals.keys())
for k in raw_signals.keys():
    # autocorr_signals[k] = autocorrelate(raw_signals[k][0])
    autocorr_signals[k] = [autocorrelate(i) for i in raw_signals[k] ]
'''

# sub_stemplot_signals(np.arange(SIGNAL_LENGTH//2)+1, autocorr_signals, ROWS, COLUMNS)
# stem_plot_signal(np.arange(SIGNAL_LENGTH//2)+1, autocorr_signals['E'], 'AutoCorrelation E')


'''
# estimate frequencies of signals after performing autocorrelation
estimated_freqs = dict().fromkeys(raw_signals.keys())
for k in raw_signals.keys():
    # estimated_freqs[k] = estimate_freq_from_ar(autocorr_signals[k], 1000)
    estimated_freqs[k] = [estimate_freq_from_ar(i, 1000) for i in autocorr_signals[k] ]
'''

'''
print('actual\t predicted\t Frequency ')
for k in raw_signals.keys():
    for est in estimated_freqs[k]:
        print(k, '\t', predict_string(est), '\t', est )
'''

'''
# first perform autocorrelation, then do a DFT with the nearest frequency basis vectors
for k in raw_signals.keys():
    for signal in raw_signals[k]:
        nearest_string = get_nearest_string(signal, 1000)
        real, imag, mag = partial_dft(signal, nearby_freq[nearest_string], SIGNAL_LENGTH)
        predicted_freq = weighted_average(list(mag.keys()), list(mag.values()) )
        print('%s \t %s \t %f \t %f \t %f' % (k,nearest_string, string_freq[k], predicted_freq, string_freq[k] - predicted_freq) )
'''

'''
string_weight = dict.fromkeys(strings)
for st in strings:
    real, imag, mag =  partial_dft(raw_signals['A'][2], nearby_freq[st], SIGNAL_LENGTH)
    string_weight[st] = np.sum(list(mag.values()) )

num_peaks = 0
max_peak = string_weight[max(string_weight, key=string_weight.get) ]
for st in strings:
    if string_weight[st] >= max_peak*0.75: num_peaks += 1

print(string_weight)
print(num_peaks)
'''

##################################################################

'''
Frequency domain signal analysis
fft_signals holds frequency domain signals from recorded sound of each guitar string.
For each guitar string, fft_signals has a dictionary, {freq:magnitude} , that has been sent from the STM32.
The STM32 performed 4 FFTs for each string, and removed DC and 60Hz components from the frequency domain signal
There are a total of 6*4 = 24 Frequency domain signals in fft_signals
'''

'''
# Stem plots of the first sample for each guitar string
for st in fft_signals.keys():
    freq = fft_signals[st][0].keys()
    mag = fft_signals[st][0].values()
    plt.stem(freq, mag, use_line_collection=True)
    plt.title(st)
    plt.grid(which='both')
    plt.minorticks_on()
    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()
    plt.show()
'''

'''
# plot all frequency domain samples of given string
for sig in fft_signals['e']:
    freq = sig.keys()
    mag = sig.values()
    plt.stem(freq, mag, use_line_collection=True)
    plt.title('e')
    plt.grid(which='both')
    plt.minorticks_on()
    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()
    plt.show()
'''



'''
# plot the averaged fft signal for 'A' and 'e'
for i in [avg_A, avg_e]:
    plt.stem(i.keys(), i.values(), use_line_collection=True)
    # plt.title('A')
    plt.grid(which='both')
    plt.minorticks_on()
    mng = plt.get_current_fig_manager()
    mng.window.showMaximized()
    plt.show()
    
plt.stem(fft_signals['A'][2].keys(), fft_signals['A'][2].values(), use_line_collection=True)
# plt.title('A')
plt.grid(which='both')
plt.minorticks_on()
mng = plt.get_current_fig_manager()
mng.window.showMaximized()
plt.show()
'''

'''
eig_vecs = dict.fromkeys(strings)

for k in eig_vecs.keys():
    eig_vecs[k] = get_eigen_fft(fft_signals[k], frequencies_of_interest)
'''


'''
# plot eigen_ffts for each guitar string
fig, a = plt.subplots(2,3)
for i,k in enumerate(eig_vecs.keys()):
    a[i//3][i%3].stem(frequencies_of_interest, eig_vecs[k], use_line_collection=True)
    a[i//3][i%3].grid()
    a[i//3][i%3].set_title(k)
plt.show()
'''

'''
for st in strings:
    run_sum = 0
    for i, f in enumerate(frequencies_of_interest):
        run_sum += fft_signals['e'][2][f] * eig_vecs[st][i]
    print(st, run_sum)
'''
eig_vecs = read_pickle_file('pickled_eig_vecs.txt')

# apply a threshold to eigvec to remove near 0 components
# Multiply each component by 100 and round to nearest integer value
# this is done to minimize floating point operations
for k in eig_vecs.keys():
    for i, val in enumerate(eig_vecs[k]):
        if val < 0.1: eig_vecs[k][i] = 0
        else: eig_vecs[k][i] = int(eig_vecs[k][i]*100)

print('eigen_frequencies = {', frequencies_of_interest, ' };')


'''
# perform dot product with fft signal and eig_ffts
for s in strings:
    string_weight = dict.fromkeys(eig_vecs.keys())
    for sig in fft_signals[s]:
        for k in eig_vecs.keys():
            weight_sum = 0
            for i, f in enumerate(frequencies_of_interest):
                weight_sum += sig[f]*eig_vecs[k][i]
            string_weight[k] = weight_sum 
        # print(string_weight)
        print(s, max(string_weight, key=string_weight.get))

'''
