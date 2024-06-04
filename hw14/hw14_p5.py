import csv
import numpy as np
import matplotlib.pyplot as plt


### import data
with open('data/sigA.csv') as f:
    t = []  # column 0
    d = []  # column 1
    reader = csv.reader(f)          # open the csv file
    for row in reader:              # iterate through rows
        t.append(float(row[0]))     # read in first column
        d.append(float(row[1]))     # read in second column


### do stuff with data

maf_width = 75
maf_output = []
for i in range(0,maf_width): maf_output.append(0)   # populate beginning of signal with zeros

for i in range(maf_width,len(d)):
    maf_output.append(np.average(d[i:i+maf_width]))





### calculate sample rate
sample_rate = (len(t)-1) / t[-1]

### take FFTs of unfiltered and filtered data
sample_T = 1/sample_rate    # sample period
n = len(d)                  # save len(d)
k = np.arange(n)            # create k variable (?)
frq = k/sample_T            # two sided frequency range
frq = frq[range(int(n/2))]  # one side frequency range

D = np.fft.fft(d)/n         # fft computing and normalization
D = D[range(int(n/2))]

Dmaf = np.fft.fft(maf_output)/n      # fft computing and normalization
Dmaf = Dmaf[range(int(n/2))]
###



### create subplots w/ time and frequency domain of signal
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,d,'k')
ax1.plot(t,maf_output,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Signal vs Time, with ' + str(maf_width) + ' Points Averaged')
ax2.loglog(frq,abs(D),'k') # plotting the fft
ax2.loglog(frq,abs(Dmaf),'r')
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
ax2.set_title('Amplitude vs Frequency, with ' + str(maf_width) + ' Points Averaged')
plt.show()
###


### example plot
# plt.plot(t,d,'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()