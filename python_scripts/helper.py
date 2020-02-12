import numpy as np
import pickle
import matplotlib.pyplot as plt

START_FLAG = b'START:'
END_FLAG = b'END'

string_freq = {
    'E': 82,
    'A': 110,
    'D': 147,
    'G': 196,
    'B': 247,
    'e': 330,
}

nearby_freq = {
    # 'E': [74.21875 , 78.125 , 82.03125 , 85.9375 , 89.84375] ,
    'E': [74.2188 , 78.125 , 82.0312 , 85.9375 , 89.8437 ,
            156.25 , 160.1562 , 164.0625 , 167.9687 , 171.875 ,
            238.2812 , 242.1875 , 246.0937 , 250.0 , 253.9062 ,
#            320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 ,
            402.3437 , 406.25 , 410.1562 , 414.0625 , 417.9687] ,
    # 'A': [101.5625 , 105.4687 , 109.375 , 113.2812 , 117.1875 ,
            # 210.9375 , 214.8437 , 218.75 , 222.6562 , 226.5625 ,
            'A': [210.9375 , 214.8437 , 218.75 , 222.6562 , 226.5625 ,
            320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 ] ,
            # 320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 ] ,
    'D': [140.625 , 144.5312 , 148.4375 , 152.3438 , 156.25 ,
            285.1562 , 289.0625 , 292.9688 , 296.875 , 300.7812 ,
            433.5937 , 437.5 , 441.4062 , 445.3125 , 449.2187 ] ,
    'G': [187.5 , 191.4062 , 195.3125 , 199.2187 , 203.125 ,
            382.8125 , 386.7187 , 390.625 , 394.5312 , 398.4375 ] ,
    'B': [238.2812 , 242.1875 , 246.0937 , 250.0 , 253.9062 ] ,
    'e': [320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 ]

}

# Nearby frequencies of each guitar string
frequencies_of_interest = list(dict.fromkeys([
    74.2188 , 78.125 , 82.0312 , 85.9375 , 89.8437 ,
       156.25 , 160.1562 , 164.0625 , 167.9687 , 171.875 ,
       238.2812 , 242.1875 , 246.0937 , 250.0 , 253.9062 ,
       320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 ,
       402.3437 , 406.25 , 410.1562 , 414.0625 , 417.9687 ,
    101.5625 , 105.4687 , 109.375 , 113.2812 , 117.1875 ,
       210.9375 , 214.8437 , 218.75 , 222.6562 , 226.5625 ,
       320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375  ,
    140.625 , 144.5312 , 148.4375 , 152.3438 , 156.25 ,
       285.1562 , 289.0625 , 292.9688 , 296.875 , 300.7812 ,
       433.5937 , 437.5 , 441.4062 , 445.3125 , 449.2187  ,
    187.5 , 191.4062 , 195.3125 , 199.2187 , 203.125 ,
       382.8125 , 386.7187 , 390.625 , 394.5312 , 398.4375  ,
    238.2812 , 242.1875 , 246.0937 , 250.0 , 253.9062  ,
    320.3125 , 324.2187 , 328.125 , 332.0312 , 335.9375 
]))
frequencies_of_interest.sort()

def parse_input_as_float(output, ser):

    ser.read_until(START_FLAG)
    temp = ser.read_until(END_FLAG)
    temp = temp.decode() # convert bytestring to string
    temp = temp.split('\n')[:-1] # split string into list of strings
    for each in temp:
        # strip \r characters and split by commas. Ignore last element since it is empty
        if 'START:' in each:
            a = len('START:')
            each = each[a:]
        output.append( [ float(i) for i in each.strip().split(',')[:-1] ] )

def write_pickle_file(list_to_write, filename):
    with open(filename, 'wb') as f:
        pickle.dump(list_to_write, f)

def read_pickle_file(filename):
    with open(filename, 'rb') as f:
        return pickle.load(f)

