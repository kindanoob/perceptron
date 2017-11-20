#ifndef GRAPH_PLOTTER_H_
#define GRAPH_PLOTTER_H_

#include <string>
#include <vector>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


const int SCREEN_WIDTH_IN_PIXELS = 1600;
const int SCREEN_HEIGHT_IN_PIXELS = 1200;

//const sf::VideoMode WINDOW_VIDEO_MODE = sf::VideoMode(SCREEN_WIDTH_IN_PIXELS, 
	                                                  //SCREE_HEIGHT_IN_PIXELS);

const sf::String WINDOW_TITLE = "Perceptron";
const int ANTIALIASING_LEVEL = 8;

const int ORIGIN_X = SCREEN_WIDTH_IN_PIXELS / 2;
const int ORIGIN_Y = SCREEN_HEIGHT_IN_PIXELS / 2;

const sf::Color AXES_COLOR = sf::Color(0, 0, 0);
const int AXES_THICKNESS = 2;

const int GRID_STEP_IN_PIXELS = 20;
const sf::Color GRID_COLOR = sf::Color(200, 200, 200);

const int PLOT_STEP_IN_PIXELS = 1;
const sf::Color PLOT_COLOR = sf::Color(0, 180, 0);
//const sf::Color PLOT_COLOR = sf::Color(255, 128, 0);

const double LEARNING_RATE = 0.1;
const int NUM_ITERATIONS = 1;
const int NUM_DIMENSIONS = 2 + 1;//including bias
const int SAMPLE_SIZE = 7;
const double WEIGHT_INITIAL_VALUE = 0;

const int LABEL_PLUS = 1;
const int LABEL_MINUS = 0;

const int MODULO = 10000;

const double CIRCLE_RADIUS = 4;
const double SQUARE_SIDE = 2 * CIRCLE_RADIUS;

class GraphPlotter {
public:
	GraphPlotter();
	~GraphPlotter();	
	void init_window();
	void init_axes();
	void init_axes2();
	void init_grid();
	void draw_axes();
	void draw_grid();
	void update_function_data();
	void plot_function();
	double f(double x);
	void init_training_data();
	void plot_training_data();
	void run();
	void init_weights();
	void print_weights();
	void train_perceptron();
public:
	sf::RenderWindow *window_;
	std::vector<sf::Vertex> x_axis_;
	std::vector<sf::Vertex> y_axis_;
	//sf::RectangleShape x_axis_;
	//sf::RectangleShape y_axis_;
	std::vector<std::vector<sf::Vertex> > vertical_grid_;
	std::vector<std::vector<sf::Vertex> > horizontal_grid_;
	std::vector<std::vector<sf::Vertex> > function_data_;
	std::vector<std::vector<int> > training_data_red_;
	std::vector<std::vector<int> > training_data_blue_;
	std::vector<double> weights_;
};


#endif
