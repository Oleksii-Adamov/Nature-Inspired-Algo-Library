# Nature-Inspired-Algo-Library: In progress
## TO DO
### 1. Genetic Algo: In progress
#### General genetic algo implemented, with Tournament Selection
#### Planning adding more selection algorithms to choose from \(at least Roulette Wheel Selection\)
### 2. ????\(Not decided yet\)
## How to use:
### Genetic Algo
		   Genetic algo requires:
			   constants:
					NUMBER_OF_INDIVIDUALS (number of individuals in every generation)
					NUMBER_OF_ELITES (number of best inividuals kept for next generation)
					NUMBER_OF_GENERATIONS

				classes/types:
					Individual class with:
						public get_fintess method (fintess must have operator < implemented)
						public mutation method with build-in mutation_rate - 
```cpp
						void mutate(Individual &a)
```
				Individual class passed as template parameter to GeneticAlgo class. If one of this methods not imlemented you'll get compile error. 
				For convinience, you can inherit IndividualInteface astract class and implement requiered pure virtual methods. 

				parameters:

					initial(starting) population -
```cpp
				Individual init_population[NUMBER_OF_INDIVIDUALS] 
 ```
				
				functions:
					breed function(given two parents, returns two children) - 
					
```cpp 
				pair<Individual, Individual> breed(Individual a, Individual b)
```

				Genetic algo returns the fittest Individual for NUMBER_OF_GENERATIONS
				
				You may need call this function several times with random initial population, to find more accurate answer
				performance of algorithm higly relies on performance of get_fitness and comparation of fintess

			Example of usage:
```cpp
				Individual the_fittest = nia::GeneticAlgo<Individual>::solve(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, MUTATION_RATE, population, breed);
```