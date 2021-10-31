# Nature-Inspired-Algo-Library: In progress
## TO DO
### 1. Genetic Algo: In progress
#### General genetic algo implemented, with Tournament Selection.
#### Unit tests for GA are implemented.
#### Planning adding more selection algorithms to choose from \(at least Roulette Wheel Selection\).
### 2. ????\(Not decided yet\)
## How to use:
### Genetic Algo
#### Genetic algo description
Genetic algo returns the fittest Individual for NUMBER_OF_GENERATIONS.
				
You may need call this function several times with random initial population, to find more accurate answer performance of algorithm higly relies on performance of get_fitness and comparison of fintess.
#### Genetic algo requires:
##### Constants:
1. NUMBER_OF_INDIVIDUALS (number of individuals in every generation)
2. NUMBER_OF_ELITES (number of best inividuals kept for next)
3. NUMBER_OF_GENERATIONS

##### Classes/types:
###### Individual class with:
1. public get_fintess method (fintess must have operator < implemented)

2. public mutation method with build-in mutation_rate - 
```cpp
void mutate(int mutation_rate);
```
Individual class passed as template parameter to GeneticAlgo class. If one of this methods not imlemented you'll get compile error. For convinience, you can inherit IndividualInteface astract class and implement requiered pure virtual methods. 

##### Parameters:

Initial(starting) population -
```cpp
Individual init_population[NUMBER_OF_INDIVIDUALS];
 ```
				
##### Functions:
breed function(given two parents, returns two children) - 
					
```cpp 
std::pair<Individual, Individual> breed(const Individual& a, const Individual& b);
```



#### Example of usage:
```cpp
Individual the_fittest = nia::GeneticAlgo<Individual>::solve(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS,MUTATION_RATE, population, breed);
```