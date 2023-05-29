# TAI_LAB02

## Lang program

### How to compile

```bash
g++ -O3 -o bin/lang src/lang.cpp
```
### How to run

Below are the parameters to run the program.
All parameters are optional except the path to the file.

```bash
./bin/cpm -k 4 -n 2 -M 5 -m 0 -a 1 -t 0.5 -i -s <model name> -p <model name> -r <path to ipb file> <path to reference> <path to target>  
```
- parameter k: Size of the windows and anchors; default value is 4
- parameter n: Number of anchors; default value is -1, which means all anchors will be used
- parameter M: Maximum number of symbols for the anchor to evaluate; default value is INFINITY
- parameter m: Minimum number of symbols for the anchor to evaluate; default value is 0
- parameter a: alpha; smoothing parameter; default value is 1
- parameter t: cutoff threshold; default value is 0.5
- parameter i: ignored last; if last guess was ignored
- parameter s: save; save the model
- parameter p: load the model
- parameter r: path for saving the ipb (info per byte) file
- parameter <path to reference file>: path to the reference file for the program to process 
- parameter <path to target file>: path to the target file for the program to process 

### Example

```bash
./bin/lang example/language2/english.utf8 example/en_sample.txt
```

## Findlang program

### How to compile

```bash
g++ -O3 -std=c++17 -o bin/findlang src/findlang.cpp
```

### How to run

```bash
./bin/cpm_gen <path to multiple language files>   <sample text> 
```

### Example

```bash
./bin/cpm_gen example/language2 example/en_sample.txt

```

## LocateLang program

### How to run

You can run using a python compiler.
