# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is used to perform hierarchical clustering over particularly large data-sets.

## Requirements
* gcc

## Compilation

To compile, execute the following command from the root of the project:

```
gcc *.c -o main -lm
```

## How to run

After compiling it, you can run the algorithm by executing the `main` binary with the following arguments, which must be informed in this order and separated by whitespace:

~~~
[1] - Branching Factor: An integer value.

[2] - Threshold: A float value.

[3] - Apply Merging Refinement: 1 for yes, 0 for no.

[4] - Dataset File Path: String.

[5] - Dataset Delimiters: String.

[6] - Ignore Last Column of the Dataset: 1 for yes, 0 for no.
~~~

Example of valid command to run the program:

```
./main 100 0.8 1 IRIS.csv , 1
```
Once the execution finishes, a `cbirch_output.csv` file will be created with the result of the clustering.

## Dataset file format

Checkout the `IRIS.csv` file in the root of the project for more informations on the format of the dataset file.

## Comparation with Roberto Perdisci's jbirch

In order to test our implementation of BIRCH we compared it with Roberto Perdisci's implementation, called [jbirch](https://github.com/perdisci/jbirch).

Important to notice that in the comparation we disabled jbirch's automatic rebuilding feature as it isn't supported by our implementation.

The comparation was made using the `covtype.csv` dataset, which is avaiable in this [branch of jbirch](https://github.com/douglas444/jbirch) together with instructions on how to run jbirch over the dataset. Following we have the equivalent command lines to run the same experiment with our implementation (edit the dataset file path as needed):

```
./main5 0.5 1 covtype.csv , 1
```

Both implementation should output the same results.
