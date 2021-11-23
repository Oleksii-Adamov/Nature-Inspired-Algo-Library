#include "pch.h"
#include "../NIA/GeneticAlgo.h"
#include "GeneticAlgoTA.h"
TEST(GeneticAlgoTest, TA_Test) {
	using namespace gata;
	int NUMBER_OF_INDIVIDUALS = 4;
	int NUMBER_OF_GENERATIONS = 100;
	int NUMBER_OF_ELITES = 1;
	double MUTATION_CHANCE = 0.06;
	const int NUMBER_OF_CITIES_1 = 4;
	City input[5];
	input[0] = { 1, 0, 0 };
	input[1] = { 2, 1, 0 };
	input[2] = { 3, 1, 1 };
	input[3] = { 4, 0, 1 };
	EXPECT_DOUBLE_EQ(testTa <NUMBER_OF_CITIES_1> (NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES, MUTATION_CHANCE, input), 4);
	const int NUMBER_OF_CITIES_2 = 2;
	input[0] = { 1, 0, 0 };
	input[1] = { 2, 1, 0 };
	EXPECT_DOUBLE_EQ(testTa <NUMBER_OF_CITIES_2>(NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES, MUTATION_CHANCE, input), 2);
	const int NUMBER_OF_CITIES_3 = 1;
	input[0] = { 1, 0, 0 };
	EXPECT_DOUBLE_EQ(testTa <NUMBER_OF_CITIES_3>(NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES, MUTATION_CHANCE, input), 0);
	const int NUMBER_OF_CITIES_4 = 5;
	input[0] = { 1, 0, 0 };
	input[1] = { 2, 1, 1 };
	input[2] = { 3, 2, 0 };
	input[3] = { 4, 3, 1 };
	input[4] = { 5, 4, 0 };
	EXPECT_LT(testTa <NUMBER_OF_CITIES_4>(NUMBER_OF_INDIVIDUALS, NUMBER_OF_GENERATIONS, NUMBER_OF_ELITES, MUTATION_CHANCE, input), 4*sqrt(2)+4+0.0001);
}