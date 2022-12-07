#include "start/base_data.hpp"
#include "render/stb_truetype.h"
#include "math/vector_math.hpp"
// #include "physics/physics.hpp"
#include "physics/physics2.hpp"
#include "render/render.hpp"
#include "input/input_large.hpp"
#include "generation/terain.hpp"
#include "render/ui.hpp"
#include "physics/entity.hpp"
#include "physics/entity_types.hpp"
#include "debug/debug.hpp"
// A crap ton of arrays to deal with everything

// Simulated array everything is being fed
cell pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// Array responsble for the UI
struct colour user_interface[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
// USR input and changes, TODO SWITCH TO EQAUTION
cell usr_input[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
bool changed[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
cell render[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
void excecution_finished();
void draw_box(cord_2d start, cord_2d end);

std::vector<ui::single_ui_element *> ui_elements;
unsigned int currenttime = (unsigned int)time(NULL);
std::mutex mtx2;
entites::Coordinator Conductor;
const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;

int main()
{
    std::cout << "Starting" << '\n';

    // Starting stuff
    srand(current_time);

    // FIXME: Switch to ACTUAL_WINDOW_WIDTH once this works
    glfwInit();
    GLFWwindow *w = glfwCreateWindow(600, 600, "idk", NULL, NULL);
    glfwMakeContextCurrent(w);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    float t[] =
        {
            -.5f, -.5f, 0.f,
            0.f, .5f, 0.f,
            .5f, -.5f, 0.f};

    // VAO, vertex Array Object
    uint32_t v;
    glGenBuffers(1, &v);
    glBindBuffer(GL_ARRAY_BUFFER, v);
    glBufferData(GL_ARRAY_BUFFER, sizeof(t) * sizeof(t) / sizeof(t[0]), &t[0], GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);

    // init::start();
    std::cout << "Started, allocating memory" << '\n';

    array_clean_start(pixels);
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
    colour_to_change.r = 254;
    colour_to_change.g = 1;
    colour_to_change.b = 1;
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
    // movingBoxes->Init();

    // ui::init_font_all("ui/font/Hack-Regular.ttf");

    // ui_elements.push_back(new ui::boxes(box_verts[0], box_verts[1], colour_to_change));
    // // It needs to have a space at the start, idk why tbh
    // // OK OK it's cause it's looping through the thing???
    // ui_elements.push_back(new ui::text(text_position, colour_to_change2, "Ghost", 30, 0));
    std::cout << "Done ui" << '\n';
    std::cout << "Real addy " << &new_version << ' ' << &pixels << '\n';

    box_verts[0].x_pos = 80;
    box_verts[1].x_pos = 100;
    box_verts[0].y_pos = 80;
    box_verts[1].y_pos = 100;

    std::thread physics(physics::simulate, pixels, new_version);

    // For now
    // while(!quit_now)
    while (!glfwWindowShouldClose(w))
    {

        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(w);
        glfwPollEvents();

        // mtx2.lock();
        //// quit_now = Input_Large::poll_usr_input(changed, usr_input, &event, quit_now, actual_2_logic_ratio);
        //// Input_Large::mix_new_version_usr_input(changed, usr_input, pixels);
        // std::memcpy(&render, &pixels, sizeof(pixels));
        // mtx2.unlock();

        // ui_elements[0]->draw(render);
        // ui_elements[1]->draw(render);
        // movingBoxes->Update(render);
        //// Debug::chunk_update_status(render, Physics::chunk_checker);

        // Render::redraw_render(render, w);
        //// SDL_RenderPresent(renderer);
    }
    quit_now= true;
    physics.join();
    std::cout << "ENDED" << '\n';
    excecution_finished();

    glfwDestroyWindow(w);
}

void excecution_finished(void)
{

    // SDL_DestroyRenderer(renderer);
    // SDL_DestroyWindow(window);
    // SDL_Quit();
}