def plot_signal(signal, title):
    plt.plot(signal)
    plt.grid()
    plt.title(title)
    plt.show()

def stem_plot_signal(x, signal, title):
    plt.stem(x, signal, use_line_collection=True)
    plt.grid()
    plt.title(title)
    plt.show()

def sub_plot_signals(signals, rows, columns):
    fig, a = plt.subplots(rows,columns)
    keys = list(signals.keys())
    for i in range(rows*columns):
        a[i//columns][i%columns].plot(signals[keys[i]][0])
        a[i//columns][i%columns].grid()
        a[i//columns][i%columns].set_title(keys[i])
    plt.show()

def sub_stemplot_signals(freq, signals, rows, columns):
    fig, a = plt.subplots(rows,columns)
    keys = list(signals.keys())
    for i in range(rows*columns):
        a[i//columns][i%columns].stem(freq, signals[keys[i]], use_line_collection=True)
        a[i//columns][i%columns].grid()
        a[i//columns][i%columns].set_title(keys[i])
    plt.show()

def filter_frequencies(x, freq_list, freq_to_remove, thresh):
    '''
    Remove unwanted frequencies in a frequency domain signal x, with frequencies in freq_list.
    @param x: frequency domain signal
    @param freq_list: corresponding frequency to each value in x
    @param freq_to_remove: desired frequency to set to 0
    @param thresh: how many Hz above and below the frequency to remove should the signal be set to 0
    example: filter_frequencies(x, freq, [60], 3) will set to 0 all values in x where frequency is 60Hz +-3Hz
    '''
    for i, freq in enumerate(freq_list):
        for f in freq_to_remove: 
            if np.abs(np.abs(freq)-f) < thresh:
                for series in x:
                    series[i] = 0

def filter_frequencies_complex(x, freq_list, freq_to_remove, thresh):
    for i, freq in enumerate(freq_list):
        for f in freq_to_remove: 
            if np.abs(np.abs(freq)-f) < thresh:
                for series in x:
                    series[i] = (0+0j)

def dot_product(x, y):
    if len(x) != len(y): return
    
    run_sum = 0
    for xx, yy in zip(x,y):
        run_sum += xx*yy
    return run_sum

def sum_of_differences(x,y):
    if len(x) != len(y): return
        
    run_sum = 0
    for xx, yy in zip(x,y):
        run_sum += np.abs(xx-yy)
    return run_sum

def autocorrelate(x):
    '''
    perform auto correlation on signal x. compare signal x to time shifted verisons of it
    '''
    signal_length = len(x)
    half_signal_length = signal_length//2
    result = []
    for i in range(half_signal_length):
        shifted_signal = x[(i+1):(i+1+half_signal_length)]
        result.append(sum_of_differences(x[:half_signal_length], shifted_signal ))

    return result

def estimate_freq_from_ar(autocorr, fs):
    '''
    Estimate a signals frequency based on it's autocorrelation
    '''
    offset = np.argmin(autocorr[1:13]) + 2
    return fs/offset

def midpoint(a, b):
    return (a+b)/2

def predict_string(freq):
    '''
    Based on the estimated frequency from performing autocorrelation, 
    return the guitar string with the frequency closest to the autocorrelation estimate
    '''
    if freq < midpoint(string_freq['E'], string_freq['A']): return 'E'
    elif freq < midpoint(string_freq['A'], string_freq['D']): return 'A'
    elif freq < midpoint(string_freq['D'], string_freq['G']): return 'D'
    elif freq < midpoint(string_freq['G'], string_freq['B']): return 'G'
    elif freq < midpoint(string_freq['B'], string_freq['e']): return 'B'
    elif freq >= midpoint(string_freq['B'], string_freq['e']): return 'e'
    
def get_averaged_freq(signal, freq_list,  target_freq, thresh, plot=False):
    # find indices of frequencies within a threshold distance to target
    # perform a weighted sum with the given indices and corresponding values
    weighted_sum = 0
    divisor = 0
    fft_sig = np.abs(np.fft.fft(signal))
    fft_sig[0] = 0
    for i, f in enumerate(freq_list):
        if(np.abs(f-target_freq) < thresh ): 
            if plot: plt.stem([freq_list[i]], [fft_sig[i]], 'r', use_line_collection=True)
            weighted_sum += freq_list[i]*fft_sig[i]
            divisor += fft_sig[i]
        else:
            if plot: plt.stem([freq_list[i]], [fft_sig[i]], use_line_collection=True)
            
    if plot: 
        plt.grid()
        plt.show()

    return (weighted_sum/divisor)

def predict_frequency(signal, freq_list, string_prediction):
    '''
    Knowing the predicted guitar string, predict the frequency of the waveform
    by taking an average of DFT coefficients around that frequency
    '''
    return get_averaged_freq(signal, freq_list, string_freq[string_prediction] , 9, plot=False)

def get_nearest_string(signal, fs):
    '''
    perform autocorrelation at 6 offsets (representing the periods of the six strings)
    return char corresponding to guitar string with highest autocorrelation value
    '''
    offsets = dict.fromkeys(string_freq)
    for k in offsets.keys():
        offsets[k] = round(fs/string_freq[k])

    signal_length = len(signal)
    half_signal_length = signal_length//2

    results = dict.fromkeys(string_freq)
    for k in offsets.keys():
        shifted_signal = signal[(offsets[k]+1):(offsets[k]+1+half_signal_length)]
        results[k] = sum_of_differences(signal[:half_signal_length], shifted_signal )

    return min(results, key=results.get)

def cos_dft_vector(freq, fs,  N):
    '''
    return vector of length N of cosine function at given frequency and sampling frequency
    For DFT, a dot product is performed with this cosine vector and a periodic signal
    '''
    return [np.cos(2*np.pi*freq*n/fs) for n in range(N) ]

def sin_dft_vector(freq, fs,  N):
    '''
    return vector of length N of sin function at given frequency and sampling frequency
    For DFT, a dot product is performed with this sin vector and a periodic signal
    '''
    return [ -np.sin(2*np.pi*freq*n/fs) for n in range(N) ]

def partial_dft(signal, nearby_freq, N):
    '''
    Perform dot product between signal and DFT basis vectors representing frequencies
    speicified by nearby_freq
    '''
    real_part = dict.fromkeys(nearby_freq)
    imag_part = dict.fromkeys(nearby_freq)
    mag = dict.fromkeys(nearby_freq)
    for freq in nearby_freq:
        real_part[freq] = 0
        imag_part[freq] = 0
        for n in range(N):
            real_part[freq] += signal[n] * np.cos(2*np.pi*freq*n/1000)
            imag_part[freq] += signal[n] * np.sin(2*np.pi*freq*n/1000)
        mag[freq] = np.sqrt(real_part[freq]**2 + imag_part[freq]**2)

    return real_part, imag_part, mag

def weighted_average(values, weights):
    '''
    Perform a weighted average on values, with corresponding weights
    '''
    if len(values) != len(weights): return

    weighted_sum = 0
    divisor = 0
    for v, w in zip(values, weights):
        weighted_sum += v*w
        divisor += w
    return weighted_sum/divisor

def get_average_fft(fft_signals):
    '''
    return an average fft frequency response for the given signals
    @param frequencies: frequency bins of the FFT
    @param fft_signals: list holding dictionaries of FFT signals {freq:mag}
    '''
    avg = dict.fromkeys(fft_signals[0].keys())
    for f in fft_signals[0].keys():
        avg[f] = np.average([ sig[f] for sig in fft_signals ] )

    return avg

def get_eigen_fft(fft_signals, freq_of_interest):
    avg = get_average_fft(fft_signals)
    eig_vec = [ avg[f] for f in freq_of_interest ]
    return [ i/np.linalg.norm(eig_vec) for i in eig_vec ]

