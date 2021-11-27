#include <fstream>
#include <chrono>
#include <thread>
#include "GeneticAlgo.h"
#include "GeneticAlgoTA.h"
#include "Visualization.h"

int main() {
	/*try {
		vis::Visualization visualization;
		visualization.init(3, 1.0, 0.9);
		visualization.add_and_draw(0.0);
		visualization.add_and_draw(0.2);
		visualization.add_and_draw(0.9);
		//std::this_thread::sleep_for(std::chrono::seconds(5));
		while (!glfwWindowShouldClose(visualization.window))
		{
			visualization.draw();
		}
	}
	catch (std::string s) {
		std::cout << s;
	}*/
	try {
		std::ifstream in("input51.txt");
		const int NUMBER_OF_CITIES = 51;
		const int NUMBER_OF_INDIVIDUALS = 500;
		const double MUTATION_CHANCE = 0.006;
		const int NUMBER_OF_GENERATIONS = 1000;
		const int NUMBER_OF_ELITES = 2;
		const int CORRECT_ANS = 426; // 51 - 426 - best accur = 98%, 76 - 538 - accur = 88%, 70 - 675 - accur = 94.5%
		gata::City input[NUMBER_OF_CITIES];
		for (int i = 0; i < NUMBER_OF_CITIES; i++) {
			in >> input[i].num >> input[i].x >> input[i].y;
			//std::cout << input[i].num <<  " " << input[i].x << " " << input[i].y << std::endl;
		}
		in.close();
		auto start = std::chrono::high_resolution_clock::now();
		vis::Visualization visualization;
		std::thread draw_thread;
		gata::Individual<NUMBER_OF_CITIES> solution = gata::calc_ans<NUMBER_OF_CITIES>(NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES,
			MUTATION_CHANCE, input, &visualization, CORRECT_ANS);
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
		std::cout << "Calculation is done, to exit close the graph window\n";
		while (!glfwWindowShouldClose(visualization.window))
		{
			visualization.draw();
		}
	}
	catch (std::string s) {
		std::cerr << s;
	}
}