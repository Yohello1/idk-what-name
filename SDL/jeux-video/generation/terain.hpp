// #include "../base_data.hpp"
namespace Terrain
{
	class Development
	{
	public:
		// Variables used go here
		
		
		//Public functions go here
		// void print_test()
		// {
		//     std::cout << "Hello does this work?" << '\n';
		// }

		void generate_terrain(int x_lower, int y_lower, int x_upper, int y_upper, int amt_points,position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
		{
			std::cout << "This started" << '\n';
			place_dots(x_lower, y_lower, x_upper, y_upper, amt_points, pixels);
			std::cout << "This ended" << '\n';

		}

	private:
	
		void place_dots(int x_lower, int y_lower, int x_upper, int y_upper, int amt_points,position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
		{
			std::cout << "Placing points" << '\n';
			int x_spacing = (x_lower - x_upper)/amt_points*-1;
			for(int i = 0; i < amt_points; i++)
			{
				int y_pos = random_y_pos(y_lower, y_upper);
				std::cout << i*x_spacing << '\n';
				pixels[i*x_spacing][y_pos].r = 255;
				pixels[i*x_spacing][y_pos].g = 255;
				pixels[i*x_spacing][y_pos].b = 255;
				pixels[i*x_spacing][y_pos].a = 255;
				std::cout << "I is: " << i << '\n';
			}
		}

		int random_y_pos(int lower, int upper)
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> dist(lower, upper);

			double output_double = dist(mt);
			std::cout << output_double << '\n';
			return (int) output_double;
		}
	};

}