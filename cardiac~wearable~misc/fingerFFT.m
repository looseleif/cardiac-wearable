% `% Signal Setup

t = linspace(0, 1, 1000);

X = csvread('finger_print.csv')
X = abs(X)
X = X-(sum(X)/length(X))
X = flip(X)

plot(X)
title('Time Domain PPG Signal')
xlabel('AU Time Scale')
ylabel('Magnitude of Reflected Light (NORMALIZED AC)')

% Filtering Operation (Butterworth)

fs = 1000
X = 0:1000
% Choose a Butterworth filter design
filterOrder = 4; % Filter order
fc = 80; % Cutoff frequency (Hz)

% Compute the filter coefficients
[b, a] = butter(filterOrder, fc/(fs/2), 'low');

% Apply the filter to the signal
X = filter(b, a, X);

plot(X)
title('Time Domain PPG Signal (FILTERED)')
xlabel('AU Time Scale')
ylabel('Magnitude of Reflected Light (NORMALIZED AC)')

FFT of Filter Signal

X = fft(X)

magX = abs(X);
N = length(X);
df = 1/(t(2)-t(1));
faxis = linspace(-df/2, df/2, N);

% Plotting of FFT

plot(faxis, fftshift(magX))
title('FFT of PPG Data (FILTERED)')
xlabel('Frequency (Hz)')
ylabel('Magnitude')
