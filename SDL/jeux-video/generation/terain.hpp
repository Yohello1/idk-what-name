// #include "../base_data.hpp"
namespace Terrain
{
	class Development
	{
	public:
		// Variables used go here

		std::vector<cord_2d> points_original;
		std::vector<cord_2d> points_old;
		std::vector<cord_2d> points_new;

		// Public functions go here
		//  void print_test()
		//  {
		//      std::cout << "Hello does this work?" << '\n';
		//  }

		void generate_terrain(int x_lower, int y_lower, int x_upper, int y_upper, int amt_points, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
		{
			std::cout << "This started" << '\n';
			place_dots(x_lower, y_lower, x_upper, y_upper, amt_points);
			points_inbetween(2);

			std::cout << "This ended" << '\n';
			for (int i = 1; i < points_old.size(); i++)
			{
				pixels[points_old[i].x_pos][points_old[i].y_pos].r = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].g = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].b = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].a = 255;
			}
		}

	private:
		void place_dots(int x_lower, int y_lower, int x_upper, int y_upper, int amt_points)
		{
			std::cout << "Placing points" << '\n';
			amt_points -= 1;
			int x_spacing = (x_lower - x_upper) / amt_points * -1;
			for (int i = 0; i < amt_points + 1; i++)
			{
				int y_pos = random_y_pos(y_lower, y_upper);
				// pixels[i*x_spacing][y_pos].r = 255;
				// pixels[i*x_spacing][y_pos].g = 255;
				// pixels[i*x_spacing][y_pos].b = 255;
				// pixels[i*x_spacing][y_pos].a = 255;

				cord_2d temp;
				temp.x_pos = i * x_spacing;
				temp.y_pos = y_pos;
				points_original.push_back(temp);
			}
		}

		void points_inbetween(int times)
		{
			for (int i = 1; i < points_original.size(); i++)
			{
				int x_mid = (points_original[i - 1].x_pos + points_original[i].x_pos) / 2;
				int y_mid = (points_original[i - 1].y_pos + points_original[i].y_pos) / 2;

				points_new.push_back(points_original[i - 1]);

				cord_2d temp;
				temp.x_pos = x_mid;
				temp.y_pos = y_mid;
				points_new.push_back(temp);

				points_new.push_back(points_original[i]);
			}

			points_old = points_new;

			for(int times_done = 0; times_done < times; times_done++)
			{
				for (int i = 0; i < points_old.size(); i++)
				{
					int x_mid = (points_old[i - 1].x_pos + points_old[i].x_pos) / 2;
					int y_mid = (points_old[i - 1].y_pos + points_old[i].y_pos) / 2;

					points_new.push_back(points_old[i - 1]);

					cord_2d temp;
					temp.x_pos = x_mid;
					temp.y_pos = y_mid;

					points_new.push_back(temp);
					points_new.push_back(points_old[i]);

					std::cout << points_new[i].x_pos << '\n';
				}

				points_old = points_new;

				for (int i = 0; i < points_new.size(); i++)
					std::cout << points_old[i].y_pos  << ',' << points_old[i].x_pos <<  "  I: " << i << '\n';

				points_new.clear();
			}
		}

		int random_y_pos(int lower, int upper)
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<double> dist(lower, upper);

			double output_double = dist(mt);
			return (int)output_double;
		}
	};

}