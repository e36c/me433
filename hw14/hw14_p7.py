import csv
import numpy as np
import matplotlib.pyplot as plt
import hw14_p7_helper as filters


### import data
with open('data/sigC.csv') as f:
    t = []  # column 0
    d = []  # column 1
    reader = csv.reader(f)          # open the csv file
    for row in reader:              # iterate through rows
        t.append(float(row[0]))     # read in first column
        d.append(float(row[1]))     # read in second column



### do stuff with data

selected_filter = filters.h_C1
sel_filter_name = filters.h_C1_name
sel_filt_cutoff = filters.h_C1_cutoff

fir_output = []
for i in selected_filter:
    fir_output.append(i)

for i in range(len(selected_filter),len(d)):
    val = 0
    for j in range(len(selected_filter)):
        val += selected_filter[j] * d[i-len(selected_filter) + j]
    fir_output.append(val)





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

Dmaf = np.fft.fft(fir_output)/n      # fft computing and normalization
Dmaf = Dmaf[range(int(n/2))]
###



### create subplots w/ time and frequency domain of signal
fig, (ax1, ax2) = plt.subplots(2, 1)
ax1.plot(t,d,'k')
ax1.plot(t,fir_output,'r')
ax1.set_xlabel('Time')
ax1.set_ylabel('Amplitude')
ax1.set_title('Signal vs Time, Applying ' + str(len(selected_filter)) + '-Coeff ' + sel_filter_name + ' Filter with w_cutoff = ' + str(sel_filt_cutoff))
ax2.loglog(frq,abs(D),'k') # plotting the fft
ax2.loglog(frq,abs(Dmaf),'r')
ax2.set_xlabel('Freq (Hz)')
ax2.set_ylabel('|Y(freq)|')
ax2.set_title('Amplitude vs Frequency, Applying ' + str(len(selected_filter)) + '-Coeff ' + sel_filter_name + ' Filter with w_cutoff = ' + str(sel_filt_cutoff))
plt.show()
###


### example plot
# plt.plot(t,d,'b-*')
# plt.xlabel('Time [s]')
# plt.ylabel('Signal')
# plt.title('Signal vs Time')
# plt.show()