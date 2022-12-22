namespace Physics
{
	bool chunk_checker[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8];
	// bool chunk_empty[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8];
	// No clue what this is for lol
	std::mutex mtx;
	void clear_data(uint16_t x_pos, uint16_t y_pos, position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{
		pixels[x_pos][y_pos].r = 0;
		pixels[x_pos][y_pos].g = 0;
		pixels[x_pos][y_pos].b = 0;
		pixels[x_pos][y_pos].a = 0;
		pixels[x_pos][y_pos].state_now = empty;
		pixels[x_pos][y_pos].temperature = 0;
		pixels[x_pos][y_pos].pressure = 0;
		pixels[x_pos][y_pos].density = 0;
	}

	// TODO: Make chunk copier

	void chunk_copier(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], u_int16_t x_chunk, u_int16_t y_chunk)
	{
		// TODO: Switch this to memcpy
		for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
		{
			for (int x_pos = x_chunk * 8; x_pos < x_chunk * 8 + 8; x_pos++)
			{
				new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
			}
		}
	}

	void sand_and_water(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{
		for (int y_chunk = (LOGICAL_WINDOW_WIDTH / 8); y_chunk > 0; y_chunk--)
		{
			for (int x_chunk = (LOGICAL_WINDOW_WIDTH / 8); x_chunk > 0; x_chunk--)
			{
				if (chunk_checker[x_chunk][y_chunk] == true)
				{
					int un_changed = 0;
					// it works don't ask
					// Started from the bottom, now we here (hint)
					mtx.lock();
					for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
					{
						for (int x_pos = x_chunk * 8; x_pos < x_chunk * 8 + 8; x_pos++)
						{
							// Note: X is current one, y is the one being checked
							/* Swaps
							 * 0 0 0
							 * 0 X 0
							 * 0 Y 0
							 */

							if (
								pixels[x_pos][y_pos].state_now == solid &&
								pixels[x_pos][y_pos + 1].state_now == fluid &&
								pixels[x_pos][y_pos + 1].density < pixels[x_pos][y_pos].density &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								// TODO: Remove the first line here, can just move it down
								position temp_4_swap = pixels[x_pos][y_pos + 1];
								new_version[x_pos][y_pos + 1] = pixels[x_pos][y_pos];
								new_version[x_pos][y_pos] = temp_4_swap;
							}

							/* Swaps
							 * 0 0 0
							 * 0 y 0
							 * 0 X 0
							 */
							if (
								pixels[x_pos][y_pos].state_now == solid &&
								pixels[x_pos][y_pos - 1].state_now == fluid &&
								pixels[x_pos][y_pos - 1].density > pixels[x_pos][y_pos].density &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								position temp_4_swap = pixels[x_pos][y_pos - 1];
								new_version[x_pos][y_pos - 1] = pixels[x_pos][y_pos];
								new_version[x_pos][y_pos] = temp_4_swap;
							}

							// Everything below this just moves it to relaveant position
							/*
							 * 0 X 0
							 * 0 Y 0
							 * 0 0 0
							 */

							else if (
								pixels[x_pos][y_pos].state_now == solid &&
								pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								new_version[x_pos][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);
								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[(x_pos / 8)][((y_pos + 1) / 8)] = true;
							}
							/*
							 * 0 X 0
							 * 0 0 Y
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == solid &&
								pixels[x_pos + 1][y_pos + 1].state_now == empty && 
								new_version[x_pos + 1][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1) && 
								x_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								new_version[x_pos + 1][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos + 1) / 8)][((y_pos + 1) / 8)] = true;
							}

							/*
							 * 0 X 0
							 * Y 0 0
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == solid &&
								pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1)
							{
								new_version[x_pos - 1][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos - 1) / 8)][((y_pos + 1) / 8)] = true;
							}

							// Water oooo
							/*
							 * 0 X 0
							 * 0 Y 0
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == fluid &&
								pixels[x_pos][y_pos + 1].state_now == empty && new_version[x_pos][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								new_version[x_pos][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos) / 8)][((y_pos + 1) / 8)] = true;
							}
							/*
							 * 0 X 0
							 * 0 0 Y
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == fluid &&
								pixels[x_pos + 1][y_pos + 1].state_now == empty && new_version[x_pos + 1][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								new_version[x_pos + 1][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos + 1) / 8)][((y_pos + 1) / 8)] = true;
							}
							/*
							 * 0 X 0
							 * Y 0 0
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == fluid &&
								pixels[x_pos - 1][y_pos + 1].state_now == empty && new_version[x_pos - 1][y_pos + 1].state_now == empty &&
								y_pos != (LOGICAL_WINDOW_WIDTH - 1) && x_pos != 1)
							{
								new_version[x_pos - 1][y_pos + 1] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos - 1) / 8)][((y_pos + 1) / 8)] = true;
							}
							/*
							 * 0 X Y
							 * 0 0 0
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == fluid &&
								pixels[x_pos + 1][y_pos].state_now == empty && new_version[x_pos + 1][y_pos].state_now == empty &&
								x_pos != (LOGICAL_WINDOW_WIDTH - 1))
							{
								new_version[x_pos + 1][y_pos] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos + 1) / 8)][((y_pos) / 8)] = true;
							}

							/*
							 * Y X 0
							 * 0 0 0
							 * 0 0 0
							 */
							else if (
								pixels[x_pos][y_pos].state_now == fluid &&
								pixels[x_pos - 1][y_pos].state_now == empty && new_version[x_pos - 1][y_pos].state_now == empty &&
								x_pos != 1)
							{
								new_version[x_pos - 1][y_pos] = pixels[x_pos][y_pos];

								clear_data(x_pos, y_pos, pixels);

								// chunk_checker[(x_pos / 8)][(y_pos / 8)] = false;
								chunk_checker[((x_pos - 1) / 8)][((y_pos) / 8)] = true;
							}
							// If this pixel didnt experience any changes, then use the same data, simple
							else if (pixels[x_pos][y_pos].state_now != empty && new_version[x_pos][y_pos].state_now == empty)
							{
								new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
								// un_changed++;
							}
							else if (pixels[x_pos][y_pos].state_now == empty && new_version[x_pos][y_pos].state_now == empty)
							{
								new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
								un_changed++;
							}
						}
					}

					if (un_changed == 64)
					{
						chunk_checker[x_chunk][y_chunk] = false;
						std::cout << "Empty chunk" << '\n';
					}
					mtx.unlock();
				}
				else if (chunk_checker[x_chunk][y_chunk] == false)
				{
					chunk_copier(pixels, new_version, x_chunk, y_chunk);
					std::cout << "Being copied" << '\n';
				}
			}
		}

		// Clears data, need to replace with memcpy
		// TODO: Replace with memcpy
		// memcpy(&new_version, &pixels, sizeof(&pixels));
		// std::cout <<
		// Just for memcpy debugging
		// std::cout << "Memory addy " << &new_version << ' ' << &pixels << '\n';
		mtx.lock();

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

			if (recheck == true)
			{
				// chunk_checker.fill(true);
				// array::fill
				// chunk_checker.assign(chunk_checker.size(), false);
				memset(chunk_checker, 1, sizeof(chunk_checker));
				recheck = false;
			}

			// memset(chunk_checker, 1, sizeof(chunk_checker));

			sand_and_water(pixels, new_version);

			auto end_time = Clock::now();

			// TODO: Change this over to a proper delay thingy
			if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 33333333)
			{
				// SDL_Delay((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 1000000);
				std::this_thread::sleep_for(std::chrono::nanoseconds((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count())));
			}
		}

		std::cout << "It don't matter it done" << '\n';
	}

	void simulate_once(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
	{

		auto start_time = Clock::now();

		sand_and_water(pixels, new_version);

		auto end_time = Clock::now();

		// if it took less than 1/30th of a second
		// Wait for the difference
		if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 22222)
		{
			// SDL_Delay((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 100000);
			std::chrono::sleep_until(system_clock::now() + milisecond(end_time - start_time));
		}
	}
}