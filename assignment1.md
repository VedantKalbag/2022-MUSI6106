# Assignment 1
## Combfilter using FIR and IIR filters
Note: Please view in light mode to properly view the plots
### Sine Wave Output Diff
The input was a sine wave of 100Hz. It was filtered using the C++ and provided matlab codes with a gain of -1 and a delay of 0.01s (or 1 cycle)
Raw output for FIR filter (Test 1 output):  
![](/report/test1.png)

Raw output for IIR filter with gain=1 and delay=0.01s (Test 2 output):  
![](/report/output.png)

Diff between provided code and C++ implementation:   
![](/report/sine_diff.png)

### Real world audio tests:
Two different files were passed through both the FIR and IIR filter implementations, and compared to the results through the provided implementation.
The audio files are perceptually similar, but there is a noticeable difference between the matlab process and the C++ code when compared sample by sample.

File: 8.wav
FIR Diff:  
![](/report/8_fir_diff.png)

IIR Diff:  
![](/report/8_iir_diff.png)

File: 6.wav
FIR Diff:  
![](/report/6_fir_diff.png)

IIR Diff:  
![](/report/6_iir_diff.png)
