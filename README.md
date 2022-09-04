# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is used to perform hierarchical clustering over particularly large data-sets.

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
gcc  -o ./main ./main.o ./smem.o ./birch.o ./array.o -s -lm
```

## How to run

After compiling it, you can run the algorithm by executing the `main` binary with the following arguments, which must be informed in this order and separated by whitespace:
~~~
1º - branching_factor: integer value
2º - threshold: float value
3º - apply_merging_refinement: 0 or 1
4º - file_path: path to the dataset file
5º - number_of_features: integer value
6º - last_column_is_label: 0 or 1
~~~
Example of valid command to run the program:

```
./main 100 0.8 1 IRIS.csv 4 1
```

Once the execution finishes, a `output.csv` file will be created with the result of the clustering.


## Dataset file format

Checkout the `IRIS.csv` file in the root of the project for more informations on the format of the dataset file.
A detail to consider is that any carriage return (\r) or new line (\n) will be considered end of the line.

## This project was tested following any known implementation of the algorithm?

Yes. In order to test our implementation of BIRCH we compared it with Roberto Perdisci's implementation, called [jbirch](https://github.com/perdisci/jbirch).

Important to notice that in the comparation we disabled jbirch's automatic rebuilding feature as it isn't supported by our implementation.

Here's a [branch of jbirch](https://github.com/douglas444/jbirch) with instructions on how to run jbirch over the IRIS.csv dataset. Following we have the equivalent command lines to run the same experiment with our implementation:

Command line that covers the case in which nodes cannot be merged into one in the mergingRefinement:

```
./main 5 0.5 1 IRIS.csv 4 1
```

Command line that covers the case in which nodes cannot be merged into one in the mergingRefinement:
```
./main 5 0.8 1 IRIS.csv 4 1
```

