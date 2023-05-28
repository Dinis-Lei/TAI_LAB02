import matplotlib.pyplot as plt
import numpy as np
import sys
from scipy.signal import butter, filtfilt
from math import log2

fname = sys.argv[1]

# open file and plot data
with open(fname, 'r') as f:
    data = f.read().split('\n')
    data = np.array([float(i) for i in data if i != ''])
    card = data[0]
    data = data[1:]
    print(log2(card))
    base = [log2(card)]*len(data)
    # print(data)
    # print(sum(data))

    # Define the filter parameters
    fs = 100     # Sampling frequency (Hz)
    cutoff = 10  # Cutoff frequency (Hz)
    order = 2    # Filter order

    # Calculate the filter coefficients using a Butterworth filter
    b, a = butter(order, cutoff / (fs / 2), btype='low')

    # Apply the filter using the filtfilt function
    y1 = filtfilt(b, a, data)

    print(np.sum(data <= log2(card)/2))
    print(np.sum(y1 <= log2(card)/2))

    step = 1000
    for i in range(0, len(y1)-1, step):
        plt.figure(i, figsize=(10, 5))
        plt.title(fname)
        #plt.scatter(x=range(len(data)), y=y1)
        plt.plot(y1[i:i+step])
        #plt.plot(base, color='red')
        plt.plot([log2(card)/2]*len(y1[i:i+step]), color='green')

        plt.figure(i+1, figsize=(10, 5))
        plt.plot(data[i:i+step])

        plt.show()
