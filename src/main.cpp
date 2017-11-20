#include <iostream>
#include <ctime>
#include <random>


#include "perceptron.h"


int main() {
	srand(time(nullptr));
	GraphPlotter *graph_plotter = new GraphPlotter();
	graph_plotter->run();
	delete graph_plotter;	
	return 0;
}
