#pragma once
#include "Ant.h"
#include <random>

namespace nia {
	namespace ant {
		class AntColonyOptTSP
		{
		private:
			static void setup_ants(size_t number_of_vertecies, size_t number_of_ants, Ant ants[], std::mt19937& gen);
			static void move_ants(size_t number_of_vertecies, size_t number_of_ants, Ant ants[], std::mt19937& gen, double** pheromone,
				double exponent_of_pheromone, double** attractiveness, double chance_of_random_path);
			static size_t next_vertex(Ant& ant, size_t trail_index, size_t number_of_vertecies, std::mt19937& gen, double** pheromone,
				double exponent_of_pheromone, double** attractiveness, double chance_of_random_path);
		public:
			static Ant solve(double** graph, size_t number_of_vertecies, double exponent_of_pheromone, double exponent_of_distance, double evaporation_factor, double pheromone_left_by_ant,
				double ant_factor, double chance_of_random_path, long long number_of_iterations);
		};
	}
}

