#include <iostream>
#include <ctime>
#include <random>
#include <memory>


#include "perceptron.h"


int main() {
	srand(time(nullptr));
	std::unique_ptr<Perceptron> p(new Perceptron());
	p->Run();
	return 0;
}
