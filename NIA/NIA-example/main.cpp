#include <fstream>
#include <chrono>
#include <string>
#include "GeneticAlgo.h"
#include "GeneticAlgoTA.h"
#include "Visualization.h"
#include "Point.h"
#include "AntColonyOptTSP.h"

void Genetic_algo_example() {
	std::ifstream in("input51.txt");
	const int NUMBER_OF_CITIES = 51;
	const int NUMBER_OF_INDIVIDUALS = 500;
	const double MUTATION_CHANCE = 0.013;
	const int NUMBER_OF_GENERATIONS = 10000;
	const int NUMBER_OF_ELITES = 2;
	const int CORRECT_ANS = 426;
	gata::City input[NUMBER_OF_CITIES];
	for (int i = 0; i < NUMBER_OF_CITIES; i++) {
		in >> input[i].num >> input[i].x >> input[i].y;
	}
	in.close();
	auto start = std::chrono::high_resolution_clock::now();
	vis::Visualization visualization;
	gata::Individual<NUMBER_OF_CITIES> solution = gata::calc_ans<NUMBER_OF_CITIES>(NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES,
		MUTATION_CHANCE, nia::ga::SELECTION::TOURNAMENT, input, &visualization/*nullptr*/, CORRECT_ANS);
	auto end = std::chrono::high_resolution_clock::now();
	std::ofstream out("output.txt");
	out << "NUMBER_OF_CITIES " << NUMBER_OF_CITIES << "\n" << "NUMBER_OF_INDIVIDUALS " << NUMBER_OF_INDIVIDUALS << "\n"
		<< "NUMBER_OF_GENERATIONS " << NUMBER_OF_GENERATIONS << "\n" << "NUMBER_OF_ELITES " << NUMBER_OF_ELITES << "\n"
		<< "MUTATION_CHANCE " << MUTATION_CHANCE << "\n";
	out << "Has been done for " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << " seconds\n";
	out << "min_length = " << solution.get_route_length() << "\nAccuracy: "
		<< (1 - ((double)(solution.get_route_length() - CORRECT_ANS)) / CORRECT_ANS) * 100 << "%\n";
	out << "Route\n";
	for (int i = 0; i < NUMBER_OF_CITIES; i++) {
		out << solution.chromosome[i].num << " -> ";
	}
	out << solution.chromosome[0].num << "\n";
	if (visualization.window != nullptr) {
		std::cout << "Calculation is done, to exit close the graph window\n";
		while (!glfwWindowShouldClose(visualization.window))
		{
			visualization.draw();
		}
	}
}

void Ant_algo_example() {
	std::ifstream in("input51.txt");
	const int NUMBER_OF_CITIES = 51;
	const double exponent_of_pheromone = 1;
	const double exponent_of_distance = 10;
	const double evaporation_factor = 0.2;
	const double pheromone_left_by_ant = 500;
	const double ant_factor = 5;
	const double chance_of_random_path = 0.00001;
	const long long number_of_iterations = 10000;
	const int CORRECT_ANS = 426;
	vis::Point<double> input[NUMBER_OF_CITIES];
	for (size_t i = 0; i < NUMBER_OF_CITIES; i++) {
		size_t num;
		in >> num;
		in >> input[num - 1].x >> input[num - 1].y;
	}
	in.close();
	double max_dist = 0;
	double** graph = new double* [NUMBER_OF_CITIES];
	for (size_t i = 0; i < NUMBER_OF_CITIES; i++) {
		graph[i] = new double[NUMBER_OF_CITIES];
		for (size_t j = 0; j < NUMBER_OF_CITIES; j++) {
			double delta_x = input[i].x - input[j].x;
			double delta_y = input[i].y - input[j].y;
			graph[i][j] = sqrt(delta_x * delta_x + delta_y * delta_y);
			max_dist = std::max(max_dist, graph[i][j]);
		}
	}

	auto start = std::chrono::high_resolution_clock::now();
	vis::Visualization visualization;
	nia::ant::Ant solution = nia::ant::AntColonyOptTSP::solve(graph, NUMBER_OF_CITIES, exponent_of_pheromone, exponent_of_distance, evaporation_factor,
		pheromone_left_by_ant, ant_factor, chance_of_random_path, number_of_iterations, &visualization, max_dist * NUMBER_OF_CITIES, CORRECT_ANS);
	auto end = std::chrono::high_resolution_clock::now();

	std::ofstream out("output.txt");
	out << "NUMBER_OF_CITIES " << NUMBER_OF_CITIES << "\n" << "number_of_iterations " << number_of_iterations << "\n"
		<< "exponent_of_pheromone " << exponent_of_pheromone << "\n"
		<< "exponent_of_distance " << exponent_of_distance << "\n" << "evaporation_factor " << evaporation_factor << "\n"
		<< "pheromone_left_by_ant " << pheromone_left_by_ant << "\n" << "ant_factor " << ant_factor << "\n"
		<< "chance_of_random_path " << chance_of_random_path << "\n";
	out << "Has been done for " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() * 0.000001 << " seconds\n";
	out << "min_length = " << solution.get_trail_length(graph) << "\nAccuracy: "
		<< (1 - ((double)(solution.get_trail_length(graph) - CORRECT_ANS)) / CORRECT_ANS) * 100 << "%\n";
	size_t route[NUMBER_OF_CITIES + 1];
	solution.get_route(route);
	out << "Route\n";
	for (int i = 0; i < NUMBER_OF_CITIES; i++) {
		out << route[i] + 1 << " -> ";
	}
	out << route[NUMBER_OF_CITIES] << '\n';
	
	for (size_t i = 0; i < NUMBER_OF_CITIES; i++)
		delete[] graph[i];
	delete[] graph;
	if (visualization.window != nullptr) {
		std::cout << "Calculation is done, to exit close the graph window\n";
		while (!glfwWindowShouldClose(visualization.window))
		{
			visualization.draw();
		}
	}
}
int main() {
	try {
		std::string command;
		printf("Enter command: ");
		getline(std::cin, command);
		if (command == "genetic") {
			Genetic_algo_example();
		}
		if (command == "ant") {
			Ant_algo_example();
		}
	}
	catch (std::string s) {
		std::cerr << s;
	}
}