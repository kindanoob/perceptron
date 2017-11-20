#include <iostream>
#include <cmath>
#include <random>


#include "perceptron.h"

GraphPlotter::GraphPlotter() {
	init_axes();
	init_grid();
	init_weights();
	init_training_data();
	update_function_data();	
	init_window();
	
}

GraphPlotter::~GraphPlotter() {
	delete window_;
}

void GraphPlotter::init_window() {
	sf::ContextSettings settings;
    settings.antialiasingLevel = ANTIALIASING_LEVEL;
	window_ = new sf::RenderWindow(sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, 
		SCREEN_HEIGHT_IN_PIXELS), WINDOW_TITLE, sf::Style::Default, settings);
	window_->setVerticalSyncEnabled(true);
}

void GraphPlotter::init_axes() {
	sf::Vertex *vertex = new sf::Vertex(sf::Vector2f(0, ORIGIN_Y), AXES_COLOR);
	x_axis_.push_back(*vertex);
	x_axis_.push_back(sf::Vertex(sf::Vector2f(SCREEN_WIDTH_IN_PIXELS, ORIGIN_Y), AXES_COLOR));
	y_axis_.push_back(sf::Vertex(sf::Vector2f(ORIGIN_X, 0), AXES_COLOR));
	y_axis_.push_back(sf::Vertex(sf::Vector2f(ORIGIN_X, SCREEN_HEIGHT_IN_PIXELS), AXES_COLOR));
}


void GraphPlotter::init_grid() {
	int y = ORIGIN_Y - GRID_STEP_IN_PIXELS;
	while (y >= 0) {
		sf::Vertex up_first(sf::Vector2f(0, y), GRID_COLOR);
		sf::Vertex up_second(sf::Vector2f(SCREEN_WIDTH_IN_PIXELS, y), GRID_COLOR);
		std::vector<sf::Vertex> up_v;
		up_v.push_back(up_first);
		up_v.push_back(up_second);
		horizontal_grid_.push_back(up_v);
		sf::Vertex down_first(sf::Vector2f(0, SCREEN_HEIGHT_IN_PIXELS - y), GRID_COLOR);
		sf::Vertex down_second(sf::Vector2f(SCREEN_WIDTH_IN_PIXELS, SCREEN_HEIGHT_IN_PIXELS - y), GRID_COLOR);
		std::vector<sf::Vertex> down_v;
		down_v.push_back(down_first);
		down_v.push_back(down_second);
		horizontal_grid_.push_back(down_v);
		y -= GRID_STEP_IN_PIXELS;
	}
	//init vertical grid
	int x = ORIGIN_X - GRID_STEP_IN_PIXELS;
	while (x >= 0) {
		sf::Vertex left_first(sf::Vector2f(x, 0), GRID_COLOR);
		sf::Vertex left_second(sf::Vector2f(x, SCREEN_HEIGHT_IN_PIXELS), GRID_COLOR);
		std::vector<sf::Vertex> left_v;
		left_v.push_back(left_first);
		left_v.push_back(left_second);
		vertical_grid_.push_back(left_v);
		sf::Vertex right_first(sf::Vector2f(SCREEN_WIDTH_IN_PIXELS - x, 0), GRID_COLOR);
		sf::Vertex right_second(sf::Vector2f(SCREEN_WIDTH_IN_PIXELS - x, SCREEN_HEIGHT_IN_PIXELS), GRID_COLOR);
		std::vector<sf::Vertex> right_v;
		right_v.push_back(right_first);
		right_v.push_back(right_second);
		vertical_grid_.push_back(right_v);
		x -= GRID_STEP_IN_PIXELS;
	}
}

void GraphPlotter::draw_axes() {
	window_->draw(&x_axis_[0], x_axis_.size(), sf::Lines);
	window_->draw(&y_axis_[0], y_axis_.size(), sf::Lines);
}
void GraphPlotter::draw_grid() {
	for(const auto& curr: vertical_grid_){
		window_->draw(&curr[0], curr.size(), sf::Lines);
	}
	for(const auto& curr: horizontal_grid_){
		window_->draw(&curr[0], curr.size(), sf::Lines);
	}
}

double GraphPlotter::f(double x) {	
	return (-weights_[1] / weights_[2] * x - weights_[0] / weights_[2]);
}

void GraphPlotter::update_function_data() {
	if(!function_data_.empty()){
		function_data_.clear();
	}
	double curr_x = 0;
	double curr_y = f(curr_x / GRID_STEP_IN_PIXELS) * GRID_STEP_IN_PIXELS;
	sf::Vertex prev_right(sf::Vector2f(curr_x + ORIGIN_X, curr_y + ORIGIN_Y), PLOT_COLOR);
	sf::Vertex prev_left(sf::Vector2f(ORIGIN_X - curr_x, curr_y + ORIGIN_Y), PLOT_COLOR);
	curr_x += PLOT_STEP_IN_PIXELS;
	while (curr_x <= SCREEN_WIDTH_IN_PIXELS / 2) {
		curr_y = f(curr_x / GRID_STEP_IN_PIXELS) * GRID_STEP_IN_PIXELS;
		sf::Vertex curr_right(sf::Vector2f(curr_x + ORIGIN_X, curr_y + ORIGIN_Y), PLOT_COLOR);
		std::vector<sf::Vertex> v1;
		v1.push_back(prev_right);
		v1.push_back(curr_right);
		
		function_data_.push_back(v1);
		curr_y = f(-curr_x / GRID_STEP_IN_PIXELS) * GRID_STEP_IN_PIXELS;
		sf::Vertex curr_left(sf::Vector2f(ORIGIN_X - curr_x, curr_y + ORIGIN_Y), PLOT_COLOR);
		std::vector<sf::Vertex> v2;
		v2.push_back(prev_left);
		v2.push_back(curr_left);
		function_data_.push_back(v2);
		curr_x += PLOT_STEP_IN_PIXELS;
		prev_left = curr_left;
		prev_right = curr_right;
	}
}


