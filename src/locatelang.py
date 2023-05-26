import subprocess
import os

compile_command = ["g++", "src/lang.cpp", "-O3", "-o", "bin/lang"]
subprocess.run(compile_command, check=True)

# Execute the compiled program
for file_name in os.listdir("examples/language"):
    if file_name + "_ipb.txt" not in os.listdir("examples/preprocess"):
        execution_command = ["./bin/lang", "examples/language/" + file_name, "examples/sample1.txt"]
        subprocess.run(execution_command, check=True)

# Define the filter parameters
fs = 100  # Sampling frequency (Hz)
cutoff = 10  # Cutoff frequency (Hz)
order = 2  # Filter order

# Calculate the filter coefficients using a Butterworth filter
b, a = butter(order, cutoff / (fs / 2), btype='low')

# Apply the filter using the filtfilt function
y1 = filtfilt(b, a, data)