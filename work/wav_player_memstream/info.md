# Wav player memstream

The wav player memstream is a simple barematel raspberrypi program that plays a wav file previously loaded into memory via the UART. The audio is played using the PWM that is connected to the audio plug so no external hardware is required other than the serial cable (http://www.adafruit.com/products/954). This was developed in the raspberrypi B model and has only been tested there but it may work with A and B+ as long as they have the same hardware configuration for the audio jack.
The code structure and some routines were taken from a bootloader from an opencores.org project called amber (an arm compatible soft core run on an FPGA) which is totally unrelated to this work.

The features supported are:
*	Plays any wav with 1 or 2 channels (mono and stereo) with sample frequency of up to 48 KHz and 16 bits per sample.
*	The user interface is simple based on test in a terminal program and uses xmodem protocol to send the file (re used from the bootloader code)
*	Audio is played using the PWM module and 2 DMA channels of the BCM2835 that work on 2 different memory buffers.
*	10 bit resolution for the PWM.
*	Audio streams with sampling rates of 48 KHz, 44.1 KHz and 32 KHz and any non-standard frequency are played with no modification.
*	Audio steams that can be represented as the divided by 2 or 4 of 48 KHz, 44.1 KHz and 32 KHz are interpolated.
*	32 coefficient FIR filter available for all the standard rates (48 KHz, 44.1 KHz, 32 KHz and divided by 2 and 4 of these).


## PWM Audio

Raspberry pi doesn’t have digital to analog converter but it is equipped with a PWM that can do a similar job for some applications. A typical voltage DAC will create a voltage proportional to the binary code, so if the binary code is 0 it will be 0V and if it is the max number representable in the given bit range the range (1023 in 10 bits for instance) it will be the supply voltage (3.3 V for instance). If for instance if the binary code is 512 in 10 bits then the output will be 1.65 V using the same scenario as the example.

The PWM is a digital signal and can go only from 0 to VCC. Typically it produces a square wave at a fixed frequency and on each period the duty cycle will be proportional to the binary number that is the input to the PWM.

Let’s take as example a sequence of numbers [0 2 5 6 7 6 5 2 0 -3 -6 -7 -8 -7 -6 -3] on a 4 bit resolution number. The sequence looks graphically like this:


![Sine Wave](http://www.songsofthecosmos.com/images/sine_wave.gif)
 
Which is very close to a sine wave. If we input the same sequence to the PWM it will look like this:

![PWM Sine Wave](http://www.songsofthecosmos.com/images/sine_wave.gif)

The final wave doesn’t look like a sine wave but it actually sounds like it. The human ear is susceptible to frequencies and the square wave produced by the PWM still has the frequency information represented by those numbers. If we could get a frequency graph of the previous example it will look like this:
 
There will be a frequency component on the original frequency that those numbers sine wave represents and there will be a huge component on the PWM frequency (how often the PWM gives a new sample). Still looks noisy though.
Two other factors that help us is that there is an analog filter at the output of the PWM which will attenuate high frequencies and he fact that the human can only listen frequency ranges between 20 Hz and 20 KHz.
 
The higher the PWM frequency the better quality sound at the output. On the other hand if the PWM frequency is inside the human range then it will be perceptible in the audio. This code only uses 32 KHz, 44.1 KHz and 48 KHz PWM frequencies which are decently above the human range.


## Dual audio buffering

In general the way this works is by using two identical buffers in memory and 2 dma channels. One buffer s being streamed to the PWM while the other one is being filled with processed data. The basic operations done on the data are arrangement of samples, interpolation, filtering, formatting data for PWM and setting up of the DMA transfer.
In the code this dual buffer is declared as a bidimensional array:

```c
uint32_t sample_buffer[2][SAMPLE_BUFF_SIZE];
```
The main loop keeps track of the current buffer using buffer_num which toggles on each data processing loop. There will be a few variables, arrays and pointers declared as bidimensional arrays too and those will be accessed only when the given buffer number is selected.
The idea is to process and arrange the data while some audio is already being played in the background. By the time the current DMA stream to the PWM finishes the next buffer and DMA channels are ready to start streaming. Thanks to the fact that the PWM also has a FIFO this is heard as a non-interrupted audio stream. The following diagram tries to show overtime how the processing is divided between buffers. Dark regions use the processor and light regions is the DMA streaming to the PWM, arrows try to show how the program flow would jump from one buffer to the other one.

![Dual Buffer](http://www.songsofthecosmos.com/images/sine_wave.gif)
 
Note that there is idle time after the DMA setup. That’s the time between finishing the data processing and the other DMA finishing streaming. If the idle time is negative then the audio stream will be interrupted this will translate in really noisy and inaccurate stream. Some of the processing in the loop is proportional to the number of samples (like FIR filtering) and some is a constant (like DMA setup). To reduce the effect of the constant processing time the size of buffer can be increased.

## Interpolation

In order to improve audio quality the code converts all the audio streams that use a lower standard frequencies to either 32 KHz, 44.1 KHz or 48 KHz by doing linear interpolation of the current sample and the previous sample.
So for instance a 16KHz audio stream will be raised to 32Khz. So now for one sample of the original file we need 2. The sample that we currently lack will be calculated as the midpoint between the 2 samples.
The following diagram shows the relationship between a sample that comes from the file and a sample to be filtered/streamed in case of the base frequencies 32 KHz, 44.1 KHz or 48 KHz. In this case there is a 1:1 relationship of sample from file and sample streamed to the pwm.

 
The next one shows what happens with 22050 Hz and 16000 Hz which ae promoted to 44.1 KHz and 32 KHz by calculating an intermediate sample being the middle point of the 2. There is a 2:1 relationship between the samples streamed and the samples in the file.
 
Similarly for 11025 Hz and 8000 Hz promoted to 44.1 KHz and 32 KHz respectively. In this case there is a 4:1 relationship and the 2 extra samples are calculated recursively as the midpoint between the previous and middle sample and the middle sample and the current sample:
 
The quality of the audio gets improved with this because the output wave is smoother and the frequency of the PWM is taken out of the audible range (8 KHz and 11025 Hz are inside the range). Another advantage of interpolation is we need less sets of coefficients for the filters, since they are dependent on sampling rate.

## Data arrangement

There is still more processing done before putting the sample in the buffer but it is simpler to explain it after interpolation. In the play_audio routine there can be found 3 nested loops using index variables i, j and k. Index i will iterate on a pair of samples, j will select left or right and k will select the interpolated sample. The arrangement of the samples in the sample_buffer obeys the following relationship:

```c
sample_buffer[buffer_num][((i*audio_info.inter+k)*2)+j] = curr_sample32 ;
```

Graphically this is how it looks:

 
## Equalization
The code supports FIR filtering using some presets. It is based on a 32 coefficient FIR filter, the samples and coefficients are interpreted as a 16 bit fixed point numbers. There is one filter per base frequency (32 KHz, 44.1 KHz and 48 KHz) so in the fir.h file 3 sets of coefficients will be found per filter.

The low pass filters have a cut off frequency of 500 Hz and the high pass filters have a cut off frequency of 2000 Hz. The coefficients were generated from this page (Which really gave 31)
http://www.arc.id.au/FilterDesign.html


## Usage

Substitute the kernel.img file in the sd card of your raspberry pi with the one in this project. If you are unfamiliar with how to do this read this tutorial from the section “Raspberry-Pi Boot Process”:
http://www.valvers.com/open-software/raspberry-pi/step01-bare-metal-programming-in-cpt1/
Once the raspberrypi is powered up and connected to a terminal program with a baud rate of 115200bps it should show a simplistic user interface with the following options:
Hit the ‘l’ to load the audio, once you do that the program will be waiting for the audio to be sent using the xmodem protocol. I use teraterm as my terminal program but make sure yours have a way to send it with yours. After the long wait (It is almost 2 min per megabyte) some information about the audio will appear on the screen (Or problems with the format if unsuccessful).
Once loaded hit on p to play. You can change the equalizer setting by hitting e before playing the audio.

## Hardware resources used.

This program tries to touch on many peripherals of the raspberry pi. The documentation for the peripherals can be found here:
https://www.raspberrypi.org/documentation/hardware/raspberrypi/bcm2835/BCM2835-ARM-Peripherals.pdf
The document is full of typos and sometimes is inaccurate so I based a lot of the configuration on other peoples work. The code tries to use defines as much as possible every time a peripheral control register is programmed. Most of the configuration can be understood by looking at the dma, pwm, gpio and system timer chapters. The only obscure configuration is setting up the clock for the PWM and something can be found in “6.3 General Purpose GPIO Clocks”.


## Future improvements:
- [ ] Using tightly coupled memory to store the samples and coefficients for FIR speed improvement.
- [ ] Using DMA interrupts instead of polling the end bit.
- [ ] Adding support for > 48KHz frequency with oversampling/interpolation of the current base frequencies.
- [ ] Compressed audio format and reading from sd card (Probably those will be different future projects)

