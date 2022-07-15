# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is used to perform hierarchical clustering over particularly large data-sets.

This is a translation of jbirch ( https://github.com/perdisci/jbirch ) from Java to C.

I ended up not needing this project, so I didn't have time to test everything. But the code is as legible as the original source, and there's no memory leaks, so it should be viable for anyone interested to continue the project.

## Requirements
* gcc

## Compilation

To compile, execute each one of the following commands from the root of the project:

```
gcc -Wall -O2  -c ./main.c -o ./main.o 
```
```
gcc -Wall -O2  -c ./array.c -o ./array.o 
```
```
gcc -Wall -O2  -c ./birch.c -o ./birch.o 
```
```
gcc -Wall -O2  -c ./smem.c -o ./smem.o 
```
```
gcc  -o ./main ./main.o ./smem.o ./birch.o ./array.o -s
```

## How to run

After compiling it, you can run the binary by executing the following command from the root of the project.
```
./main
```