void GraphPlotter::init_training_data() {
	training_data_red_.clear();
	training_data_blue_.clear();
	for (int i = 0; i < SAMPLE_SIZE; ++i) {
		int rand_x = rand() % 20;
		int rand_y = rand() % 20;
		int sign = ((rand() % MODULO) - (MODULO / 2)) > 0 ? 1: -1;
		training_data_red_.push_back({1, rand_x, sign * rand_y, LABEL_PLUS});
		rand_x = rand() % 20;
		rand_y = rand() % 20;
		sign = ((rand() % MODULO) - (MODULO / 2)) > 0 ? 1: -1;
		training_data_blue_.push_back({1, -rand_x, sign * rand_y, LABEL_MINUS});
	}	
}

void GraphPlotter::plot_training_data() {
	for (const auto& curr: training_data_red_) {
		sf::RectangleShape rect;
		rect.setSize(sf::Vector2f(SQUARE_SIDE, SQUARE_SIDE));
		rect.setOrigin(sf::Vector2f(SQUARE_SIDE / 2, SQUARE_SIDE / 2));
		rect.setPosition(sf::Vector2f(curr[1] * GRID_STEP_IN_PIXELS + ORIGIN_X, 
			curr[2] * GRID_STEP_IN_PIXELS + ORIGIN_Y));
		rect.setFillColor(sf::Color(255, 0, 0));
		window_->draw(rect);
	}
	for (const auto& curr: training_data_blue_) {
		sf::CircleShape circle;
		circle.setRadius(CIRCLE_RADIUS);
		circle.setOrigin(sf::Vector2f(CIRCLE_RADIUS, CIRCLE_RADIUS));
		circle.setPosition(sf::Vector2f(curr[1] * GRID_STEP_IN_PIXELS + ORIGIN_X, 
			curr[2] * GRID_STEP_IN_PIXELS + ORIGIN_Y));
		circle.setFillColor(sf::Color(0, 0, 255));
		window_->draw(circle);
	}
}

void GraphPlotter::plot_function() {
	for (const auto& curr: function_data_) {
		window_->draw(&curr[0], curr.size(), sf::Lines);
	}
}

void GraphPlotter::init_weights() {
	weights_.clear();
	for (int i = 0; i < NUM_DIMENSIONS; ++i) {
		int rand_val = (rand() % MODULO) - (MODULO / 2);
		double curr = rand_val / (MODULO / 2.0);
		weights_.push_back(curr);
	}
	print_weights();
}

void GraphPlotter::print_weights() {
	std::cout << "print weights: " << std::endl;
	for(const auto& curr: weights_){
		std::cout << curr << ", ";
	}
	std::cout << std::endl;
}

void GraphPlotter::train_perceptron() {
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		for (auto &curr: training_data_red_) {			
			double eval = 0;
			for (int j = 0; j < NUM_DIMENSIONS; ++j) {
				eval += weights_[j] * curr[j];
			}
			double output = (eval > 0) ? LABEL_PLUS : LABEL_MINUS;
			for (int j = 0; j < NUM_DIMENSIONS; ++j) {				
				weights_[j] += LEARNING_RATE * (curr[3] - output) * curr[j];				
			}
		}
	}
	print_weights();
	for (int i = 0; i < NUM_ITERATIONS; ++i) {
		for (auto &curr: training_data_blue_) {
			double eval = 0;
			for (int j = 0; j < NUM_DIMENSIONS; ++j) {
				eval += weights_[j] * curr[j];
			}
			double output = (eval > 0) ? LABEL_PLUS : LABEL_MINUS;
			for (int j = 0; j < NUM_DIMENSIONS; ++j) {				
				weights_[j] += LEARNING_RATE * (curr[3] - output) * curr[j];
			}
		}
	}
}

void GraphPlotter::run() {
	while (window_->isOpen()) {
		sf::Event e;
		while (window_->pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				window_->close();
			}
			if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::Escape) {
					window_->close();
				}
			}
			if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::T) {
					train_perceptron();
					update_function_data();
					//print_weights();
				}
			}
			if (e.type == sf::Event::KeyPressed) {
				if (e.key.code == sf::Keyboard::G) {
					init_weights();
					init_training_data();
					update_function_data();
					//print_weights();
				}
			}
		}
		window_->clear(sf::Color(255, 255, 255));
		draw_grid();
		draw_axes();		
		plot_training_data();		
	    plot_function();
		window_->display();
	}
}
