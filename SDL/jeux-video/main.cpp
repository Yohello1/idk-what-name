#include "start/base_data.hpp"
#include "ui/stb_truetype.h"
#include "math/vector_math.hpp"
#include "physics/physics.hpp"
#include "ui/render.hpp"
#include "input/input_large.hpp"
#include "generation/terain.hpp"
#include "ui/ui.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
// A crap ton of arrays to deal with everything

// Simulated array everything is being fed
struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
struct position usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;

int main()
{
    std::cout << "Starting" << '\n';
    init::start();
    std::cout << "Started, allocating memory" << '\n';

    init::array_clean_start(pixels);
    const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;
    std::cout << "Thingy started now" << '\n';
    // Terrain::Development Terrain_gen;
    // Terrain_gen.Development::generate_terrain(0, LOGICAL_WINDOW_WIDTH / 2, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH, 8, pixels);

    cord_2d box_verts[2];
    box_verts[0].x_pos = 0;
    box_verts[1].x_pos = 100;
    box_verts[0].y_pos = 0;
    box_verts[1].y_pos = 25;

    cord_2d text_position;
    text_position.x_pos = 0;
    text_position.y_pos = 5;
    std::cout << "Txt pos " << text_position.x_pos << " " << text_position.y_pos << '\n';

    colour colour_to_change;
    colour_to_change.r = 231;
    colour_to_change.g = 63;
    colour_to_change.b = 0;
    colour_to_change.a = 255;

    colour colour_to_change2;
    colour_to_change2.r = 46;
    colour_to_change2.g = 253;
    colour_to_change2.b = 124;
    colour_to_change2.a = 255;

    // All just for entities testing lol
    Conductor.Init();
    entites::Signature box_signature;
    Conductor.RegisterComponent<entites::sqaure_box>();
    Conductor.RegisterComponent<entites::direction>();
    Conductor.RegisterComponent<entites::rgba_colour>();
    auto movingBoxes = Conductor.RegisterSystem<Moving_Day>();
    box_signature.set(Conductor.GetComponentType<entites::sqaure_box>());
    std::vector<entites::Entity> entities(0);
    // Making all the entitites
    for (auto &entity : entities)
    {
        entity = Conductor.CreateEntity();

        Conductor.AddComponent(
            entity,
            entites::sqaure_box{});

        Conductor.AddComponent(
            entity,
            entites::direction{});

        Conductor.AddComponent(
            entity,
            entites::rgba_colour{});
    }
    movingBoxes->Init();

    ui::init_font_all("ui/font/Hack-Regular.ttf");

    ui_elements.push_back(new ui::boxes(box_verts[0], box_verts[1], colour_to_change));
    // It needs to have a space at the start, idk why tbh
    // OK OK it's cause it's looping through the thing???
    ui_elements.push_back(new ui::text(text_position, colour_to_change2, "Ghost", 30, 0));

    std::cout << "Done ui" << '\n';

    std::cout << "Real addy " << &new_version << ' ' << &pixels << '\n';

    // bool quit = false;
    std::thread physics(Physics::simulate, pixels, new_version);
    // int frame_count = 0;

    while (!quit_now)
    {
        // std::cout << "Real addy " << &new_version  << ' ' << &pixels << '\n';

        mtx2.lock();
        quit_now = Input_Large::poll_usr_input(changed, usr_input, &event, quit_now, actual_2_logic_ratio);
        Input_Large::mix_new_version_usr_input(changed, usr_input, pixels);
        std::memcpy(&render, &pixels, sizeof(pixels));
        mtx2.unlock();

        // std::cout << "Start drawing words" << '\n';
        ui_elements[0]->draw(render);
        ui_elements[1]->draw(render);
        // std::cout << "Done drawing words, drawing sqaures" << '\n';
        movingBoxes->Update(render);
        Debug::chunk_update_status(render, Physics::chunk_checker);

        Render::redraw_render(render, renderer);
        SDL_RenderPresent(renderer);

        // just want to test this out, remove it later
        // std::cout << "Status" << Physics::chunk_checker[1][0];
    }
    physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();
}

void excecution_finished(void)
{

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
