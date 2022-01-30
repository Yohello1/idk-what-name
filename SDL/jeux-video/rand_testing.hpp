namespace Testing
{
    int count_sand(position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
    {
        int i = 0;
        for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
        {
            for (int y_pos = LOGICAL_WINDOW_WIDTH; y_pos > 0; y_pos--)
            {
                if (pixels[x_pos][y_pos].state_now == solid)
                {
                    i++;
                }
            }
        }
        return i;
    }
}