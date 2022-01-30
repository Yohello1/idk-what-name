namespace Debug_Printing
{
    void print_density(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {

        for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
        {
            for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
            {
                std::cout << "(" << unsigned(pixels[y_pos][x_pos].density) << ")";
            }
            std::cout << '\n';
        }
    }
}