#pragma once
#include "../NIA/GeneticAlgo.h"
#include <algorithm>
#include <random>
#include <ctime>
#include <iostream>
#include "Visualization.h"
namespace gata {
	const double EPS = 1e-9;
	struct City { // or Gene
		int num;
		double x, y;
	private:
	public:
		static double distance(City a, City b) {
			double delta_x = a.x - b.x;
			double delta_y = a.y - b.y;
			return sqrt(delta_x * delta_x + delta_y * delta_y);
		}
	};
	template<int chromosome_size>
	class Individual : public nia::ga::IndividualInteface<City, chromosome_size, double> { // or Route
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
			swap_cities(this->chromosome[i], this->chromosome[j]);
		}
		void calc_route_length() {
			route_length = 0;
			for (int i = 0; i < chromosome_size; i++) {
				if (i + 1 != chromosome_size) {
					route_length += City::distance(this->chromosome[i], this->chromosome[i + 1]);
				}
				else {
					route_length += City::distance(this->chromosome[i], this->chromosome[0]);
				}
			}
		}
		void calc_fitness() {
			fitness = 1 / ((double)get_route_length());
		}
	public:
		void mutate(double mutation_chance, std::mt19937& gen) override {
			std::uniform_real_distribution<> chance_dis(0.0, 1.0);
			std::uniform_int_distribution<> pos_dis(0, chromosome_size - 1);
			for (int i = 0; i < chromosome_size; i++) {
				if (chance_dis(gen) < mutation_chance) {
					int j = pos_dis(gen);
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
		std::random_device rd;  //Will be used to obtain a seed for the random number engine
		std::seed_seq seed{ rd(), static_cast<unsigned int>(time(nullptr)) }; // in case random_device is not implemented, fall back to time(0)
		std::mt19937 gen(seed); //Standard mersenne_twister_engine seeded with seed
		for (int i = 0; i < NUMBER_OF_INDIVIDUALS; i++) {
			std::shuffle(cities, cities + NUMBER_OF_CITIES, gen);
			copy(NUMBER_OF_CITIES, cities, population[i].chromosome);
		}
	};
	template<int chromosome_size>
	std::pair<Individual<chromosome_size>, Individual<chromosome_size>> breed ( const Individual<chromosome_size>& first_parent,
		const Individual<chromosome_size>& second_parent, std::mt19937& gen) {
		auto city_cmp = [](const City& a, const City& b) {
			return a.num < b.num;
		};
		Individual<chromosome_size> first_child;
		Individual<chromosome_size> second_child;
		std::uniform_int_distribution<> pos_dis(0, chromosome_size - 1);
		int geneA = pos_dis(gen), geneB = pos_dis(gen);
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
	template<int CHROMOSOME_SIZE>
	double testTa (const int NUMBER_OF_INDIVIDUALS, const int NUMBER_OF_GENERATIONS, const int NUMBER_OF_ELITES,
		const double MUTATION_CHANCE, nia::ga::SELECTION selection, City cities[]) {
		Individual<CHROMOSOME_SIZE>* population = new Individual<CHROMOSOME_SIZE>[NUMBER_OF_INDIVIDUALS];
		init_population(CHROMOSOME_SIZE, NUMBER_OF_INDIVIDUALS, population, cities);
		Individual<CHROMOSOME_SIZE> the_fittest = nia::ga::GeneticAlgo<Individual<CHROMOSOME_SIZE>>::solve(NUMBER_OF_INDIVIDUALS,
			NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, MUTATION_CHANCE, population, breed, selection);
		std::cerr << "[          ] ";
		for (int i = 0; i < CHROMOSOME_SIZE; i++) {
			std::cerr << the_fittest.chromosome[i].num << " -> ";
		}
		delete[] population;
		return the_fittest.get_route_length();
	}

	template<int CHROMOSOME_SIZE>
	Individual<CHROMOSOME_SIZE> calc_ans(const int NUMBER_OF_INDIVIDUALS, const int NUMBER_OF_GENERATIONS, const int NUMBER_OF_ELITES,
		const double MUTATION_CHANCE, nia::ga::SELECTION selection, City cities[], vis::Visualization* visualization_ptr = nullptr, const int correct_ans = -1) {
		double correct_fitness = -1;
		if (correct_ans != -1) {
			correct_fitness = 1 / ((double)correct_ans);
		}
		Individual<CHROMOSOME_SIZE>* population = new Individual<CHROMOSOME_SIZE>[NUMBER_OF_INDIVIDUALS];
		init_population(CHROMOSOME_SIZE, NUMBER_OF_INDIVIDUALS, population, cities);
		Individual<CHROMOSOME_SIZE> the_fittest = nia::ga::GeneticAlgo<Individual<CHROMOSOME_SIZE>>::solve(NUMBER_OF_INDIVIDUALS,
			NUMBER_OF_ELITES, NUMBER_OF_GENERATIONS, MUTATION_CHANCE, population, breed, selection, visualization_ptr,
			correct_fitness + 0.05*correct_fitness, correct_fitness);
		delete[] population;
		return the_fittest;
	}
}