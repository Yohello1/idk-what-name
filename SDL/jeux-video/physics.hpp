namespace Physics
{ // #include "render.hpp"
	// #include "base_data.hpp"

	// No clue what this is for lol
	std::mutex mtx;

	void sand_and_water(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{
		mtx.lock();

		// it works don't ask
		for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
		{
			for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
			{
				// sand ig

				if (
					pixels[x_pos][y_pos].state_now == solid &&
					pixels[x_pos][y_pos + 1].state_now == fluid &&
					pixels[x_pos][y_pos + 1].density < pixels[x_pos][y_pos].density &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					position temp_4_swap = pixels[x_pos][y_pos + 1];
					new_version[x_pos][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos][y_pos + 1].density = pixels[x_pos][y_pos].density;

					new_version[x_pos][y_pos].r = temp_4_swap.r;
					new_version[x_pos][y_pos].g = temp_4_swap.g;
					new_version[x_pos][y_pos].b = temp_4_swap.b;
					new_version[x_pos][y_pos].a = temp_4_swap.a;
					new_version[x_pos][y_pos].state_now = temp_4_swap.state_now;
					new_version[x_pos][y_pos].temperature = temp_4_swap.temperature;
					new_version[x_pos][y_pos].pressure = temp_4_swap.pressure;
					new_version[x_pos][y_pos].density = temp_4_swap.density;
				}
				if (
					pixels[x_pos][y_pos].state_now == solid &&
					pixels[x_pos][y_pos - 1].state_now == fluid &&
					pixels[x_pos][y_pos - 1].density > pixels[x_pos][y_pos].density &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					position temp_4_swap = pixels[x_pos][y_pos - 1];
					new_version[x_pos][y_pos - 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos][y_pos - 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos][y_pos - 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos][y_pos - 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos][y_pos - 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos][y_pos - 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos][y_pos - 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos][y_pos - 1].density = pixels[x_pos][y_pos].density;

					new_version[x_pos][y_pos].r = temp_4_swap.r;
					new_version[x_pos][y_pos].g = temp_4_swap.g;
					new_version[x_pos][y_pos].b = temp_4_swap.b;
					new_version[x_pos][y_pos].a = temp_4_swap.a;
					new_version[x_pos][y_pos].state_now = temp_4_swap.state_now;
					new_version[x_pos][y_pos].temperature = temp_4_swap.temperature;
					new_version[x_pos][y_pos].pressure = temp_4_swap.pressure;
					new_version[x_pos][y_pos].density = temp_4_swap.density;
				}

				else if (
					pixels[x_pos][y_pos].state_now == solid &&
					pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					new_version[x_pos][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}
				else if (
					pixels[x_pos][y_pos].state_now == solid &&
					pixels[x_pos + 1][y_pos + 1].state_now == empty && new_version[x_pos + 1][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					new_version[x_pos + 1][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos + 1][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos + 1][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos + 1][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos + 1][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos + 1][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos + 1][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos + 1][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (
					pixels[x_pos][y_pos].state_now == solid &&
					pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1)
				{
					new_version[x_pos - 1][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos - 1][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos - 1][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos - 1][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos - 1][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos - 1][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos - 1][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos - 1][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				// Water oooo
				else if (
					pixels[x_pos][y_pos].state_now == fluid &&
					pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{					new_version[x_pos][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (
					pixels[x_pos][y_pos].state_now == fluid &&
					pixels[x_pos + 1][y_pos + 1].state_now == empty && new_version[x_pos + 1][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					new_version[x_pos + 1][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos + 1][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos + 1][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos + 1][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos + 1][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos + 1][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos + 1][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos + 1][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (
					pixels[x_pos][y_pos].state_now == fluid &&
					pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty &&
					y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1)
				{
					new_version[x_pos - 1][y_pos + 1].r = pixels[x_pos][y_pos].r;
					new_version[x_pos - 1][y_pos + 1].g = pixels[x_pos][y_pos].g;
					new_version[x_pos - 1][y_pos + 1].b = pixels[x_pos][y_pos].b;
					new_version[x_pos - 1][y_pos + 1].a = pixels[x_pos][y_pos].a;
					new_version[x_pos - 1][y_pos + 1].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos - 1][y_pos + 1].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos - 1][y_pos + 1].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos - 1][y_pos + 1].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (
					pixels[x_pos][y_pos].state_now == fluid &&
					pixels[x_pos + 1][y_pos].state_now == empty && new_version[x_pos + 1][y_pos].state_now == empty &&
					x_pos != (LOGICAL_WINDOW_WIDTH - 1))
				{
					new_version[x_pos + 1][y_pos].r = pixels[x_pos][y_pos].r;
					new_version[x_pos + 1][y_pos].g = pixels[x_pos][y_pos].g;
					new_version[x_pos + 1][y_pos].b = pixels[x_pos][y_pos].b;
					new_version[x_pos + 1][y_pos].a = pixels[x_pos][y_pos].a;
					new_version[x_pos + 1][y_pos].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos + 1][y_pos].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos + 1][y_pos].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos + 1][y_pos].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (
					pixels[x_pos][y_pos].state_now == fluid &&
					pixels[x_pos - 1][y_pos].state_now == empty && new_version[x_pos - 1][y_pos].state_now == empty &&
					x_pos != 1)
				{
					new_version[x_pos - 1][y_pos].r = pixels[x_pos][y_pos].r;
					new_version[x_pos - 1][y_pos].g = pixels[x_pos][y_pos].g;
					new_version[x_pos - 1][y_pos].b = pixels[x_pos][y_pos].b;
					new_version[x_pos - 1][y_pos].a = pixels[x_pos][y_pos].a;
					new_version[x_pos - 1][y_pos].state_now = pixels[x_pos][y_pos].state_now;
					new_version[x_pos - 1][y_pos].temperature = pixels[x_pos][y_pos].temperature;
					new_version[x_pos - 1][y_pos].pressure = pixels[x_pos][y_pos].pressure;
					new_version[x_pos - 1][y_pos].density = pixels[x_pos][y_pos].density;


					pixels[x_pos][y_pos].r = 0;
					pixels[x_pos][y_pos].g = 0;
					pixels[x_pos][y_pos].b = 0;
					pixels[x_pos][y_pos].a = 0;
					pixels[x_pos][y_pos].state_now = empty;
					pixels[x_pos][y_pos].temperature = 0;
					pixels[x_pos][y_pos].pressure = 0;
					pixels[x_pos][y_pos].density = 0;
				}

				else if (pixels[x_pos][y_pos].state_now != empty && new_version[x_pos][y_pos].state_now == empty)
				{
					new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
				}
			}
		}
		// memcpy(&pixels, &new_version, sizeof(pixels));
		for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
		{
			for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
			{
				pixels[x_pos][y_pos] = new_version[x_pos][y_pos];
				new_version[x_pos][y_pos].r = 0;
				new_version[x_pos][y_pos].g = 0;
				new_version[x_pos][y_pos].b = 0;
				new_version[x_pos][y_pos].a = 0;
				new_version[x_pos][y_pos].state_now = empty;
				new_version[x_pos][y_pos].temperature = 0;
				new_version[x_pos][y_pos].pressure = 0;
			}
		}
		mtx.unlock();
	}

	void draw_box_white_sand(cord_2d start, cord_2d end, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{
		// The location is relative to where it is on the screen/window :P
		// That made no sense
		int higher_x = std::min(start.x_pos, end.x_pos);
		int higher_y = std::min(start.y_pos, end.y_pos);
		int lower_x = std::max(start.x_pos, end.y_pos);
		int lower_y = std::max(start.y_pos, end.y_pos);

		for (int x_pos = higher_x; x_pos < lower_x; x_pos++)
		{
			for (int y_pos = higher_y; y_pos < lower_y; y_pos++)
			{
				pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].a = 255;
				pixels[x_pos][y_pos].state_now = solid;
			}
		}
	}

	/*
	Gotta eat every bagguete, gotta eat your grains
	Oranges too
	I wonder if you could eat leafs
	Probably, they're edible? Maybe? Whacky
	In other news there is a water bottle which needs to be filled
	*/
	

	void simulate(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{

		while (!quit_now)
		{
			auto start_time = Clock::now();

			sand_and_water(pixels, new_version);

			/*
        	* FIXME My I better come back and take out the fluid related code from sand and move that into it's own function so there aren't
        	* a ton of random eqautions just thrown everywehre and anywhere they fit
        	*/
			// water_sim(pixels, new_version);

			auto end_time = Clock::now();

			std::cout << "Quit Statys " << quit_now << '\n';

			if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 33333333)
			{
				SDL_Delay((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 1000000);
			}
		}

		std::cout << "It don't matter it done" << '\n';
	}

	void simulate_once(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{

		auto start_time = Clock::now();

		sand_and_water(pixels, new_version);

		auto end_time = Clock::now();

		if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 22222)
		{
			SDL_Delay((22222 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 100000);
		}

		std::cout << "It don't matter it done" << '\n';
	}
}