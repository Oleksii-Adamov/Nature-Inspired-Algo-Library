#include "Ant.h"
namespace nia {
	namespace ant {
		void Ant::set_number_of_vertices(size_t input_number_of_vertices)
		{
			number_of_vertices = input_number_of_vertices;
			visited = new bool[number_of_vertices];
			for (int i = 0; i < number_of_vertices; i++) {
				visited[i] = false;
			}
			trail = new size_t[number_of_vertices];
		}
		Ant::Ant() {
			// do nothing
		}
		Ant::~Ant() {
			delete[] visited;
			delete[] trail;
		}
		bool Ant::is_visited(size_t vertex) {
			return visited[vertex];
		}
		void Ant::visit(size_t trail_index, size_t vertex) {
			trail[trail_index] = vertex;
			visited[vertex] = true;
		}
		void Ant::calc_trail_length(double** graph) {
			trail_length = 0;
			for (size_t i = 0; i < number_of_vertices - 1; i++) {
				trail_length += graph[trail[i]][trail[i + 1]];
			}
			trail_length += graph[trail[number_of_vertices - 1]][trail[0]];
		}
		double Ant::get_trail_length(double** graph) {
			if (trail_length < 0) calc_trail_length(graph);
			return trail_length;
		}
		void Ant::get_route(size_t* route_buffer) {
			for (size_t i = 0; i < number_of_vertices - 1; i++) {
				route_buffer[i] = trail[i];
			}
			route_buffer[number_of_vertices] = trail[0];
		}
		void Ant::clear() {
			for (int i = 0; i < number_of_vertices; i++) {
				visited[i] = false;
				trail[i] = 0;
			}
			trail_length = -1;
		}
		Ant::Ant(const Ant& other) {
			number_of_vertices = other.number_of_vertices;
			trail_length = other.trail_length;
			if (trail == nullptr) {
				visited = new bool[number_of_vertices];
				trail = new size_t[number_of_vertices];
			}
			for (size_t i = 0; i < number_of_vertices; i++) {
				trail[i] = other.trail[i];
				visited[i] = other.visited[i];
			}
		}
		Ant& Ant::operator=(const Ant& other) {
			if (this != &other) {
				number_of_vertices = other.number_of_vertices;
				trail_length = other.trail_length;
				if (trail == nullptr) {
					visited = new bool[number_of_vertices];
					trail = new size_t[number_of_vertices];
				}
				for (size_t i = 0; i < number_of_vertices; i++) {
					trail[i] = other.trail[i];
					visited[i] = other.visited[i];
				}
			}
			return *this;
		}
		Ant::Ant(Ant&& other) noexcept {
			number_of_vertices = other.number_of_vertices;
			trail_length = other.trail_length;
			trail = other.trail;
			visited = other.visited;
			other.trail = nullptr;
			other.visited = nullptr;
		}
		Ant& Ant::operator=(Ant&& other) noexcept {
			if (this != &other) {
				number_of_vertices = other.number_of_vertices;
				trail_length = other.trail_length;
				trail = other.trail;
				visited = other.visited;
				other.trail = nullptr;
				other.visited = nullptr;
			}
			return *this;
		}
	}
}