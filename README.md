# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is used to perform hierarchical clustering over particularly large data-sets.

## Requirements
* gcc

## Compilation

To compile, execute each one of the following commands from the root of the project:

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

[6] - Number of Features: An integer value.

[7] - Ignore Last Column of the Dataset: 1 for yes, 0 for no.
~~~

Example of valid command to run the program:

```
./main 100 0.8 1 IRIS.csv 4 1 ,\r\n
```

Once the execution finishes, a `cbirch_output.csv` file will be created with the result of the clustering.


## Dataset file format

Checkout the `IRIS.csv` file in the root of the project for more informations on the format of the dataset file.

## More

In order to test our implementation of BIRCH we compared it with Roberto Perdisci's implementation, called [jbirch](https://github.com/perdisci/jbirch).

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

