import subprocess
import os
import numpy as np
from math import log2

sequence_result={}
matrix=[]
languages=[]
compile_command = ["g++", "src/lang.cpp", "-O3", "-o", "bin/lang"]
subprocess.run(compile_command, check=True)

# Execute the compiled program
for file_name in os.listdir("examples/language"):
    if "ipb_" + file_name + "_mixedsample.txt"  not in os.listdir("examples/preprocess"):
        execution_command = ["./bin/lang", "examples/language/" + file_name, "examples/mixedsample.txt"]
        subprocess.run(execution_command, check=True)
        languages.append(file_name.split(".")[0])
        with open( "examples/preprocess/mixedsample/ipb_" + file_name.split(".")[0] + "_mixedsample.txt", 'r') as f:
            data = f.read().split('\n')
            matrix.append([float(i) for i in data if i != ''])


i=1
min = 100
for array in matrix:
    if array[i]<min:
        min = array[i]
        current_lang = matrix.index(array)
sequence_result[current_lang]=[0]
while i < len(matrix[0]):
        if matrix[current_lang][i] > log2(matrix[current_lang][0]):
            previous_lang = current_lang
            for array in matrix:
                if array[i]<min:
                    min = array[i]
                    current_lang = matrix.index(array)
            if previous_lang != current_lang:
                sequence_result[previous_lang].append(i-1)
                if current_lang in sequence_result:
                    sequence_result[current_lang].append(i)
                else:
                    sequence_result[current_lang]=[i]
            i+=1

print(sequence_result)

with open("examples/mixedsample.txt", "r") as file:
    file_content = file.read()

for language in sequence_result:
    for i in range(0, len(sequence_result[language])-1, 2):
        print(file_content[sequence_result[language][i]:sequence_result[language][i+1]])
    

# # Define the filter parameters
# fs = 100  # Sampling frequency (Hz)
# cutoff = 10  # Cutoff frequency (Hz)
# order = 2  # Filter order

# # Calculate the filter coefficients using a Butterworth filter
# b, a = butter(order, cutoff / (fs / 2), btype='low')

# # Apply the filter using the filtfilt function
# y1 = filtfilt(b, a, data)