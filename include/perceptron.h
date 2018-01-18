#ifndef PERCEPTRON_H_
#define PERCEPTRON_H_

#include <string>
#include <vector>
#include <array>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>


const int kScreenWidthInPixels = 1600;
const int kScreenHeightInPixels = 1200;


const sf::String kWindowTitle = "Perceptron";
const int kAntialiasingLevel = 8;

const int kOriginX = kScreenWidthInPixels / 2;
const int kOriginY = kScreenHeightInPixels / 2;

const sf::Color kAxesColor = sf::Color(0, 0, 0);
const int kAxesThickness = 2;

const int kGridStepInPixels = 20;
const sf::Color kGridColor = sf::Color(200, 200, 200);

const int kPlotStepInPixels = 1;
const sf::Color kPlotColor = sf::Color(0, 180, 0);
//const sf::Color kPlotColor = sf::Color(255, 128, 0);

const double kLearningRate = 0.005;
const int kNumIterations = 1;
const int kNumDimensions = 2 + 1;//including bias
const int kSampleSize = 7;
const double kWeightInitialValue = 0;

const int kModulo = 10000;

const double kCircleRadius = 4;
const double kSquareSide = 2 * kCircleRadius;

//struct representing sample points
struct Example {
	Example(const std::vector<int>& data, int label) : data_(data), label_(label){}
	std::vector<int> data_;
	int label_ = 0;
};


class Perceptron {
public:
	Perceptron();
	~Perceptron();	
	void InitWindow();
	void InitAxes();
	void InitGrid();
	void DrawAxes();
	void DrawGrid();
	void UpdateDecisionBoundaryData();
	void PlotDecisionBoundary();
	double F(double x);
	void InitTrainingData();
	void PlotTrainingData();
	void Run();
	void InitWeights();
	void PrintWeights();
	void TrainPerceptron();
public:
	std::unique_ptr<sf::RenderWindow> window_;
	std::vector<sf::Vertex> x_axis_;
	std::vector<sf::Vertex> y_axis_;
	std::vector<std::vector<sf::Vertex> > vertical_grid_;
	std::vector<std::vector<sf::Vertex> > horizontal_grid_;
	std::vector<std::vector<sf::Vertex> > decision_boundary_data_;
	std::vector<Example> training_data_;
	std::vector<double> weights_;
};


#endif //PERCEPTRON_H_