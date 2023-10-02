#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <chrono>
#include <mutex>
#include <thread>
#include <atomic>
#include <string>
#include <memory>
#include <bitset>
#include <queue>
#include <unordered_map>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <array>
#include <boost/algorithm/clamp.hpp>
#include <algorithm>
#include <iomanip>
#include "../include/glad/glad.h"
#include <GL/gl.h>
#include <GL/glu.h>
// #include <GL/glu_mangle.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>



#define STB_TRUETYPE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define BLEND_TEXT 0
// #define INPUT_DEBUG

static float ortho_near = -1000.0f;
static float ortho_farr =  3000.f;

typedef std::chrono::high_resolution_clock Clock;


// TODO: change the input system to properly check this stuff
std::atomic<bool> recheck;

// time
unsigned int current_time = (unsigned int)time(NULL);
// something needs to bedone about this
// Use density, and type (static or dynamic)
// entity is rlly just fixed_pos with extra steps

// This has to be declared in advanced for some randoms stuff
/**
 * @brief it's colour type
 * 
 */
struct colour_t
{
    uint8_t r, g, b, a;

    void reset()
    {
        r = g = b = a = 0;
    }
};

struct material_t
{
    // How big will the explosion be, how far it goes
    // and temp changes
    uint8_t explosivity = 0;
    // At what temp/pressure/whatever does it start to become unstable
    uint8_t volatility = 0;
    // Flow is 0 for solid stuff?
    uint8_t flow = 0;

    // Stickiness
    uint8_t stickiness = 0;


    // I wonder, if i call this, does it get 
    // all the global variables
    // void (*update)(cell_t, int, int);
    
    /**
     * Malina just dm'd me on insta,
     * I got a feeling somethings up
     * how badly will I be ridculed this time
     * actually, Im not even sure at this
     * point, it's always so confusing
     */
};


// omg this is such a confusing function
class cell_t
{
public:
// I wonder if i can throw this into one scope-
    void reset_state()
    {
        pressure = 0;
        density = 0;
        temperature = 0;
    }

    /**
     * @brief First val is current cell_t, second is the old version of the one presure ismoving to, third is the new vresion where the pressure is moving to NOTE: CHANGE CAP
     *
     * @param orignal_ptr
     * @param succesor
     * @param predecessor
     * @param change
     * TODO: Remove the if statement in this function
     */
    void modify_cross_die_pressure(cell_t *origin_pressure, cell_t *desination_soon, cell_t *to_be_added, int16_t change)
    {
        if (origin_pressure->pressure - change > 0 && desination_soon->pressure + to_be_added->pressure + change < 500)
        {
            origin_pressure->pressure -= 1;
            // desination_soon->pressure += 1;
            to_be_added->pressure += 1;
        }
    }

    /**
     * @brief changes the value of one cell_t, just add
     *
     * @param orignal_ptr
     * @param change
     */
    void modify_pressure(cell_t *orignal_ptr, int16_t change)
    {
        // cell_t* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).pressure - change) > 0)
        {
            (*orignal_ptr).pressure -= change;
        }
    }

    /**
     * @brief changes the value of one cell_t, just add
     *
     * @param orignal_ptr
     * @param change
     */
    void modify_density(cell_t *orignal_ptr, int16_t change)
    {
        // cell_t* original = orignal_ptr;
        // (*orignal_ptr).pressure;
        if (((*orignal_ptr).density - change) > 0)
        {
            (*orignal_ptr).density -= change;
        }
    }

    /*
    I wonder if Im driving away my friends, I hope im not
    */

    /*
    I wonder if I'll ever find someone I can take my mask off infront of
    I bet it'll never happen
    */

    /**
     * @brief change pressure
     *
     * @param change
     */
    int16_t pressure_change(int16_t change)
    {
        pressure += change;

        return pressure;
    }

    /**
     * @brief change density
     *
     * @param change
     */
    int16_t density_change(uint16_t change)
    {
        density += change;

        return density;
    }

    /**
     * @brief change temperature
     *
     * @param new_temp
     */
    float temperature_change(float new_temp)
    {
        temperature = new_temp;

        return temperature;
    }

    /**
     * @brief
     *
     * @return int16_t
     */
    int16_t fetch_pressure()
    {
        return pressure;
    }

    /**
     * @brief Set the pressure object
     *
     * @param new_val
     */
    void set_pressure(uint16_t new_val)
    {
        pressure = new_val;
    }

    /*
     * Yk, I've been thinking abt these comments whilst Im yeeting all of these old rgba things
     * I dont think they'd notice me gon
     * SO they better not fret when Im gone
     * Cayse they treate me like Im nothing ngl
     * They forget and ignore me
     * Then they claim they just forgot, even when I do ask
     * Which begs the question, will they fret when my dissapearence starts to affect them?
     * Oh well, at least there's not a heart theif at large.
     * 
     * Well I can now confirm they dont notice me when Im gone
     */

    /**
     * @brief
     *
     * @return float
     */
    float fetch_temp()
    {
        return temperature;
    }

    /**
     * @brief It just SETS IT, does not add or subtract
     *
     * @param new_temp
     */
    void modify_temp(float new_temp)
    {
        temperature = new_temp;
    }

    /**
     * @brief
     *
     * @return uint8_t
     */
    uint8_t fetch_flow()
    {
        return flow;
    }

    /**
     * @brief Set the flow object, JUST SETS NO ADD NO SUBTRACT
     *
     * @param new_val
     */
    void modify_flow(uint8_t new_val)
    {
        flow = new_val;
    }

    /**
     * @brief
     *
     * @return int16_t
     */
    int16_t fetch_density()
    {
        return density;
    }

    /**
     * @brief SETS DENSITY
     *
     * @param new_val
     */
    void modify_density(uint16_t new_val)
    {
        density = new_val;
    }

    // Function to return whether it can be interacted with or not
    // I dunno why Im making this a function lmao
    bool is_inert()
    {
        return inert;
    }
    void set_inert(bool new_val)
    {
        inert = new_val;
    }

    colour_t* fetch_col()
    {
        return (&col);
    }

    void set_col(colour_t temp)
    {
        col = temp;
    }

    // Im gonna keep this here in a comment for nostaglic purposes
    // bool get_empty()
    // {
    //     return false;
    // }

private:
    uint16_t pressure = 0;
    uint16_t density = 0;
    uint16_t stickiness = 0;
    uint16_t flow = 0;
    uint16_t temperature = 0;
    bool inert = false;
    struct colour_t col;
    // struct material_t chem_props;

    /**
     * Rewrite the material stuff
     * So, now every material/cell will store a hash of the material which is occupied by them
     * When being rendered, this hash will be used to retrieve data about this one material
     * Continued in render.hpp?
     */
    /**
     * I think I'll just hangout with them, no need to try and like, force myself to be friends with them
     */
    /**
     * Future me here, it hurt more to hangout with them
     * then it did to just be alone
     */

protected:
};

struct cord_2d
{
    uint16_t x_pos;
    uint16_t y_pos;
};

std::string getFileContents(const char *filename)
{
    std::ifstream in(filename, std::ios::binary);
    if(in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return (contents);
    }

    throw(errno);
}
