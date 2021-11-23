#pragma once
/*!
\file
\brief Header that contains implementation of genetic algorithm
*/
#include <utility>
#include <functional>
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
		/// Method to mutate Individual with given 0 <= chance <= 1
		/// </summary>
		virtual void mutate(int mutation_rate) = 0;
	};
	/*!
	* \brief Class that implements Genetic Algorithm in solve method.
	
		\tparam Individual Individual class with:\n
				public get_fitness method (fitness must have operator > implemented)\n
				public mutation method with build-in mutation_rate \code void mutate(int mutation_rate) \endcode
		Individual class passed as template parameter to GeneticAlgo class. If one of this methods not implemented you'll get compile error.\n 
		For convenience, you can inherit IndividualInteface abstract class and implement required pure virtual methods. 

		You may need call solve method several times with random initial population, to find more accurate answers.
		Performance of algorithm highly relies on performance of get_fitness and comparison of fitness.\n
		Example of usage:
		\code
		Individual the_fittest = nia::GeneticAlgo<Individual>::solve(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, 
		MUTATION_RATE, population, breed);
		\endcode		
	*/
	template <typename Individual>
	class GeneticAlgo {
	private:
		static bool comp_individ(Individual& l, Individual& r) {
			return l.get_fitness() > r.get_fitness();
		}
		static Individual* mating_pool(const long long NUMBER_OF_INDIVIDUALS, Individual population[], long long size) {
			Individual* pool = new Individual[size];
			// Tournament selection
			for (int i = 0; i < size; i++) {
				int index1 = rand() % NUMBER_OF_INDIVIDUALS;
				int index2 = rand() % NUMBER_OF_INDIVIDUALS;
				if (population[index1].get_fitness() > population[index2].get_fitness()) {
					pool[i] = population[index1];
				}
				else
					pool[i] = population[index2];
			}
			return pool;
		}
		static void next_generation(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES, const long long MUTATION_RATE, 
			Individual population[],
			std::pair<Individual, Individual>(*breed)(const Individual&, const Individual&)
			) {
			int m_pool_size = NUMBER_OF_INDIVIDUALS - NUMBER_OF_ELITES;
			Individual* m_pool = mating_pool(NUMBER_OF_INDIVIDUALS, population, m_pool_size);
			std::sort(m_pool, m_pool + m_pool_size, comp_individ);
			for (int i = 0; i < NUMBER_OF_ELITES; i++) {
				population[i] = m_pool[i];
				population[i].mutate(MUTATION_RATE);
			}
			int ind = NUMBER_OF_ELITES;
			for (int i = 0; ind < NUMBER_OF_INDIVIDUALS; i += 2) {
				if (i + 1 == m_pool_size) {
					population[ind] = m_pool[i];
					break;
				}
				std::pair<Individual, Individual> children = breed(m_pool[i], m_pool[i + 1]);
				population[ind] = children.first;
				population[ind].mutate(MUTATION_RATE);
				ind++;
				if (ind < NUMBER_OF_INDIVIDUALS) {
					population[ind] = children.second;
					population[ind].mutate(MUTATION_RATE);
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
		* \param[in] MUTATION_RATE mutation rate that will be passed in Individual.mutate(MUTATION_RATE) method
		* \param[in] init_population initial(starting) population
		* \param[in] breed breed function(given two parents, returns two children), aka crossover
		* \return The fittest Individual for NUMBER_OF_GENERATIONS
		*/
		static Individual solve(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES,
			const long long NUMBER_OF_GENERATIONS, const long long MUTATION_RATE, Individual init_population[], 
			std::pair<Individual, Individual>(*breed)(const Individual&, const Individual&)
			)
		{
			Individual* population = new Individual[NUMBER_OF_INDIVIDUALS];
			Individual prev_best = init_population[0];
			for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++) {
				population[i] = init_population[i];
				if (prev_best.get_fitness() < population[i].get_fitness()) {
					prev_best = population[i];
				}
			}
			for (int i = 0; i < NUMBER_OF_GENERATIONS; i++) {
				next_generation(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, MUTATION_RATE, population, breed);
				int best = 0;
				for (int i = 1; i < NUMBER_OF_INDIVIDUALS; i++) {
					if (population[i].get_fitness() > population[best].get_fitness()) {
						best = i;
					}
				}
				if (prev_best.get_route_length() == -2 || population[best].get_route_length() < prev_best.get_route_length()) {
					prev_best = population[best];
				}
			}
			delete[] population;
			return prev_best;
		}
	};
}