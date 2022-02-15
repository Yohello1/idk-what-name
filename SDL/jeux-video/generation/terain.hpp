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
			points_inbetween(6);
			remove_doubles();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			smoothen();
			
			std::cout << "The size of the vector/array is " << points_new.size() << '\n';

			std::cout << "This ended" << '\n';
			for (int unsigned i = 1; i < points_old.size(); i++)
			{
				pixels[points_old[i].x_pos][points_old[i].y_pos].r = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].g = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].b = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].a = 255;
				pixels[points_old[i].x_pos][points_old[i].y_pos].state_now = fixed_pos;
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
				points_old.push_back(temp);
			}
		}

		void points_inbetween(int times)
		{
			for (int unsigned i = 1; i < points_old.size() - 1; i++)
			{
				std::cout << points_old[i].x_pos << ',' << points_old[i].y_pos << "  I: " << i << '\n';
			}

			for (int unsigned i = 1; i < points_old.size(); i++)
			{

				int x_mid = (points_old[i - 1].x_pos + points_old[i].x_pos) / 2;
				int y_mid = (points_old[i - 1].y_pos + points_old[i].y_pos) / 2;

				points_new.push_back(points_old[i - 1]);

				cord_2d temp;
				temp.x_pos = x_mid;
				temp.y_pos = y_mid;
				points_new.push_back(temp);

				points_new.push_back(points_old[i]);
			}

			for (int unsigned i = 0; i < points_new.size(); i++)
			{
				std::cout << points_new[i].x_pos << ',' << points_new[i].y_pos << "  I: " << i << '\n';
			}

			std::cout << "Filling in time" << '\n';
			points_old = points_new;

			for (int times_done = 0; times_done < times; times_done++)
			{
				for (int unsigned i = 1; i < points_old.size() - 5; i++)
				{
					// I have no clue, this is the only thing that stops the streak across the window
					if (points_old[i].x_pos > points_old[i - 1].x_pos)
					{
						int x_mid = (points_old[i - 1].x_pos + points_old[i].x_pos) / 2;
						int y_mid = (points_old[i - 1].y_pos + points_old[i].y_pos) / 2;

						points_new.push_back(points_old[i - 1]);

						cord_2d temp;
						temp.x_pos = x_mid;
						temp.y_pos = y_mid;

						points_new.push_back(temp);
						points_new.push_back(points_old[i]);
					}
					else
					{
						points_new.push_back(points_old[i - 1]);
						points_new.push_back(points_old[i]);
					}
				}

				points_old = points_new;

				for (int unsigned i = 0; i < points_new.size(); i++)
				{
					std::cout << points_new[i].x_pos << ',' << points_new[i].y_pos << "  I: " << i << '\n';
				}

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

		void remove_doubles()
		{
			bool changes_0 = false;
			while (changes_0 == false)
			{
				int i = 0;
				int j = 0;

				points_new.clear();
				points_new.push_back(points_old[0]);
				for (int unsigned i = 1; i < points_old.size(); i++)
				{
					if (points_old[i].x_pos == points_old[i - 1].x_pos)
					{
						// Do nothing lol
						i++;
						// if(points_new[j].x_pos != points_old[i].x_pos && points_new[j].y_pos != points_old[i].y_pos)
						// {
						// 	points_new.push_back(points_old[i]);
						// 	j++;
						// }
					}
					else
					{
						points_new.push_back(points_old[i]);
						j++;
					}
				}

				if (i == 0)
				{
					changes_0 = true;
				}
				points_old = points_new;
			}
		}

		void smoothen()
		{
			points_new.clear();
			points_new.push_back(points_old[0]);
			for (unsigned i = 2; i < points_old.size() - 2; i++)
			{
				int y_previous = points_old[i - 2].y_pos;
				int y_next = points_old[i + 2].y_pos;

				cord_2d temp;
				temp.x_pos = points_old[i].x_pos;
				temp.y_pos = (y_next + y_previous) / 2;

				points_new.push_back(temp);
			}
			// points_new.push_back(points_old.back());

			points_old = points_new;
		}
	};

}