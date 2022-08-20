/*
By Yohwllooooo

Created: 2022-08-20
Last modified: 2022-08-20

Modifications: 
Creating the file, and adding all but simulation function
Creating comments to explain search/simulation thing
*/

namespace
{
    // Remember what chunks need to checked, and which ones can be skipped
    bool chunk_checker[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8];
    // A mutex lmfao
    std::mutex mtx;

    // Clears the data of a pixel
    void clear_data(position the_pixel)
    {
        memset(the_pixel, 0, sizeof(the_pixel));
    }

    void chunk_copier(position pixels[LOGICAL_WINDOW_WINDOW][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WINDOW][LOGICAL_WINDOW_WIDTH], u_int16_t x_chunk, u_int16_t y_chunk)
    {

        for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
        {
            memcpy(new_version[x_chunk * 8][y_pos], pixels[x_chunk * 8][y_pos], sizeof(new_version[0][0] * 8));
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

    void simulate(positions pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {
        // Decleration of the time variables
        auto start_time, end_time;
        while (!quit_now)
        {
            start_time = Clock::now();

            // TODO: Remove this, and find alternative
            if (recheck == true)
            {
                memset(chunk_checker, 1, sizeof(chunk_checker));
                recheck = false;
            }

            basic_cellular_automata(pixels, new_version);
            end_time = Clock::now();
            
            // If the frame took less than what ever 1/60th of a second is, then wait for that long
            // Or really just sleep for that long 
            if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 33333333)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds((33333333 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count())));
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

}