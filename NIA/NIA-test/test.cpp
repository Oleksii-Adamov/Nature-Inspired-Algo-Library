#include "pch.h"
#include "../NIA/GeneticAlgo.h"
#include "GeneticAlgoTA.h"
TEST(GeneticAlgoTest, TA_Test) {
	using namespace gata;
	const int CHROMOSOME_SIZE_1 = 4;
	int NUMBER_OF_CITIES = CHROMOSOME_SIZE_1;
	int NUMBER_OF_INDIVIDUALS = 4;
	int NUMBER_OF_GENERATIONS = 100;
	int NUMBER_OF_ELITES = 1;
	int MUTATION_RATE = 200; // possibility = MUTATION_RATE/32767
	City* input = new City[NUMBER_OF_CITIES];
	input[0] = { 1, 0, 0 };
	input[1] = { 2, 1, 0 };
	input[2] = { 3, 1, 1 };
	input[3] = { 4, 0, 1 };

	Individual<CHROMOSOME_SIZE_1>* population = new Individual<CHROMOSOME_SIZE_1>[NUMBER_OF_INDIVIDUALS];
	init_population(NUMBER_OF_CITIES, NUMBER_OF_INDIVIDUALS, population, input);
	/*std::function<std::pair<Individual<CHROMOSOME_SIZE_1>, Individual<CHROMOSOME_SIZE_1>>(const Individual<CHROMOSOME_SIZE_1>&, 
		const Individual<CHROMOSOME_SIZE_1>&)> breed = [](const Individual<CHROMOSOME_SIZE_1>& first_parent, const Individual<CHROMOSOME_SIZE_1>& second_parent) {
		auto city_cmp = [](const City& a, const City& b) {
			return a.num < b.num;
		};
		Individual<CHROMOSOME_SIZE_1> first_child;
		Individual<CHROMOSOME_SIZE_1> second_child;
		int geneA = rand() % CHROMOSOME_SIZE_1, geneB = rand() % CHROMOSOME_SIZE_1;
		int start_gene = std::min(geneA, geneB), end_gene = std::max(geneA, geneB), sub_route_size = end_gene - start_gene + 1;
		City* a_sub_route = new City[sub_route_size];
		City* b_sub_route = new City[sub_route_size];
		for (int i = start_gene; i <= end_gene; i++) {
			int ind = i - start_gene;
			a_sub_route[ind] = first_parent.chromosome[i];
			b_sub_route[ind] = second_parent.chromosome[i];
			first_child.chromosome[i] = first_parent.chromosome[i];
			second_child.chromosome[i] = second_parent.chromosome[i];
		}
		std::sort(a_sub_route, a_sub_route + sub_route_size, city_cmp);
		std::sort(b_sub_route, b_sub_route + sub_route_size, city_cmp);
		int ind_f = 0, ind_s = 0;
		for (int i = 0; i < CHROMOSOME_SIZE_1; i++) {
			if (!std::binary_search(a_sub_route, a_sub_route + sub_route_size, second_parent.chromosome[i], city_cmp)) {
				if (ind_f == start_gene) ind_f = end_gene + 1;
				if (ind_f < CHROMOSOME_SIZE_1) {
					first_child.chromosome[ind_f] = second_parent.chromosome[i];
					ind_f++;
				}
			}
			if (!std::binary_search(b_sub_route, b_sub_route + sub_route_size, first_parent.chromosome[i], city_cmp)) {
				if (ind_s == start_gene) ind_s = end_gene + 1;
				if (ind_s < CHROMOSOME_SIZE_1) {
					second_child.chromosome[ind_s] = first_parent.chromosome[i];
					ind_s++;
				}
			}
		}
		delete[] a_sub_route;
		delete[] b_sub_route;
		return std::make_pair(first_child, second_child);
	};*/
	Individual<CHROMOSOME_SIZE_1> the_fittest = nia::GeneticAlgo<Individual<CHROMOSOME_SIZE_1>>::solve(NUMBER_OF_INDIVIDUALS,
		NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, MUTATION_RATE, population, breed);
	std::cerr << "[          ] ";
	for (int i = 0; i < NUMBER_OF_CITIES; i++) {
		std::cerr << the_fittest.chromosome[i].num << " -> ";
	}
	EXPECT_DOUBLE_EQ(the_fittest.get_route_length(), 4);
	/*EXPECT_TRUE(false) << the_fittest.chromosome[0].num << " -> " << the_fittest.chromosome[1].num << " -> "
		<< the_fittest.chromosome[2].num << " -> " << the_fittest.chromosome[3].num << " -> " << the_fittest.chromosome[0].num;
	delete[] population;
	/*static const int CHROMOSOME_SIZE = 4;
	static const int NUMBER_OF_CITIES = CHROMOSOME_SIZE;
	static const int NUMBER_OF_INDIVIDUALS = 4;
	static const int NUMBER_OF_GENERATIONS = 100;
	static const int NUMBER_OF_ELITES = 1;
	static const int MUTATION_RATE = 200; // possibility = MUTATION_RATE/32767
	static const double EPS = 1e-1;
	struct City { // or Gene
		int num;
		double x, y;
	public:
		static double distance(City a, City b) {
			double delta_x = a.x - b.x;
			double delta_y = a.y - b.y;
			return sqrt(delta_x * delta_x + delta_y * delta_y);
		}
	};
	class Individual : public nia::IndividualInteface<City, CHROMOSOME_SIZE, double> { // or Route
	public:
		Individual() {
			fitness = -1;
			route_length = -1;
		}
		double get_fitness() override {
			if (abs(fitness - (-1)) < EPS) calc_fitness();
			return fitness;
		}
	private:
		double fitness;
		double route_length;
		static void swap_cities(City& a, City& b) {
			City t = a;
			a = b;
			b = t;
		}
		void swap_cities(int i, int j) {
			swap_cities(chromosome[i], chromosome[j]);
		}
		void calc_route_length() {
			route_length = 0;
			for (int i = 0; i < NUMBER_OF_CITIES; i++) {
				if (i + 1 != NUMBER_OF_CITIES) {
					route_length += City::distance(chromosome[i], chromosome[i + 1]);
				}
				else {
					route_length += City::distance(chromosome[i], chromosome[0]);
				}
			}
		}
		void calc_fitness() {
			fitness = 1 / ((double)get_route_length());
		}
	public:
		void mutate() override {
			for (int i = 0; i < CHROMOSOME_SIZE; i++) {
				if (rand() < MUTATION_RATE) {
					int j = rand() % CHROMOSOME_SIZE;
					swap_cities(i, j);
				}
			}
		}
		double get_route_length() {
			if (abs(route_length - (-1)) < EPS) calc_route_length();
			return route_length;
		}
	};
	auto copy = [](const City from[NUMBER_OF_CITIES], City to[NUMBER_OF_CITIES]) {
		for (int i = 0; i < NUMBER_OF_CITIES; i++) {
			to[i] = from[i];
		}
	};
	auto init_population = [&](Individual population[NUMBER_OF_INDIVIDUALS], City cities[NUMBER_OF_CITIES]) {
		srand(time(0));
		for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++) {
			std::random_shuffle(cities, cities + NUMBER_OF_CITIES);
			copy(cities, population[i].chromosome);
		}
	};
	std::function<std::pair<Individual, Individual>(const Individual&, const Individual&)> breed = [](const Individual& first_parent, const Individual& second_parent) {
		auto city_cmp = [](const City& a, const City& b) {
			return a.num < b.num;
		};
		Individual first_child;
		Individual second_child;
		int geneA = rand() % CHROMOSOME_SIZE, geneB = rand() % CHROMOSOME_SIZE;
		int start_gene = std::min(geneA, geneB), end_gene = std::max(geneA, geneB), sub_route_size = end_gene - start_gene + 1;
		City* a_sub_route = new City[sub_route_size];
		City* b_sub_route = new City[sub_route_size];
		for (int i = start_gene; i <= end_gene; i++) {
			int ind = i - start_gene;
			a_sub_route[ind] = first_parent.chromosome[i];
			b_sub_route[ind] = second_parent.chromosome[i];
			first_child.chromosome[i] = first_parent.chromosome[i];
			second_child.chromosome[i] = second_parent.chromosome[i];
		}
		std::sort(a_sub_route, a_sub_route + sub_route_size, city_cmp);
		std::sort(b_sub_route, b_sub_route + sub_route_size, city_cmp);
		int ind_f = 0, ind_s = 0;
		for (int i = 0; i < CHROMOSOME_SIZE; i++) {
			if (!std::binary_search(a_sub_route, a_sub_route + sub_route_size, second_parent.chromosome[i], city_cmp)) {
				if (ind_f == start_gene) ind_f = end_gene + 1;
				if (ind_f < CHROMOSOME_SIZE) {
					first_child.chromosome[ind_f] = second_parent.chromosome[i];
					ind_f++;
				}
			}
			if (!std::binary_search(b_sub_route, b_sub_route + sub_route_size, first_parent.chromosome[i], city_cmp)) {
				if (ind_s == start_gene) ind_s = end_gene + 1;
				if (ind_s < CHROMOSOME_SIZE) {
					second_child.chromosome[ind_s] = first_parent.chromosome[i];
					ind_s++;
				}
			}
		}
		delete[] a_sub_route;
		delete[] b_sub_route;
		return std::make_pair(first_child, second_child);
	};
	City input[NUMBER_OF_CITIES];
	input[0] = { 1, 0, 0 };
	input[1] = { 2, 1, 0 };
	input[2] = { 3, 1, 1 };
	input[3] = { 4, 0, 1 };
	Individual* population = new Individual [NUMBER_OF_INDIVIDUALS];
	init_population(population, input);
	Individual the_fittest = nia::GeneticAlgo<Individual>::solve(NUMBER_OF_INDIVIDUALS, NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS,
		population, breed);
	std::cerr << "[          ] ";
	for (int i = 0; i < NUMBER_OF_CITIES; i++) {
		std::cerr << the_fittest.chromosome[i].num << " -> ";
	}
	EXPECT_DOUBLE_EQ(the_fittest.get_route_length(), 4);
	/*EXPECT_TRUE(false) << the_fittest.chromosome[0].num << " -> " << the_fittest.chromosome[1].num << " -> "
		<< the_fittest.chromosome[2].num << " -> " << the_fittest.chromosome[3].num << " -> " << the_fittest.chromosome[0].num;
	delete[] population;*/
}