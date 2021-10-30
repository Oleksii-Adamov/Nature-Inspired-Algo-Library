#pragma once
#include <utility>
#include <functional>
namespace nia {
	
	template<typename Tgene, int chromosome_size, typename Tfitess>
	class IndividualInteface {
		using Individual = IndividualInteface<Tgene, chromosome_size, Tfitess>;
	public:
		Tgene chromosome[chromosome_size];
		virtual Tfitess get_fitness() = 0;
		virtual void mutate() = 0;
		// copy constructor
		/*virtual IndividualInteface(const IndividualInteface& t)
		{
			for (int i = 0; i < chromosome_size; i++) {
				chromosome[i] = t.chromosome[i];
			}
		}*/
		/*virtual std::pair<IndividualInteface<Tgene, chromosome_size, Tfitess>, IndividualInteface<Tgene, chromosome_size, Tfitess>> 
			breed(IndividualInteface<Tgene, chromosome_size, Tfitess>& second_parent); // first parent is this*/
	};
	/* genetic algo requires:
	   constants:
			chromosome_size
			NUMBER_OF_INDIVIDUALS (number of individuals in every generation)
			NUMBER_OF_ELITES (number of best inividuals kept for next generation)
			NUMBER_OF_GENERATIONS

		classes/types:
			Gene type/class
			Individual class with:
				public get_fintess method (fintess must have operator < implemented)
				mutation method with build-in mutation_rate - void mutate(Individual &a)
				static breed method (given two parents, returns two children)- static pair<Individual, Individual> breed(Individual a, Individual b)

		parameters:
			initial(starting) population - Individual init_population[NUMBER_OF_INDIVIDUALS]
		functions:
			mutation function with build-in mutation_rate - void mutate(Individual &a)
			breed function(given two parents, returns two children)- pair<Individual, Individual> breed(Individual a, Individual b)
		
		Genetic algo returns the fittest Individual for NUMBER_OF_GENERATIONS
		you may need call this function several times with random initial population, to find more accurate answer
		performance of algorithm higly relies on performance of get_fitness and comparation of fintess
	*/
	// maybe just typename Individual, but then we don't use inheritance, so I don't know...
	//template<typename Tgene, int chromosome_size, typename Tfitess>
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
		static void next_generation(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES, Individual population[], 
			std::function <std::pair<Individual, Individual>(const Individual&, const Individual&)>& breed) {
			int m_pool_size = NUMBER_OF_INDIVIDUALS - NUMBER_OF_ELITES;
			Individual* m_pool = mating_pool(NUMBER_OF_INDIVIDUALS, population, m_pool_size);
			std::sort(m_pool, m_pool + m_pool_size, comp_individ);
			for (int i = 0; i < NUMBER_OF_ELITES; i++) {
				population[i] = m_pool[i];
				population[i].mutate();
			}
			int ind = NUMBER_OF_ELITES;
			for (int i = 0; ind < NUMBER_OF_INDIVIDUALS; i += 2) {
				if (i + 1 == m_pool_size) {
					population[ind] = m_pool[i];
					break;
				}
				std::pair<Individual, Individual> children = breed(m_pool[i], m_pool[i + 1]);
				population[ind] = children.first;
				population[ind].mutate();
				ind++;
				if (ind < NUMBER_OF_INDIVIDUALS) {
					population[ind] = children.second;
					population[ind].mutate();
					ind++;
				}
			}
			delete[] m_pool;
		}
	public:
		static Individual solve(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES,
			const long long NUMBER_OF_GENERATIONS, Individual init_population[], 
			std::function <std::pair<Individual, Individual>(const Individual&, const Individual&)>& breed)
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
				next_generation(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, population, breed);
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
	/*template<typename Tgene, int chromosome_size, typename Tfitess>
	IndividualInteface<Tgene, chromosome_size, Tfitess> genetic_algo(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES,
		const long long NUMBER_OF_GENERATIONS, IndividualInteface<Tgene, chromosome_size, Tfitess> init_population[NUMBER_OF_INDIVIDUALS], 
		void(*mutate)(IndividualInteface<Tgene, chromosome_size, Tfitess>&), 
		std::pair<IndividualInteface<Tgene, chromosome_size, Tfitess>, IndividualInteface<Tgene, chromosome_size, Tfitess>>(*breed)
		(IndividualInteface<Tgene, chromosome_size, Tfitess>, IndividualInteface<Tgene, chromosome_size, Tfitess>));*/
}