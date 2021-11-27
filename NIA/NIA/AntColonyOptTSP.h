#pragma once
namespace nia {
	namespace ant {
		class AntColonyOptTSP
		{
		private:
			double exponent_of_pheromone;
			double exponent_of_distance;
			double evaporation_factor;
		public:
			double solve(double** graph);
		};
	}
}

