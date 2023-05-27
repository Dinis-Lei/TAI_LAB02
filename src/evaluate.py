import subprocess
import os
import numpy as np


n_hits = 0
confidences = []
count = 0
n_files = len(os.listdir("examples/test_target2"))

for file_name in os.listdir("examples/test_target2"):
    
    prefix = file_name[:2]
    count += 1
    print(f"{count} out of {n_files}", end="\r")
    execution_command = ["./bin/findlang", "examples/test_target2/" + file_name]
    subprocess.run(execution_command, check=True, stdout=subprocess.PIPE, universal_newlines=True)

    # get output from subprocess
    output = subprocess.check_output(execution_command)
    output = output.decode("utf-8")
    #print(output)

    # loop through the output in reverse order
    hit_value = 1
    for line in output.splitlines()[::-1]:
        if "The most provable languages are :" in line:
            hit_value = 0

        if prefix == "pt" and "portugues" in line:
            n_hits += hit_value
            confidences.append(float(line.split(" ")[-1]))
            break
        if prefix == "en" and "english" in line:
            n_hits += hit_value
            confidences.append(float(line.split(" ")[-1]))
            break
        if prefix == "es" and "spanish" in line:
            n_hits += hit_value
            confidences.append(float(line.split(" ")[-1]))
            break
        if prefix == "gr" and "greek" in line:
            n_hits += hit_value
            confidences.append(float(line.split(" ")[-1]))
            break
        if prefix == "la" and "latin" in line:
            n_hits += hit_value
            confidences.append(float(line.split(" ")[-1]))
            break

print("\n")
confidences = np.array(confidences)
print("n_hits: ", n_hits)
print("count: ", count)
print("Accuracy: ", n_hits / count)
print(confidences)
print("Average confidence: ", confidences.mean())
