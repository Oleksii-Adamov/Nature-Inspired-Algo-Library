#include "AntColonyOptTSP.h"
#include <ctime>
#include <cmath>
#include <iostream>

namespace nia {
	namespace ant {
		void AntColonyOptTSP::setup_ants(size_t number_of_vertecies, size_t number_of_ants, Ant ants[], std::mt19937& gen) {
			std::uniform_int_distribution<size_t> vertex_dis(0, number_of_vertecies - 1);
			for (size_t i = 0; i < number_of_ants; i++) {
				ants[i].clear();
				ants[i].visit(0, vertex_dis(gen));
			}
		}
		size_t AntColonyOptTSP::next_vertex(Ant& ant, size_t index_of_trail, size_t number_of_vertecies, std::mt19937& gen, double** pheromone,
			double exponent_of_pheromone, double** attractiveness, double chance_of_random_path) {
			std::uniform_real_distribution<> dis(0, 1);
			
			// chance to choose random vertex
			if (dis(gen) < chance_of_random_path) {
				std::uniform_int_distribution<size_t> vertex_dis(0, number_of_vertecies - 1);
				size_t random_vertex = vertex_dis(gen);
				while (ant.is_visited(random_vertex))
					random_vertex = vertex_dis(gen);
				return random_vertex;
			}

			double sum = 0;
			// allocating on stack
			double* numerators = (double*)alloca(number_of_vertecies * sizeof(double));

			// calculating numerators and sum
			for (size_t i = 0; i < number_of_vertecies; i++) {
				if (ant.is_visited(i)) {
					numerators[i] = 0;
				}
				else {
					double test1 = std::pow(pheromone[ant.trail[index_of_trail - 1]][i], exponent_of_pheromone);
					double test2 = attractiveness[ant.trail[index_of_trail - 1]][i];
					numerators[i] = std::pow(pheromone[ant.trail[index_of_trail - 1]][i], exponent_of_pheromone) * 
						attractiveness[ant.trail[index_of_trail - 1]][i];
					sum += numerators[i];
				}
			}
			if (sum > 0) {
				double random_number = dis(gen);
				double offset = 0.0;
				for (size_t i = 0; i < number_of_vertecies; i++) {
					offset += numerators[i] / sum;
					if (random_number < offset) {
						return i;
					}
				}
			}
			else {
				std::uniform_int_distribution<size_t> vertex_dis(0, number_of_vertecies - 1);
				size_t random_vertex = vertex_dis(gen);
				while (ant.is_visited(random_vertex))
					random_vertex = vertex_dis(gen);
				return random_vertex;
			}
		}
		void AntColonyOptTSP::move_ants(size_t number_of_vertecies, size_t number_of_ants, Ant ants[], std::mt19937& gen, double** pheromone,
			double exponent_of_pheromone, double** attractiveness, double chance_of_random_path) {
			for (size_t index_of_trail = 1; index_of_trail < number_of_vertecies; index_of_trail++) {
				for (size_t i = 0; i < number_of_ants; i++) {
					ants[i].visit(index_of_trail, next_vertex(ants[i], index_of_trail, number_of_vertecies, gen, pheromone, exponent_of_pheromone,
						attractiveness, chance_of_random_path));
				}
			}
		}
		void update_pheromone(size_t number_of_vertecies, double** graph, double** pheromone, double evaporation_factor, double pheromone_left_by_ant, size_t number_of_ants, 
			Ant ants[]) {
			for (size_t i = 0; i < number_of_vertecies; i++) {
				for (size_t j = 0; j < number_of_vertecies; j++) {
					pheromone[i][j] *= 1 - evaporation_factor;
				}
			}
			for (size_t i = 0; i < number_of_ants; i++) {
				double contribution = pheromone_left_by_ant / ants[i].get_trail_length(graph);
				for (int i = 0; i < number_of_vertecies - 1; i++) {
					pheromone[ants[i].trail[i]][ants[i].trail[i + 1]] += contribution;
				}
				pheromone[ants[i].trail[number_of_vertecies - 1]][ants[i].trail[0]] += contribution;
			}
		}
		Ant AntColonyOptTSP::solve(double** graph, size_t number_of_vertecies, double exponent_of_pheromone, double exponent_of_distance,
			double evaporation_factor, double pheromone_left_by_ant, double ant_factor, double chance_of_random_path, long long number_of_iterations) {
			// generating random sequence
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::seed_seq seed{ rd(), static_cast<unsigned int>(time(nullptr)) }; // in case random_device is not implemented, fall back to time(0)
			std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with seed

			// creating ants
			size_t number_of_ants = (size_t)(ant_factor * number_of_vertecies);
			Ant* ants = new Ant[number_of_ants];
			for (size_t i = 0; i < number_of_ants; i++)
				ants[i].set_number_of_vertices(number_of_vertecies);

			// creating pheromones matrix
			double** pheromone = new double*[number_of_vertecies];
			for (size_t i = 0; i < number_of_vertecies; i++) {
				pheromone[i] = new double[number_of_vertecies];
				for (size_t j = 0; j < number_of_vertecies; j++) {
					pheromone[i][j] = 0;
				}
			}

			// creating and calculating attractiveness matrix
			double** attractiveness = new double* [number_of_vertecies];
			for (size_t i = 0; i < number_of_vertecies; i++) {
				attractiveness[i] = new double[number_of_vertecies];
				for (size_t j = 0; j < number_of_vertecies; j++) {
					attractiveness[i][j] = std::pow(1 / graph[i][j], exponent_of_distance);
				}
			}
			Ant best_ant;
			
			// looping through iterations
			for (long long i = 0; i < number_of_iterations; i++) {
				setup_ants(number_of_vertecies, number_of_ants, ants, gen);
				move_ants(number_of_vertecies, number_of_ants, ants, gen, pheromone, exponent_of_pheromone, attractiveness, chance_of_random_path);
				update_pheromone(number_of_vertecies, graph, pheromone, evaporation_factor, pheromone_left_by_ant, number_of_ants, ants);

				// updating best and visualizing if needed
				size_t best = 0;
				for (size_t j = 1; j < number_of_ants; j++) {
					if (ants[j].get_trail_length(graph) < ants[best].get_trail_length(graph))
						best = j;
				}
				if (i == 0 || best_ant.get_trail_length(graph) < ants[best].get_trail_length(graph))
					best_ant = ants[best];
				//std::cout << ants[best].get_trail_length(graph) << "\n";
			}
			
			// deleting ants
			delete[] ants;

			// deleting pheromones matrix
			for (size_t i = 0; i < number_of_vertecies; i++) {
				delete[] pheromone[i];
			}
			delete[] pheromone;

			// deleting attractiveness matrix
			for (size_t i = 0; i < number_of_vertecies; i++) {
				delete[] attractiveness[i];
			}
			delete[] attractiveness;

			return best_ant;
		}
	}
}
