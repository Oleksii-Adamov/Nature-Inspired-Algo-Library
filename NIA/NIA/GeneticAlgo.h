#pragma once
#include<utility>
namespace nia {
	
	template<typename Tgene, int chromosome_size, typename Tfitess>
	class IndividualInteface {
	protected:
		Tgene chromosome[chromosome_size];
	public:
		virtual Tfitess get_fintess();
	};
	/* genetic algo requires:
		classes/types:
			Gene type/class
			Individual class with public get_fintess method (fintess must have operator < implemented)
		constants:
			chromosome_size
			NUMBER_OF_INDIVIDUALS (number of individuals in every generation)
			NUMBER_OF_ELITES (number of best inividuals kept for next generation)
			NUMBER_OF_GENERATIONS
		parameters:
			initial(starting) population - Individual init_population[NUMBER_OF_INDIVIDUALS]
		functions:
			mutation function with build-in mutation_rate - void mutate(Individual &a)
			breed function(given two parents, returns two children)- pair<Individual, Individual> breed(Individual a, Individual b)
		
		Genetic algo returns the fittest Individual for NUMBER_OF_GENERATIONS
		you may need call this function several times with random initial population, to find more accurate answer
		performance of algorithm higly relies on performance of get_fitness and comparation of fintess
	*/
	template<typename Tgene, int chromosome_size, typename Tfitess>
	class GeneticAlgo {
		using Individual = IndividualInteface<Tgene, chromosome_size, Tfitess>;
	public:
		static Individual solve(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES,
			const long long NUMBER_OF_GENERATIONS, Individual init_population[NUMBER_OF_INDIVIDUALS],
			void(*mutate)(Individual&),
			std::pair<Individual, Individual>(*breed)
			(Individual, Individual));
	};
	/*template<typename Tgene, int chromosome_size, typename Tfitess>
	IndividualInteface<Tgene, chromosome_size, Tfitess> genetic_algo(const long long NUMBER_OF_INDIVIDUALS, const long long NUMBER_OF_ELITES,
		const long long NUMBER_OF_GENERATIONS, IndividualInteface<Tgene, chromosome_size, Tfitess> init_population[NUMBER_OF_INDIVIDUALS], 
		void(*mutate)(IndividualInteface<Tgene, chromosome_size, Tfitess>&), 
		std::pair<IndividualInteface<Tgene, chromosome_size, Tfitess>, IndividualInteface<Tgene, chromosome_size, Tfitess>>(*breed)
		(IndividualInteface<Tgene, chromosome_size, Tfitess>, IndividualInteface<Tgene, chromosome_size, Tfitess>));*/
}