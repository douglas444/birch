# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is used to perform hierarchical clustering over particularly large data-sets.

## To-do
- [x] Compare with jbirch's [results](https://github.com/douglas444/jbirch/tree/reference-results)
- [ ] Fix memory leaks
- [ ] Improve efficiency of the functions from sample.c and array.c
- [ ] Make sure that [jbirch](https://github.com/perdisci/jbirch) is correct, since it is being used as reference for this implementation
- [ ] Implement jbirch's automatic rebuilding feature

## Requirements
* gcc

## Compilation

To compile, execute the following command from the root of the project:

```
gcc *.c -o main -lm
```

## How to run

After compiling it, you can run the algorithm by executing the `main` binary with the following arguments, which must be informed in this order and separated by whitespace:

1. Branching Factor: An integer value.

2. Threshold: A float value.

3. Apply Merging Refinement: 1 for yes, 0 for no.

4. Dataset File Path: String.

5. Dataset Delimiters: String.

6. Ignore Last Column of the Dataset: 1 for yes, 0 for no.

Example of valid command to run the program:

```
./main 100 0.8 1 IRIS.csv , 1
```
Once the execution finishes, a `cbirch_output.csv` file will be created with the result of the clustering.

## Dataset file format

The program takes as input a `csv` file where new lines (\n) and carriage returns (\r) are interpreted as end of line symbols. The column's delimiter is passed as an argument as described in the `How to Run` section.
Labels can only appear in the last column of the dataset, and if they are present, the last argument of the program must be set to 1.

## Comparation with Roberto Perdisci's jbirch

In order to test our implementation of BIRCH we compared it with Roberto Perdisci's implementation, called [jbirch](https://github.com/perdisci/jbirch).

Important to notice that in the comparation we disabled jbirch's automatic rebuilding feature as it isn't supported by our implementation.

The comparation was made using the `covtype.csv` dataset. Please read the `README.md`file in this [branch of jbirch](https://github.com/douglas444/jbirch/tree/reference-results) for informations on how to obtain the dataset and run jbirch. Following we have the equivalent command lines to run the same experiment with our implementation (edit the dataset file path as needed).

With merge refinement:
```
./main 5 0.5 1 covtype.csv , 1
```

Without merge refinement:
```
./main 5 0.5 0 covtype.csv , 1
```

Both implementations should output the same results.

## How to run Valgrind's memory check

Valgrind must be installed in your machine.

Compilation:
```
gcc -ggdb3 *.c -o main -lm
```
Running the test:
```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./main 100 0.8 1 IRIS.csv , 1
```

