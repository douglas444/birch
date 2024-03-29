# BIRCH

BIRCH (Balanced Iterative Reducing and Clustering Using Hierarchies) is an unsupervised data mining algorithm which is 
used to perform hierarchical clustering over particularly large data-sets.

## To-do
- [x] Compare with jbirch's [results](https://github.com/douglas444/jbirch/tree/reference-results);
- [ ] Fix memory leaks;
- [ ] Make array.c more memory efficient by allowing setting the size of the elements;
- [ ] Analyze the ideal size for every Array initialization;
- [ ] Implement a more efficient way of reading csv files (intance.c);
- [ ] Make sure that [jbirch](https://github.com/perdisci/jbirch) is correct, since it is being used as reference for 
this implementation;
- [ ] Implement jbirch's automatic rebuilding feature.
- [ ] Output the entire tree, not only the leafs.

## Requirements
* gcc

## How to Compile

To compile, execute the following command line from the root of the project:

```
gcc ./src/main.c ./src/*/*.c -o ./bin/main -lm
```

## How to Run

After compiling it, you can run the algorithm by executing the `./bin/main` binary with the following arguments, which 
must be informed in this order and separated by whitespace:

1. Branching Factor: An integer value;
2. Threshold: A float value;
3. Apply Merging Refinement: 1 for yes, 0 for no;
4. Dataset File Path: String;
5. Dataset Delimiters: String;
6. Ignore Last Column of the Dataset: 1 for yes, 0 for no;
7. Output File Name: String.

Example of valid command line to run the program (execute it from the root of the project):

```
./bin/main 100 0.8 1 IRIS.csv , 1 output.csv
```

Once the execution finishes, the output file containing the result of the clustering will be created with the name 
specified in the "Output File Name" argument. The output file contains the cluster identifier of each instance,
in the same order that the instances appears in the input file.

## Dataset File Format

The program takes as input a `csv`. The column's delimiter is passed as an argument as described in the `How to Run` 
section. Labels can only appear in the last column of the dataset. Make sure to set the 
"Ignore Last Column of the Dataset" accordingly.

## Comparison with Roberto Perdisci's jbirch

In order to test our implementation of BIRCH we compared it with Roberto Perdisci's implementation, called 
[jbirch](https://github.com/perdisci/jbirch). The comparison was made using the 
[covtype.csv](https://github.com/douglas444/arm-stream-framework/blob/main/arm-stream-exp/src/main/resources/covtype.csv) 
dataset, which is able to cover all the relevant code from both implementations. Important to notice that in the 
comparison we disabled jbirch's automatic rebuilding feature as it isn't supported by our implementation yet.

To reproduce the tests, follow these steps:
1. Run jbirch on the [covtype.csv](https://github.com/douglas444/arm-stream-framework/blob/main/arm-stream-exp/src/main/resources/covtype.csv) dataset by following the instructions in the `README.md` file of [this branch](https://github.com/douglas444/jbirch/tree/reference-results) of the project;
2. Run our implementation, the C implementation, over the same dataset and with the same configurations by executing 
this command line from the root of the project (edit the dataset file path as needed):
`./bin/main 5 0.5 1 covtype.csv , 1 cbirch_output.csv`;
3. Compare the output file of both implementations. For the test to be succesfull both files must have the same content.

## How to Run Valgrind's Memory Check

Valgrind must be installed in your machine.

To compile birch with ggdb3 flag execute the following command line from the root of the project:
```
gcc -ggdb3 ./src/main.c ./src/*/*.c -o ./bin/main -lm
```
To run the valgrind over birch with the 
[covtype.csv](https://github.com/douglas444/arm-stream-framework/blob/main/arm-stream-exp/src/main/resources/covtype.csv) 
dataset execute the following command line from the root of the project (edit the dataset file path as needed):
```
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./bin/main 100 0.8 1 covtype.csv , 1
```
The covtype dataset triggers all the functions of the implementation, allowing Valgrind to test every piece of code.
