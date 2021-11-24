#pragma once
/*!
\file
\brief Header that contains implementation of genetic algorithm
*/
#include <utility>
#include <functional>
#include <random>
#include <algorithm>
#include <iostream>
namespace nia {
	/// <summary>
	/// Interface for Individual.
	/// </summary>
	/// \tparam Tgene Type of gene
	/// \tparam chromosome_size Size of chromosome for each Individual
	/// \tparam Tfitess Type of fitness of Individual
	// <typeparam name="Tgene" Type of gene, Tgene chromosome[cromosome size] will be created></typeparam>
	// <typeparam name="Tfitess" Type of fitness of Individual></typeparam>
	template<typename Tgene, int chromosome_size, typename Tfitess>
	class IndividualInteface {
	public:
		Tgene chromosome[chromosome_size];
		/// <summary>
		/// Method to get fitness from Individual (using already done calculations for better performance is welcome) 
		/// </summary>
		virtual Tfitess get_fitness() = 0;
		/// <summary>
		/// Method to mutate Individual with given 0 <= mutation_chance <= 1
		/// </summary>
		virtual void mutate(double mutation_chance, std::mt19937& gen) = 0;
	};
	/*!
	* \brief Class that implements Genetic Algorithm in solve method.
	
		\tparam Individual Individual class with:\n
				public get_fitness method (fitness must have operator > implemented)\n
				public mutation method with build-in mutation_rate \code void mutate(double mutation_chance, std::mt19937& gen) \endcode
		Individual class passed as template parameter to GeneticAlgo class. If one of this methods not implemented you'll get compile error.\n 
		For convenience, you can inherit IndividualInteface abstract class and implement required pure virtual methods. 

		You may need call solve method several times with random initial population, to find more accurate answers.
		Performance of algorithm highly relies on performance of get_fitness and comparison of fitness.\n
		Example of usage:
		\code
		Individual the_fittest = nia::GeneticAlgo<Individual>::solve(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, MUTATION_CHANCE, population, breed);
		\endcode		
	*/
	template <typename Individual>
	class GeneticAlgo {
	private:
		static bool comp_individ(Individual& l, Individual& r) {
			return l.get_fitness() > r.get_fitness();
		}
		static Individual* mating_pool(const size_t NUMBER_OF_INDIVIDUALS, Individual population[], size_t size, std::mt19937& gen) {
			Individual* pool = new Individual[size];
			std::uniform_int_distribution<size_t> pos_dis(0, NUMBER_OF_INDIVIDUALS - 1);
			// Tournament selection
			for (size_t i = 0; i < size; i++) {
				size_t index1 = pos_dis(gen);
				size_t index2 = pos_dis(gen);
				if (population[index1].get_fitness() > population[index2].get_fitness()) {
					pool[i] = population[index1];
				}
				else
					pool[i] = population[index2];
			}
			return pool;
		}
		static void next_generation(const size_t NUMBER_OF_INDIVIDUALS, const size_t NUMBER_OF_ELITES, const double MUTATION_CHANCE,
			Individual population[],
			std::pair<Individual, Individual>(*breed)(const Individual&, const Individual&, std::mt19937&), std::mt19937& gen
			) {
			size_t m_pool_size = NUMBER_OF_INDIVIDUALS - NUMBER_OF_ELITES;
			Individual* m_pool = mating_pool(NUMBER_OF_INDIVIDUALS, population, m_pool_size, gen);
			std::sort(m_pool, m_pool + m_pool_size, comp_individ);
			for (size_t i = 0; i < NUMBER_OF_ELITES; i++) {
				population[i] = m_pool[i];
				population[i].mutate(MUTATION_CHANCE, gen);
			}
			size_t ind = NUMBER_OF_ELITES;
			for (size_t i = 0; ind < NUMBER_OF_INDIVIDUALS; i += 2) {
				if (i + 1 == m_pool_size) {
					population[ind] = m_pool[i];
					break;
				}
				std::pair<Individual, Individual> children = breed(m_pool[i], m_pool[i + 1], gen);
				population[ind] = children.first;
				population[ind].mutate(MUTATION_CHANCE, gen);
				ind++;
				if (ind < NUMBER_OF_INDIVIDUALS) {
					population[ind] = children.second;
					population[ind].mutate(MUTATION_CHANCE, gen);
					ind++;
				}
			}
			delete[] m_pool;
		}
	public:
		/*!
		* \param[in] NUMBER_OF_INDIVIDUALS number of individuals in every generation
		* \param[in] NUMBER_OF_ELITES number of best individuals kept for next generation
		* \param[in] NUMBER_OF_GENERATIONS number of generations this algorithm will process
		* \param[in] MUTATION_CHANCE 0 <= mutation chance <= 1 that will be passed in Individual.mutate method
		* \param[in] init_population initial(starting) population
		* \param[in] breed breed function(given two parents and random generator, returns two children), aka crossover
		* \return The fittest Individual for NUMBER_OF_GENERATIONS
		*/
		static Individual solve(const size_t NUMBER_OF_INDIVIDUALS, const size_t NUMBER_OF_ELITES,
			const long long NUMBER_OF_GENERATIONS, const double MUTATION_CHANCE, Individual init_population[],
			std::pair<Individual, Individual>(*breed)(const Individual&, const Individual&, std::mt19937&)
			)
		{
			// generating random sequence
			std::random_device rd;  //Will be used to obtain a seed for the random number engine
			std::seed_seq seed{ rd(), static_cast<unsigned int>(time(nullptr)) }; // in case random_device is not implemented, fall back to time(0)
			std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with seed
			// copying initial population
			Individual* population = new Individual[NUMBER_OF_INDIVIDUALS];
			Individual prev_best = init_population[0];
			for (size_t i = 0; i < NUMBER_OF_INDIVIDUALS; i++) {
				population[i] = init_population[i];
				if (prev_best.get_fitness() < population[i].get_fitness()) {
					prev_best = population[i];
				}
			}
			// looping through generations
			for (long long i = 0; i < NUMBER_OF_GENERATIONS; i++) {
				next_generation(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, MUTATION_CHANCE, population, breed, gen);
				size_t best = 0;
				for (size_t j = 1; j < NUMBER_OF_INDIVIDUALS; j++) {
					if (population[j].get_fitness() > population[best].get_fitness()) {
						best = j;
					}
				}
				if (prev_best.get_route_length() == -2 || population[best].get_route_length() < prev_best.get_route_length()) {
					prev_best = population[best];
				}
				//std::cout << prev_best.get_route_length() << std::endl;
			}
			delete[] population;
			return prev_best;
		}
	};
}