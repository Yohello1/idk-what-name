extern entites::Coordinator Conductor;

class Moving_Day : public entites::System
{
public:
    void Init();
    void Update(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH]);
};

void Moving_Day::Init()
{
    // God knows how to use this
    for (auto const &entity : mEntities)
    {
        // Fetching all the components
        auto &location = Conductor.GetComponent<entites::sqaure_box>(entity);
        auto &rotation = Conductor.GetComponent<entites::direction>(entity);
        auto &colour = Conductor.GetComponent<entites::rgba_colour>(entity);

        // Where they gon spawn
        // int new_x = 128;
        // int new_y = 128;

        std::random_device dev;
        std::mt19937 rng(dev());

        std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 360); // distribution in range [1, 6]

        // Set location
        location.point.x_pos = 64;
        location.point.y_pos = 64;

        // std::uniform_int_distribution<std::mt19937::result_type> dist6(0, 360); // distribution in range [1, 6]

        rotation.degrees = (uint)(dist6(rng));

        // Random colours
        colour.red   = (rand() % 255);
        colour.green = (rand() % 255);
        colour.blue  = (rand() % 255);
        colour.alpha = (rand() % 255);
    }
}

void Moving_Day::Update(cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH])
{
    for (auto const &entity : mEntities)
    {
        auto &location = Conductor.GetComponent<entites::sqaure_box>(entity);
        auto &rotation = Conductor.GetComponent<entites::direction>(entity);
        auto &colour = Conductor.GetComponent<entites::rgba_colour>(entity);

        for (int x_pos = location.point.x_pos - 5; x_pos < location.point.x_pos + 5; x_pos++)
        {
            for (int y_pos = location.point.y_pos - 5; y_pos < location.point.y_pos + 5; y_pos++)
            {

                pixels[x_pos][y_pos].r = colour.red;
                pixels[x_pos][y_pos].g = colour.green;
                pixels[x_pos][y_pos].b = colour.blue;
                pixels[x_pos][y_pos].a = colour.alpha;
            }
        }

        if (location.point.y_pos < 5 ||
            location.point.y_pos > (LOGICAL_WINDOW_WIDTH - 6))
        {
            rotation.degrees *= -1;
            rotation.degrees %= 360;
        }
        else if (location.point.x_pos < 5 ||
                 location.point.x_pos > (LOGICAL_WINDOW_WIDTH - 6))
        {

            // I assume nothing will ever be 0
            if (rotation.degrees == 180)
            {
                rotation.degrees = 0;
            }
            else if (rotation.degrees < 180)
            {
                rotation.degrees -= 180;
                rotation.degrees *= -1;
            }
            else if (180 < rotation.degrees)
            {
                rotation.degrees -= 180;
                rotation.degrees *= -1;
                rotation.degrees += 360;
            }

            rotation.degrees %= 360;
        }
        else if (location.point.y_pos < 3 ||
                 location.point.y_pos > (LOGICAL_WINDOW_WIDTH - 3))
        {
            location.point.x_pos = 64;
            location.point.y_pos = 64;
        }

        else if (location.point.x_pos < 3 ||
                 location.point.x_pos > (LOGICAL_WINDOW_WIDTH - 3))
        {
            location.point.x_pos = 64;
            location.point.y_pos = 64;
        }
        float y_shift = sin(rotation.degrees * 3.14 / 180);
        float x_shift = cos(rotation.degrees * 3.14 / 180);

        location.point.x_pos += x_shift;
        location.point.y_pos += y_shift;
    }
}
