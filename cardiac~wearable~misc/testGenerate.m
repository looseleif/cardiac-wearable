% Signal Setup
   t = linspace(0, 1, 1000);
   fs = 1000;
   X = csvread('finger_print.csv');
   X = abs(X);
   X = X - (sum(X) / length(X));
   X = flip(X);
   figure;
   plot(X);1
   title('Time Domain PPG Signal');
   xlabel('AU Time Scale');
   ylabel('Magnitude of Reflected Light (NORMALIZED AC)');
   % Filtering Operation (Butterworth)
   
   filterOrder = 4; % Filter order
   fc = 80; % Cutoff frequency (Hz)
   % Compute the filter
   [b, a] = butter(filterOrder, fc / (fs / 2), 'low');
   % Apply the filter to the signal
   X_filtered = filter(b, a, X);
   figure;
   plot(X_filtered);
   title('Time Domain PPG Signal (FILTERED)');
   xlabel('AU Time Scale');
   ylabel('Magnitude of Reflected Light (NORMALIZED AC)');
   % Finding peaks
   [pks, locs] = findpeaks(X_filtered);
   % Calculate RR intervals
   rrIntervals = diff(locs) * (1 / fs); % Convert indices difference to time intervals (in seconds)
   % Detect atrial fibrillation and output meanRR, stdRR, and cv
   [afDetected, meanRR, stdRR, cv] = atrialFibrillationDetection(rrIntervals);
   % Display the results
   fprintf('Mean RR interval: %.4f seconds\n', meanRR);
   fprintf('Standard deviation of RR intervals: %.4f seconds\n', stdRR);
   fprintf('Coefficient of variation: %.4f\n', cv);
   if afDetected
       disp('Atrial fibrillation detected.');
   else
       disp('Atrial fibrillation not detected.');
   end