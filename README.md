# perceptron
C++ implementation of perceptron classifier using SFML

Requirements: 1) C++11, 2) SFML 2.4.2

The program was tested on Ubuntu 16.04 LTS Xenial, gcc 5.4.0.

Instructions:

1) Update the makefile with the correct location of SFML_DIR (that is, the directory where SFML is located on your computer).
2) Build the project, using "make" command in the terminal.
3) Launch the executable file "perceptron".
4) Keyboard controls. Press "G" to generate new training set, press "T" to perform one iteration of training (some training sets require more than one iteration to complete classification, so in these cases hit the "T" key until the classification is done).


This is a simple implementation of the single-layer perceptron classifier. It classifies the set of training points on the 2-D plane into two classes. Points belonging to the first class are colored red and to second class are colored blue. Generation of training set is guaranteed to produce linearly separable classes of points. So the perceptron will always converge to the solution and classify input data after the finite number of iterations. The learning is done in steps (iterations). After each iteration the linear boundary is updated until all points are correctly classified. The solution found is one of possibly many solutions. Generating algorithm can be changed to produce linearly non-separable training set leading to situations when classification is not possible.

If you want to learn more about perceptron please refer to the following wikipedia article https://en.wikipedia.org/wiki/Perceptron
