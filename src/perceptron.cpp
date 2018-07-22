#include "perceptron.h"
#include <iostream>
#include <cmath>
#include <random>
#include <utility>


Perceptron::Perceptron() {
    InitAxes();
    InitGrid();
    InitWeights();
    InitTrainingData();
    UpdateDecisionBoundaryData();   
    InitWindow();
}

Perceptron::~Perceptron() {}


sf::RenderWindow& Perceptron::Window() {
    return *window_;
}

std::vector<double>& Perceptron::Weights() {
    return weights_;
}

void Perceptron::InitWindow() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = kAntialiasingLevel;
    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(kScreenWidthInPixels, kScreenHeightInPixels), kWindowTitle, sf::Style::Default, settings);
    Window().setVerticalSyncEnabled(true);
}

void Perceptron::InitAxes() {
    x_axis_.push_back(sf::Vertex(sf::Vector2f(0, kOriginY), kAxesColor));
    x_axis_.push_back(sf::Vertex(sf::Vector2f(kScreenWidthInPixels, kOriginY), kAxesColor));
    y_axis_.push_back(sf::Vertex(sf::Vector2f(kOriginX, 0), kAxesColor));
    y_axis_.push_back(sf::Vertex(sf::Vector2f(kOriginX, kScreenHeightInPixels), kAxesColor));
}


void Perceptron::InitGrid() {
    //init horizontal grid
    int y = kOriginY - kGridStepInPixels;
    while (y >= 0) {
        sf::Vertex up_first(sf::Vector2f(0, y), kGridColor);
        sf::Vertex up_second(sf::Vector2f(kScreenWidthInPixels, y), kGridColor);
        std::vector<sf::Vertex> up_v;
        up_v.push_back(up_first);
        up_v.push_back(up_second);
        horizontal_grid_.push_back(up_v);
        sf::Vertex down_first(sf::Vector2f(0, kScreenHeightInPixels - y), kGridColor);
        sf::Vertex down_second(sf::Vector2f(kScreenWidthInPixels, kScreenHeightInPixels - y), kGridColor);
        std::vector<sf::Vertex> down_v;
        down_v.push_back(down_first);
        down_v.push_back(down_second);
        horizontal_grid_.push_back(down_v);
        y -= kGridStepInPixels;
    }
    //init vertical grid
    int x = kOriginX - kGridStepInPixels;
    while (x >= 0) {
        sf::Vertex left_first(sf::Vector2f(x, 0), kGridColor);
        sf::Vertex left_second(sf::Vector2f(x, kScreenHeightInPixels), kGridColor);
        std::vector<sf::Vertex> left_v;
        left_v.push_back(left_first);
        left_v.push_back(left_second);
        vertical_grid_.push_back(left_v);
        sf::Vertex right_first(sf::Vector2f(kScreenWidthInPixels - x, 0), kGridColor);
        sf::Vertex right_second(sf::Vector2f(kScreenWidthInPixels - x, kScreenHeightInPixels), kGridColor);
        std::vector<sf::Vertex> right_v;
        right_v.push_back(right_first);
        right_v.push_back(right_second);
        vertical_grid_.push_back(right_v);
        x -= kGridStepInPixels;
    }
}

void Perceptron::DrawAxes() {
    Window().draw(&x_axis_[0], x_axis_.size(), sf::Lines);
    Window().draw(&y_axis_[0], y_axis_.size(), sf::Lines);
}

void Perceptron::DrawGrid() {
    for (const auto& curr: vertical_grid_) {
        Window().draw(&curr[0], curr.size(), sf::Lines);
    }
    for (const auto& curr: horizontal_grid_) {
        Window().draw(&curr[0], curr.size(), sf::Lines);
    }
}

double Perceptron::F(double x) {    
    return (-weights_[1] / weights_[2] * x - weights_[0] / weights_[2]);
}

void Perceptron::UpdateDecisionBoundaryData() {
    if (!decision_boundary_data_.empty()) {
        decision_boundary_data_.clear();
    }
    double curr_x = 0;
    double curr_y = F(curr_x / kGridStepInPixels) * kGridStepInPixels;
    sf::Vertex prev_right(sf::Vector2f(curr_x + kOriginX, curr_y + kOriginY), kPlotColor);
    sf::Vertex prev_left(sf::Vector2f(kOriginX - curr_x, curr_y + kOriginY), kPlotColor);
    curr_x += kPlotStepInPixels;
    while (curr_x <= kScreenWidthInPixels / 2) {
        curr_y = F(curr_x / kGridStepInPixels) * kGridStepInPixels;
        sf::Vertex curr_right(sf::Vector2f(curr_x + kOriginX, curr_y + kOriginY), kPlotColor);
        std::vector<sf::Vertex> v1;
        v1.push_back(prev_right);
        v1.push_back(curr_right);
        
        decision_boundary_data_.push_back(v1);
        curr_y = F(-curr_x / kGridStepInPixels) * kGridStepInPixels;
        sf::Vertex curr_left(sf::Vector2f(kOriginX - curr_x, curr_y + kOriginY), kPlotColor);
        std::vector<sf::Vertex> v2;
        v2.push_back(prev_left);
        v2.push_back(curr_left);
        decision_boundary_data_.push_back(v2);
        curr_x += kPlotStepInPixels;
        prev_left = curr_left;
        prev_right = curr_right;
    }
}

