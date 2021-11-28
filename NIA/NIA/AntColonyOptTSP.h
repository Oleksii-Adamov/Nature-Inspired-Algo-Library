#pragma once
/*!
\file
\brief Header for AntColonyOptTSP class
*/
#include "Ant.h"
#include <random>
#include "Visualization.h"
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
			/*!
			* \param[in] graph distance matrix of graph
			* \param[in] number_of_vertecies number of vertecies in graph
			* \param[in] exponent_of_pheromone exponent for exponentiation of pheromone on each edge of trail to decide where ant will go
			* \param[in] exponent_of_distance exponent for exponentiation of 1/dist on each edge of trail to decide where ant will go
			* \param[in] evaporation_factor after every iteration pheromone on trail x = x * (1 - evaporation_factor)
			* \param[in] pheromone_left_by_ant contribution to trail =  (pheromone left by ant) / (length of trail(tour))
			* \param[in] ant_factor number_of_ants = ant_factor * number_of_vertecies
			* \param[in] chance_of_random_path chance that random path
			* \param[in] number_of_iterations number of iterations (number of times that ants will be spawn) 
			* \param[in] visualization_ptr pointer to uninitialized (method init haven't been called) Visualization instance, pass nullptr or nothing, if you don't want visualization
			* \param[in] max_dist maximum distance that could be represented in visualization (must be initialized with value > 0, otherwise std::string exception)
			* \param[in] ans optional parameter that represents distance of correct answer > 0 (affects only visualization)
			* \return The Ant that done the best tour for number_of_iterations
			*/
			static Ant solve(double** graph, size_t number_of_vertecies, double exponent_of_pheromone, double exponent_of_distance, double evaporation_factor,
				double pheromone_left_by_ant, double ant_factor, double chance_of_random_path, long long number_of_iterations,
				vis::Visualization* visualization_ptr = nullptr, const double max_dist = -1, const double ans = -1);
		};
	}
}

