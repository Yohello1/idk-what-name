/*
By Yohwllooooo

Created: 2022-08-20
Last modified: 2022-08-20

Modifications:
Creating the file, and adding all but simulation function
Creating comments to explain search/simulation thing
*/

namespace physics
{
    // Remember what chunks need to checked, and which ones can be skipped
    bool chunk_checker[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8];
    // A mutex lmfao
    std::mutex mtx;

    // // Clears the data of a pixel
    // NOTE: DOES NOT WORK
    // void clear_data(cell the_pixel)
    // {
    //     memset(the_pixel, 0, sizeof(the_pixel));
    // }

    void chunk_copier(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], u_int16_t x_chunk, u_int16_t y_chunk)
    {

        // for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
        // {
        //     // Cannot memcpy classes
        //     memcpy(new_version[x_chunk * 8][y_pos], pixels[x_chunk * 8][y_pos], sizeof(new_version[0][0])*8);
        // }

        for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
        {
            for (int x_pos = x_chunk * 8; x_pos > (x_chunk * 8 - 8); x_pos--)
            {
                new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
            }
        }
    }
    // Hao, Mandy, and Chloe are nice tbh
    // I liked talking and playing w them
    // I wish I coulda played with them too
    // O I cant forget Oz, Ant, Sophia, Mickey and the others
    // They're just nice to be with, I wish we could be friends forever?
    // Hopefully that curse finally gets exorcised, or I can become a better person
    // Firstly I need to become nicer, and stop insulting so much
    // Especially as a knee jerk reaction

    /*
    Deprecated, not to be used
    */
    void draw_box_white_water(cord_2d start, cord_2d end, cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
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
                pixels[x_pos][y_pos].r = 212;
                pixels[x_pos][y_pos].g = 241;
                pixels[x_pos][y_pos].b = 249;
                pixels[x_pos][y_pos].a = 255;
                pixels[x_pos][y_pos].pressure = 10;
            }
        }
    }

    // I think I should also try and improve my friendship with my irl friend group?
    // Who am I kidding
    // We're far from friends
    // Probably something like, aquentices
    // I barely talk to them
    // I've talked to them like 4 times total
    // What's the point of being w them?
    // Should I try and find new friends?
    // I mean... Like I want to have a fun friend group
    // But I myself am kinda boring, and un-interesting
    // I hate programming 4 this reason
    // During Kayla's bday they said 'no talking abt programming, school, or dogs'
    // And that was basically all I had to talk abt
    // That's all I knew that they knew...
    // I don't even know how to fix this, it's not smth I can just do overnight
    // GOD PROGRAMMING IS SO BAD
    // How can I be better at holding a convo????
    // I cant even hold one w hao at this point, what hope do I have to hold it with other people
    // Dam, Im a horrible person to be a friend with, I cant hold a convo, nor do I really have much to talk abt
    // Why would anyone be friends with me???? At this point the only thing I can think of is cause they want smth

    // So we start with a grid of 256x256
    // Simulating all of it is hard, and takes a long time
    // So we divide it into 8x8 chunks to speed it up
    // The original grid is split into a 32x32 grid of chunks
    // I should learn chinese
    // Of course chunk 0x0, and 32x32 exist
    // To get around this when doing loops to do checks we do
    // s = start point
    // t = Given
    // c = current point
    // - OR |= border
    // + = corner
    /*
    +--------+
    |t       |
    |        |
    |        |
    |        |
    |        |
    |        |
    |       s|
    +--------+
    +--------+
    |t       |
    |        |
    |        |
    |        |
    |        |
    |        |
    |      cs|
    +--------+
    |t       |
    |        |
    |        |
    |        |
    |        |
    |        |
    |     c s|
    +--------+
    etc etc
    */
    // Using chunk * 8 we get it's min value, or the bottom right corner's value
    // From there, we just go to the right and to the left
    // To find the target point we use ((chunk*8)-8)

    // So many people I grew up with as role models, and saw like weekly have grown up
    // They've all moved, gotten jobs, homes, and such
    // Next thing I know, they'll have kids, and companies lmao

    // TODO: Use an Octree to store the data
    //  TODO: Turn this into a sort of queue, which mutliple threads can work on

    std::vector<float> differences;

    void cellular_automata(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {
        // Itterate through chunks
        // for (int y_chunk = (LOGICAL_WINDOW_WIDTH / 8); y_chunk > 0; y_chunk--)
        // {
        //     for (int x_chunk = (LOGICAL_WINDOW_WIDTH / 8); x_chunk > 0; x_chunk--)
        //     {

        //     }
        // }

        float total_val = 0;

        for (int y_pos = LOGICAL_WINDOW_WIDTH - 1; y_pos > 0; y_pos--)
        {
            for (int x_pos = LOGICAL_WINDOW_WIDTH - 1; x_pos > 0; x_pos--)
            {
                // TODO: When this is final, just use less declerations and get right to the point
                //  There is a faster way to do it, it's just messier
                cord_2d update_cord[4];
                update_cord[0].x_pos = x_pos;
                update_cord[1].x_pos = x_pos;
                update_cord[2].y_pos = y_pos;
                update_cord[3].y_pos = y_pos;

                update_cord[0].y_pos = std::min((y_pos - 1), LOGICAL_WINDOW_WIDTH - 1);
                update_cord[1].y_pos = std::min((y_pos + 1), LOGICAL_WINDOW_WIDTH - 1);
                update_cord[2].x_pos = std::min((x_pos - 1), LOGICAL_WINDOW_WIDTH - 1);
                update_cord[3].x_pos = std::min((x_pos + 1), LOGICAL_WINDOW_WIDTH - 1);

                // std::cout << '(' << x_pos << ',' << y_pos << ')' << '\n';

                for (int i = 0; i < 4; i++)
                {
                    if (pixels[update_cord[i].x_pos][update_cord[i].y_pos].inert != true)
                    {

                        if (pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure != pixels[x_pos][y_pos].pressure)
                        {
                            // Difference in pressure
                            int DPress = pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure - pixels[x_pos][y_pos].pressure;
                            DPress /= DPress;
                            // std::cout << DPress;
                            new_version[x_pos][y_pos] = pixels[x_pos][y_pos];

                            new_version[x_pos][y_pos].pressure = pixels[x_pos][y_pos].fetch_pressure() - DPress;
                            new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure = pixels[update_cord[i].x_pos][update_cord[i].y_pos].fetch_pressure() + DPress;


                            if( (new_version[x_pos][y_pos].pressure + new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure) != (pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure + pixels[x_pos][y_pos].pressure))
                            { 
                                std::cout << "ERROR PRESSURE IS NOT SAME" << '\n';
                                std::cout << (-1 * DPress) <<',' << DPress << '\n';

                            }

                            // new_version[x_pos][y_pos].pressure = pixels[x_pos][y_pos].pressure_change(DPress * -1);
                            // new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure = pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure_change(DPress);

                            // new_version[x_pos][y_pos].pressure = new_version[x_pos][y_pos].add(pixels[x_pos][y_pos].pressure, +DPress);
                            // new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure = pixels[update_cord[i].x_pos][update_cord[i].y_pos].add(pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure,-DPress);
                        }
                        else
                        {
                            new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
                        }

                        // // std::cout << "Running" << '\n';
                        // // Get the difference of pressure
                        // float DPress = pixels[x_pos][y_pos].pressure - pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure;

                        // // // Find some way so that I can do pressure*DPress*neigh.pressure/neigh.pressure
                        // // // But find a way to deal with being divided by 0
                        // float Flow = pixels[x_pos][y_pos].pressure * DPress;
                        // // // TODO: Change this to std::clamp when it starts compiling?
                        // Flow = boost::algorithm::clamp(Flow, pixels[x_pos][y_pos].pressure / 6.0f, (-1 * pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure) / 6.0f);
                        // // new_version[x_pos][y_pos].pressure = pixels[x_pos][y_pos].pressure - Flow;
                        // pixels[x_pos][y_pos].pressure = pixels[x_pos][y_pos].add(pixels[x_pos][y_pos].pressure, -Flow);
                        // new_version[x_pos][y_pos].pressure = pixels[x_pos][y_pos].pressure;
                        // new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure = new_version[update_cord[i].x_pos][update_cord[i].y_pos].add(new_version[update_cord[i].x_pos][update_cord[i].y_pos].pressure, Flow);

                        // // pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure += Flow;

                        // // new_version[x_pos][y_pos] = pixels[x_pos][y_pos];

                        // // if (pixels[x_pos][y_pos].pressure > 0)
                        // // {
                        // //     std::cout << "Change" << std::setw(5) << pixels[x_pos][y_pos].pressure << ',' << pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure << '\n';
                        // //     // pixels[x_pos][y_pos].pressure -= 0.1;
                        // //     pixels[x_pos][y_pos].pressure = pixels[x_pos][y_pos].add(pixels[x_pos][y_pos].pressure, -.01);
                        // // }
                        // // if (DPress != 0 && pixels[x_pos][y_pos].pressure > 0)
                        // // {
                        // //     std::cout << "Change" << Flow << ',' << DPress << '\n' << pixels[x_pos][y_pos].pressure  << pixels[x_pos][y_pos].pressure << ',' << pixels[update_cord[i].x_pos][update_cord[i].y_pos].pressure << '\n';
                        // // }
                        // // if( pixels[x_pos][y_pos].pressure !=)

                        total_val += pixels[x_pos][y_pos].pressure;
                    }
                    else
                    {
                        // new_version[x_pos][y_pos] = pixels[x_pos][y_pos];
                    }
                }
                // std::cout << new_version[x_pos][y_pos].pressure << ',';
            }
            // std::cout << '\n';
        }
        // std::cout << '\n';
        // std::cout << '\n';

        differences.push_back(total_val);

        // pixels.std::swap (new_version);
        // std::swap(pixels, new_version);
        // std::memcpy(pixels, new_version, sizeof(new_version[0][0]) * LOGICAL_WINDOW_WIDTH * LOGICAL_WINDOW_WIDTH);

        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
            {
                pixels[x_pos][y_pos] = new_version[x_pos][y_pos];
            }
        }
    }

    // Must be final function
    void simulate(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {
        while (!quit_now)
        {
            auto start_time = Clock::now();

            // TODO: Remove this, and find alternative
            if (recheck == true)
            {
                memset(chunk_checker, 1, sizeof(chunk_checker));
                recheck = false;
            }

            cellular_automata(pixels, new_version);
            auto end_time = Clock::now();

            // If the frame took less than what ever 1/60th of a second is, then wait for that long
            // Or really just sleep for that long
            if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 33333333)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count())));
            }
        }

        for (int i = 0; i < differences.size(); i++)
        {
            std::cout << differences[i] << ',';
        }
    }
}