import subprocess
import os
import numpy as np
from math import log2
import matplotlib.pyplot as plt
from scipy.signal import butter, filtfilt

saved = False
if not saved:
    sequence_result={}
    matrix=[]
    languages=[]
    compile_command = ["g++", "src/lang.cpp", "-O3", "-o", "bin/lang"]
    subprocess.run(compile_command, check=True)

    # Execute the compiled program
    for file_name in os.listdir("examples/language"):
        if "ipb_" + file_name + "_mixedsample.txt"  not in os.listdir("examples/preprocess"):
            execution_command = ["./bin/lang", "-a", "0.1",  "-t", "0.8", "-n", "30", "-r", "ipb_" + file_name.split(".")[0] + "_mixedsample.txt", "examples/language/" + file_name, "examples/mixedsample.txt"]
            subprocess.run(execution_command, check=True)
            languages.append(file_name.split(".")[0])
            with open( "ipb_" + file_name.split(".")[0] + "_mixedsample.txt", 'r') as f:
                data = f.read().split('\n')
                data = [float(i) for i in data if i != '']
                data = [file_name.split(".")[0]] + data
                print(len(data))
                matrix.append(data)


    matrix = np.array(matrix)

    # save the matrix
    np.save("matrix.npy", matrix)

else:
    matrix = np.load("matrix.npy")

#print(matrix)
#print(matrix.shape)
languages = matrix[:,0]
asizes = matrix[:,1].astype(float)
matrix = matrix[:,2:].astype(float)
#print(asizes)
threshold = np.log2(asizes)/2
#print(threshold)

fs = 100  # Sampling frequency (Hz)
cutoff = 5  # Cutoff frequency (Hz)
order = 2  # Filter order

# Calculate the filter coefficients using a Butterworth filter
b, a = butter(order, cutoff / (fs / 2), btype='low')

# Apply the filter using the filtfilt function
matrix1 = filtfilt(b, a, matrix)

def get_row_index_of_min(matrix):
    return np.argmin(matrix, axis=0)


row_indices = get_row_index_of_min(matrix)
row_indices1 = get_row_index_of_min(matrix1)
#print(row_indices)
#print(row_indices1)

# get the values of the matrix where the row index is the minimum
min_values = matrix[row_indices, np.arange(matrix.shape[1])]
min_values1 = matrix1[row_indices1, np.arange(matrix1.shape[1])]
values = []
values1 = []

for i in range(row_indices.shape[0]):
    if min_values[i] > threshold[row_indices[i]]:
        values.append("None")
        row_indices[i] = -1
    else:
        values.append(languages[row_indices[i]])

for i in range(row_indices.shape[0]):
    if min_values1[i] > threshold[row_indices1[i]]:
        values1.append("None")
        row_indices1[i] = -1
    else:
        values1.append(languages[row_indices1[i]])
values = np.array(values)
values1 = np.array(values1)
#print(values)
#print(row_indices.shape)
print(np.sum(values != "None"))
print("Accuracy: ", np.sum(values != "None")/values.shape[0])

print(np.sum(values1 != "None"))
print("Accuracy: ", np.sum(values1 != "None")/values1.shape[0])


with open(  "examples/mixedsample.txt", 'r') as f:
    # iterate over every character in the file
    file_content = f.read()
    i = 0
    color_scheme = [
        "\033[0;31m", # red
        "\033[0;32m", # green
        "\033[0;33m", # yellow
        "\033[0;34m", # blue
        "\033[0;35m", # purple
        "\033[0;36m", # cyan
        '\033[94m',   # light blue      
        '\033[95m',   # light purple      
        '\033[96m',   # light cyan  
        '\033[33;1m',   # magenta
        '\033[92m',   # light green    
        "\033[0;37m", # white
    ]    

    for idx in range(len(languages)):
        print(color_scheme[idx], languages[idx])
    print(color_scheme[-1], "None")

    for char in file_content:
        print(f"{color_scheme[row_indices[i]]}{char}", end="")
        i += 1
    print()
    i = 0
    for char in file_content:
        print(f"{color_scheme[row_indices1[i]]}{char}", end="")
        i += 1
    print()




# make the plot prettier
plt.style.use('ggplot')
plt.rcParams['figure.figsize'] = (20, 10)
fig, axes = plt.subplots(6, 2, figsize=(15, 8))

# Flatten the axes array to make it easier to work with
axes = axes.flatten()
#plt.figure(figsize=(50, 10))
for i in range(matrix.shape[0]):
    data = matrix[i,:]

    # Calculate the filter coefficients using a Butterworth filter
    b, a = butter(order, cutoff / (fs / 2), btype='low')

    # Apply the filter using the filtfilt function
    y1 = filtfilt(b, a, data)

    axes[i].plot(y1)
    axes[i].plot(threshold[i]*np.ones(y1.shape))
    axes[i].plot(data)
    axes[i].set_title("Language: " + languages[i])
plt.tight_layout()
plt.show()