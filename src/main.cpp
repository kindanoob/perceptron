#include <iostream>
#include <ctime>
#include <random>
#include <memory>

#include "perceptron.h"


int main() {
    srand(time(nullptr));
    std::unique_ptr<Perceptron> p = std::make_unique<Perceptron>();
    p->Run();
    return 0;
}
