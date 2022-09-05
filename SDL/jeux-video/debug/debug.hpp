namespace Debug
{
    void print_density(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {

        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                std::cout << "(" << unsigned(pixels[y_pos][x_pos].fetch_density()) << ")";
            }
            std::cout << '\n';
        }
    }

    void chunk_update_status(cell ui[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH], bool chunks[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8])
    {
        bool chunk_temp[LOGICAL_WINDOW_WIDTH / 8][LOGICAL_WINDOW_WIDTH / 8];
        std::memcpy(chunk_temp, chunks, sizeof(chunk_temp));

        for (int x_chunk = 0; x_chunk < (LOGICAL_WINDOW_WIDTH / 8); x_chunk++)
        {
            for (int y_chunk = 0; y_chunk < (LOGICAL_WINDOW_WIDTH / 8); y_chunk++)
            {
                if (chunk_temp[x_chunk][y_chunk] == false)
                {
                    for (int y_pos = y_chunk * 8; y_pos > (y_chunk * 8 - 8); y_pos--)
                    {
                        for (int x_pos = x_chunk * 8; x_pos < x_chunk * 8 + 8; x_pos++)
                        {
                            ui[x_pos][y_pos].change_r(50);
                        }
                    }
                }
                // std::cout << chunk_temp[x_chunk][y_chunk] /*<< ','*/;
            }
            // std::cout << '\n';
        }
        // std::cout << '\n';
        // std::cout << '\n';
    }

}