//initialized the training_data_ vector with training examples of two different classes (labeled 1 and 0),
//makes sure that the training set is linearly separable
void Perceptron::InitTrainingData() {
    training_data_.clear();
    for (int i = 0; i < kSampleSize; ++i) {
        int rand_x = 1 + rand() % 20;
        int rand_y = rand() % 20;
        int sign = ((rand() % kModulo) - (kModulo / 2)) > 0 ? 1 : -1;
        training_data_.push_back(Example({1, rand_x, sign * rand_y}, 1));
        rand_x = 1 + rand() % 20;
        rand_y = rand() % 20;
        sign = ((rand() % kModulo) - (kModulo / 2)) > 0 ? 1 : -1;
        training_data_.push_back(Example({1, -rand_x, sign * rand_y}, 0));
    }   
}

void Perceptron::PlotTrainingData() {
    for (const auto& curr: training_data_) {
        if (curr.label_ == 1) {
            sf::RectangleShape rect;
            rect.setSize(sf::Vector2f(kSquareSide, kSquareSide));
            rect.setOrigin(sf::Vector2f(kSquareSide / 2, kSquareSide / 2));
            rect.setPosition(sf::Vector2f(curr.data_[1] * kGridStepInPixels + kOriginX, 
                curr.data_[2] * kGridStepInPixels + kOriginY));
            rect.setFillColor(sf::Color(255, 0, 0));
            Window().draw(rect);
        } else {
            sf::CircleShape circle;
            circle.setRadius(kCircleRadius);
            circle.setOrigin(sf::Vector2f(kCircleRadius, kCircleRadius));
            circle.setPosition(sf::Vector2f(curr.data_[1] * kGridStepInPixels + kOriginX, 
                curr.data_[2] * kGridStepInPixels + kOriginY));
            circle.setFillColor(sf::Color(0, 0, 255));
            Window().draw(circle);
        }       
    }
}

void Perceptron::PlotDecisionBoundary() {
    for (const auto& curr: decision_boundary_data_) {
        Window().draw(&curr[0], curr.size(), sf::Lines);
    }
}

void Perceptron::InitWeights() {
    weights_.clear();
    for (int i = 0; i < kNumDimensions; ++i) {
        int rand_val = (rand() % kModulo) - (kModulo / 2);
        double curr = rand_val / (kModulo / 2.0);
        weights_.push_back(curr);
    }
    PrintWeights();
}

void Perceptron::PrintWeights() {
    std::cout << "Print weights: " << std::endl;
    for (const auto& curr: weights_) {
        std::cout << curr << ", ";
    }
    std::cout << std::endl;
}

void Perceptron::TrainPerceptron() {
    for (int i = 0; i < kNumIterations; ++i) {
        for (auto &curr: training_data_) {
            double eval = 0.0;
            for (int j = 0; j < kNumDimensions; ++j) {
                eval += weights_[j] * curr.data_[j];
            }
            double predicted_label = (eval > 0) ? 1.0 : 0.0;
            double true_label = curr.label_;
            for (int j = 0; j < kNumDimensions; ++j) {              
                weights_[j] += kLearningRate * (true_label - predicted_label) * curr.data_[j];
            }
        }
    }
}

void Perceptron::Run() {
    while (Window().isOpen()) {
        sf::Event e;
        while (Window().pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                Window().close();
            }
            if (e.type == sf::Event::KeyPressed) {
                if (e.key.code == sf::Keyboard::Escape) {
                    Window().close();
                } else if (e.key.code == sf::Keyboard::T) {
                    TrainPerceptron();
                    UpdateDecisionBoundaryData();
                } else if (e.key.code == sf::Keyboard::G) {
                    InitWeights();
                    InitTrainingData();
                    UpdateDecisionBoundaryData();
                }
            }
        }
        Window().clear(kWindowClearColor);
        DrawGrid();
        DrawAxes();     
        PlotTrainingData();     
        PlotDecisionBoundary();
        Window().display();
    }
}
