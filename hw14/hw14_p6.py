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


### do stuff with data

# iir weighted average
A = 0.9
B = 1-A

iir_output = [d[0]]

for i in range(1,len(d)):
    iir_output.append( A*iir_output[i-1] + B*d[i])





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

Dmaf = np.fft.fft(iir_output)/n      # fft computing and normalization
Dmaf = Dmaf[range(int(n/2))]
###



### create subplots w/ time and frequency domain of signal
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,d,'k')
ax1.plot(t,iir_output,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Signal vs Time, with A=' + str(A)+' and B='+ str(B)[0:4])
ax2.loglog(frq,abs(D),'k') # plotting the fft
ax2.loglog(frq,abs(Dmaf),'r')
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
ax2.set_title('Amplitude vs Frequency, with A=' + str(A)+' and B='+ str(B)[0:4])
plt.show()
###


### example plot
# plt.plot(t,d,'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()