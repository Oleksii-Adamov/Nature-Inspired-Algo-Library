#pragma once
#include "../NIA/GeneticAlgo.h"
#include <algorithm>
namespace gata {
	const double EPS = 1e-1;
	struct City { // or Gene
		int num;
		double x, y;
	private:
		/*static int nint(double x) {
			return (int)(x + 0.5);
		}*/
	public:
		static double distance(City a, City b) {
			double delta_x = a.x - b.x;
			double delta_y = a.y - b.y;
			return /*nint(*/sqrt(delta_x * delta_x + delta_y * delta_y)/*)*/;
		}
	};
	template<int chromosome_size>
	class Individual : public nia::IndividualInteface<City, chromosome_size, double> { // or Route
	public:
		//static int mutation_rate;
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
			for (int i = 0; i < chromosome_size; i++) {
				if (i + 1 != chromosome_size) {
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
		void mutate(int mutation_rate) override {
			for (int i = 0; i < chromosome_size; i++) {
				if (rand() < mutation_rate) {
					int j = rand() % chromosome_size;
					swap_cities(i, j);
				}
			}
		}
		double get_route_length() {
			if (abs(route_length - (-1)) < EPS) calc_route_length();
			return route_length;
		}
	};
	void copy (int NUMBER_OF_CITIES, const City from[], City to[]) {
		for (int i = 0; i < NUMBER_OF_CITIES; i++) {
			to[i] = from[i];
		}
	};
	template<int chromosome_size>
	void init_population (int NUMBER_OF_CITIES, int NUMBER_OF_INDIVIDUALS, Individual<chromosome_size> population[], City cities[]) {
		srand(time(0));
		for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++) {
			std::random_shuffle(cities, cities + NUMBER_OF_CITIES);
			copy(NUMBER_OF_CITIES, cities, population[i].chromosome);
		}
	};
	//template<int chromosome_size>
	//std::pair<Individual<chromosome_size>, Individual<chromosome_size>> breed (const Individual<chromosome_size>& first_parent,
	//	const Individual<chromosome_size>& second_parent) {
	//template<typename Individual>
	template<int chromosome_size>
	std::pair<Individual<chromosome_size>, Individual<chromosome_size>> breed (
		const Individual<chromosome_size>& first_parent,
		const Individual<chromosome_size>& second_parent) {
		auto city_cmp = [](const City& a, const City& b) {
			return a.num < b.num;
		};
		Individual<chromosome_size> first_child;
		Individual<chromosome_size> second_child;
		int geneA = rand() % chromosome_size, geneB = rand() % chromosome_size;
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
		for (int i = 0; i < chromosome_size; i++) {
			if (!std::binary_search(a_sub_route, a_sub_route + sub_route_size, second_parent.chromosome[i], city_cmp)) {
				if (ind_f == start_gene) ind_f = end_gene + 1;
				if (ind_f < chromosome_size) {
					first_child.chromosome[ind_f] = second_parent.chromosome[i];
					ind_f++;
				}
			}
			if (!std::binary_search(b_sub_route, b_sub_route + sub_route_size, first_parent.chromosome[i], city_cmp)) {
				if (ind_s == start_gene) ind_s = end_gene + 1;
				if (ind_s < chromosome_size) {
					second_child.chromosome[ind_s] = first_parent.chromosome[i];
					ind_s++;
				}
			}
		}
		delete[] a_sub_route;
		delete[] b_sub_route;
		return std::make_pair(first_child, second_child);
	}
}