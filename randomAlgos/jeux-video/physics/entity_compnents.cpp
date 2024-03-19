struct location_bounds
{
    u_int16_t x_pos;
    u_int16_t y_pos;
};

struct health
{
    u_int16_t health;
    u_int16_t max_health;
};

struct mana
{
    u_int16_t mana;
    u_int16_t max_mana;
};

struct cord_2d_float
{
    double x_pos;
    double y_pos;

};

struct sqaure_box
{
    cord_2d_float point;
    uint8_t size;
};

// https://www.mathsisfun.com/geometry/unit-circle.html
// Ur gonna forget how this works in two hours time
// 0 = right direction
// Actually this is just a 0-2^16 degrees
// Makesure to do a modulo 360 afta every operation
struct direction
{
    int16_t degrees;
};



struct basic_data
{
    char name[31]; // I want to keep this under 256 bytes
    // uint16_t time_of_creation; // No clue if this is worth having
    uint8_t type_of_entity;
    // Whether it needs to be "calculated" or not
    // TODO switch this into chunks
    bool live;
};

struct rgba_colour
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
    uint8_t alpha;

};
