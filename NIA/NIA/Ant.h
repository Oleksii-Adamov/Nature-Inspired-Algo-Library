#pragma once
/*!
\file
\brief Header for Ant class
*/
namespace nia {
	namespace ant {
		class Ant
		{
		private:
			bool* visited = nullptr;
			size_t number_of_vertices;
			double trail_length = -1;
			void calc_trail_length(double** graph);
		public:
			size_t* trail = nullptr;
			Ant();
			void set_number_of_vertices(size_t input_number_of_vertices);
			~Ant();
			bool is_visited(size_t vertex);
			void visit(size_t trail_index, size_t vertex);
			double get_trail_length(double** graph);
			/// <summary>
			/// writes route of this ant in route_buffer 
			/// </summary>
			/// \param[in] route_buffer has to have size at least equal to number_of_vertices + 1
			void get_route(size_t* route_buffer);
			void clear();
			Ant(const Ant& other);
			Ant& operator=(const Ant& other);
			Ant(Ant&& other) noexcept;
			Ant& operator=(Ant&& other) noexcept;
		};
	}
}

