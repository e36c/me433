import csv
import numpy as np
import matplotlib.pyplot as plt


### import data
with open('data/sigD.csv') as f:
    t = []  # column 0
    d = []  # column 1
    reader = csv.reader(f)          # open the csv file
    for row in reader:              # iterate through rows
        t.append(float(row[0]))     # read in first column
        d.append(float(row[1]))     # read in second column





### calculate sample rate
sample_rate = (len(t)-1) / t[-1]

### take FFT
sample_T = 1/sample_rate    # sample period
n = len(d)                  # this will be used a good bit
k = np.arange(n)            # create k variable (?)
frq = k/sample_T            # two sided frequency range
frq = frq[range(int(n/2))]  # one side frequency range
D = np.fft.fft(d)/n         # fft computing and normalization
D = D[range(int(n/2))]

### create subplots w/ time and frequency domain of signal
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,d,'b')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Signal vs Time')
ax2.loglog(frq,abs(D),'b') # plotting the fft
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
ax2.set_title('Amplitude vs Frequency')
plt.show()
###


### example plot
# plt.plot(t,d,'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()