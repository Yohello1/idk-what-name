#include <cstdlib>
#include <cstring>
#include <SDL2/SDL.h>
#include <ctime>
#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <thread>
#include <chrono>
#define LOGICAL_WINDOW_WIDTH 256
#define ACTUAL_WINDOW_WIDTH 1024
const int noise_density = 80;
const int wind_dir_degrees = 80;
const int actual_2_logic_ratio = ACTUAL_WINDOW_WIDTH / LOGICAL_WINDOW_WIDTH;
//time
unsigned int current_time = (unsigned int)time(NULL);
// pointers to these things
SDL_Renderer *renderer;
SDL_Window *window;
SDL_Event event;
enum pixel_state
{
   empty,
   gas,
   fluid,
   solid,
   fire,
   burning,
   fixed_pos,
   burnt,
   eternal_fire,
   flameable,
};
struct position
{
   pixel_state state_now;
   uint8_t r, g, b, a;
   uint16_t temperature;
   double pressure;
};
// why must I make this
struct cord_2d
{
   int x_pos;
   int y_pos;
};

struct position pixels[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position new_version[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];
struct position smoke[LOGICAL_WINDOW_WIDTH][LOGICAL_WINDOW_WIDTH];

void excecution_finished();
void smoke_move();
int noise_gen();
void scr_dump();
int pressure_total_dump();

// TODO update comments
// So this is gonna be how smoke & fire works
// Ok so fire make smoke, which goes to another layer?
// render normal
// Then render smoke
// Then present
// Remember to use renderclear for SDL

// no clue what this does, I just use it for timing lol
typedef std::chrono::high_resolution_clock Clock;

int main()
{
   std::cout << "MY BAGUETTES ARE ON FIRE" << std::endl;
   // telling it how to scale
   SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
   // Printing the time
   std::cout << "Time = " << current_time << "\n";
   // setting random seed
   srand(current_time);
   // rise my glorious creation*
   SDL_Init(SDL_INIT_VIDEO);
   SDL_CreateWindowAndRenderer(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_WIDTH, 0, &window, &renderer);
   SDL_RenderSetLogicalSize(renderer, LOGICAL_WINDOW_WIDTH, LOGICAL_WINDOW_WIDTH);
   SDL_SetWindowTitle(window, "Averager");
   SDL_RenderClear(renderer);
   // PAINT IT BLACK
   int start_pressure = 0;

   SDL_Delay(1000);

   // Default everything to 0
   for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
   {
      for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
      {
         pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = 0;
         pixels[x_pos][y_pos].state_now = empty;
         pixels[x_pos][y_pos].temperature = 0;
         pixels[x_pos][y_pos].pressure = 0;
      }
   }
   // scr_dump();

   // for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
   // {
   //    for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
   //    {
   //       if (noise_gen() == 1)
   //       {
   //          pixels[x_pos][y_pos].pressure = 255;
   //          pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = 255;
   //          start_pressure += 255;
   //          // SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, 255);
   //          // SDL_RenderDrawPoint(renderer, x_pos, y_pos);
   //          // SDL_RenderPresent(renderer);
   //       }
   //    }
   // }

   std::thread pressure_pointer(smoke_move);
   // scr_dump();

   // The magic happens here
   // I have no idea how any of this works lol

   int mouse_x = 0;
   int mouse_y = 0;

   while (1)
   {
      if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      {
         break;
      }
      // auto start_time = Clock::now();
      // eqaulize();

      // SDL_RenderClear(renderer);
      for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
      {
         for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
         {
            pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].pressure;
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
         }
      }
      SDL_RenderPresent(renderer);

      switch (event.button.button)
      {
      case SDL_BUTTON_LEFT:
         SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
         SDL_GetMouseState(&mouse_x, &mouse_y);
         std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
         std::cout << "It somehow worked" << std::endl;
         SDL_RenderDrawPoint(renderer, mouse_x / actual_2_logic_ratio, mouse_y / actual_2_logic_ratio);
         SDL_RenderPresent(renderer);
         pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].r = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].g = pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].a = 255;
         pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].b = 0;
         pixels[mouse_x / actual_2_logic_ratio][mouse_y / actual_2_logic_ratio].state_now = fixed_pos;
         SDL_RenderPresent(renderer);
         break;
      case SDL_BUTTON_RIGHT:
         // set draw colour
         SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
         // Get mouse position
         SDL_GetMouseState(&mouse_x, &mouse_y);
         //Output location
         std::cout << "(" << mouse_x / actual_2_logic_ratio << "," << mouse_y / actual_2_logic_ratio << ")" << std::endl;
         // Get mouse position, convert to logical position, then make like a block around it which is 8x8 to make I think white
         for (int y_pos = (mouse_y / actual_2_logic_ratio) - 4; y_pos != LOGICAL_WINDOW_WIDTH - 1 && y_pos < (mouse_y / actual_2_logic_ratio) + 4; y_pos++)
         {
            for (int x_pos = (mouse_x / actual_2_logic_ratio) - 4; x_pos != LOGICAL_WINDOW_WIDTH - 1 && x_pos < (mouse_x / actual_2_logic_ratio) + 4; x_pos++)
            {
               // Drawing , outputing position, draw, and seting new state
               SDL_RenderDrawPoint(renderer, x_pos, y_pos);
               std::cout << "(" << x_pos << "," << y_pos << ")" << std::endl;
               pixels[x_pos][y_pos].state_now = solid;
               pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].b = 255;
            }
            SDL_RenderPresent(renderer);
         }

         break;
      }

      // std::cout << "Next Itteration" << std::endl;

      // 1/60 - time to run minus
      // auto end_time = Clock::now();
      // if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 16000000)
      // {
      //    SDL_Delay((16000000 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 100000);
      // }
   }

   pressure_pointer.join();
   int end_pressure = pressure_total_dump();
   std::cout << "Start  pressure: " << start_pressure << "\nEnd    pressure: " << end_pressure << std::endl
             << "Total difference " << end_pressure - start_pressure << std::endl;
   // scr_dump();
   excecution_finished();
}

void smoke_move()
{

   // What the heck is happening here???
   /*
   So, get average of surrounding cells this becomes target
   if cell is below the target, find cell with above target amount, and subtract one from it and add to cell

   If above target, subtract one from cell and add to cell below the average
   */

   // while (1)
   // {

   //    if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
   //    {
   //       break;
   //    }
   //    auto start_time = Clock::now();
   //    for (int x_pos = 1; x_pos < LOGICAL_WINDOW_WIDTH - 1; x_pos++)
   //    {
   //       for (int y_pos = 1; y_pos < LOGICAL_WINDOW_WIDTH - 1; y_pos++)
   //       {
   //          double target_pressure;
   //          // Fetch average

   //          /*
   //                0 1 0
   //                2 X 3
   //                0 4 0
   //             */
   //          target_pressure = (pixels[x_pos][y_pos - 1].pressure + pixels[x_pos - 1][y_pos].pressure + pixels[x_pos + 1][y_pos].pressure + pixels[x_pos][y_pos + 1].pressure) / 4;
   //          // std::cout << "Target Pressure" << target_pressure << std::endl;

   //          // Below target
   //          if (pixels[x_pos][y_pos].pressure < target_pressure)
   //          {
   //             /*
   //                5 1 6
   //                2 X 3
   //                7 4 8
   //             */
   //             if (pixels[x_pos][y_pos - 1].pressure > target_pressure)
   //             {
   //                pixels[x_pos][y_pos].pressure++;
   //                pixels[x_pos][y_pos - 1].pressure--;
   //                // std::cout << "Change be made1" << std::endl;
   //             }

   //             else if (pixels[x_pos - 1][y_pos].pressure > target_pressure)
   //             {
   //                pixels[x_pos][y_pos].pressure++;
   //                pixels[x_pos - 1][y_pos].pressure--;
   //                // std::cout << "Change be made" << std::endl;
   //             }
   //             else if (pixels[x_pos + 1][y_pos].pressure > target_pressure)
   //             {
   //                pixels[x_pos][y_pos].pressure++;
   //                pixels[x_pos + 1][y_pos].pressure--;
   //                // std::cout << "Change be made" << std::endl;
   //             }
   //             else if (pixels[x_pos][y_pos + 1].pressure > target_pressure)
   //             {
   //                pixels[x_pos][y_pos].pressure++;
   //                pixels[x_pos][y_pos + 1].pressure--;
   //                //std::cout << "Change be made" << std::endl;
   //             }
   //          }

   //          if (pixels[x_pos][y_pos].state_now == burning)
   //          {
   //             // std::cout << "There is a toasty baguette here" << std::endl;
   //             /*
   //                             . 3 .
   //                             2 X 4
   //                             . 1 .
   //                            */
   //             // Just go look at the fire_wind_math_idk.png picture for an explaination
   //             if (y_pos != 255 && pixels[x_pos][y_pos + 1].state_now == flameable && pixels[x_pos][y_pos + 1].temperature != 510)
   //             {
   //                // I HAVE NO CLUE BUT I'LL TRY AND EXPLAIN
   //                // this is like taking the dir and changing how fast it's heating up the tiles based on the intensity in its direction
   //                // "wind_dir_degrees * 3.141 / 180" switches it to radians, then pass it through sin/cos
   //                // sin for height, cos for horizonetelasjs
   //                pixels[x_pos][y_pos + 1].temperature += sin(wind_dir_degrees * 3.141 / 180);
   //                // sets colour stuff by taking temp and halfint it, might remove this tbh
   //                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].temperature / 2;
   //                // Setting colour and drawing
   //                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos + 1].r, pixels[x_pos][y_pos + 1].g, pixels[x_pos][y_pos + 1].b, pixels[x_pos][y_pos + 1].a);
   //                SDL_RenderDrawPoint(renderer, x_pos, y_pos + 1);
   //             }
   //             if (x_pos != 1 && pixels[x_pos - 1][y_pos].state_now == flameable && pixels[x_pos - 1][y_pos].temperature != 510)
   //             {
   //                pixels[x_pos - 1][y_pos].temperature += cos(wind_dir_degrees * 3.141 / 180);
   //                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].temperature / 2;
   //                SDL_SetRenderDrawColor(renderer, pixels[x_pos - 1][y_pos].r, pixels[x_pos - 1][y_pos].g, pixels[x_pos - 1][y_pos].b, pixels[x_pos - 1][y_pos].a);
   //                SDL_RenderDrawPoint(renderer, x_pos - 1, y_pos);
   //             }
   //             if (y_pos != 1 && pixels[x_pos][y_pos - 1].state_now == flameable && pixels[x_pos][y_pos - 1].temperature != 510)
   //             {
   //                // The +1 stuff is to offset the negative stuff so it doesnt become negative temp
   //                pixels[x_pos][y_pos - 1].temperature += (sin(wind_dir_degrees * 3.141 / 180) + 1);
   //                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].temperature / 2;
   //                SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos - 1].r, pixels[x_pos][y_pos - 1].g, pixels[x_pos][y_pos - 1].b, pixels[x_pos][y_pos - 1].a);
   //                SDL_RenderDrawPoint(renderer, x_pos, y_pos - 1);
   //             }
   //             if (x_pos != 255 && pixels[x_pos + 1][y_pos].state_now == flameable && pixels[x_pos + 1][y_pos].temperature != 510)
   //             {
   //                pixels[x_pos + 1][y_pos].temperature += (cos(wind_dir_degrees * 3.141 / 180) + 1);
   //                pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].temperature / 2;
   //                SDL_SetRenderDrawColor(renderer, pixels[x_pos + 1][y_pos].r, pixels[x_pos + 1][y_pos].g, pixels[x_pos + 1][y_pos].b, pixels[x_pos + 1][y_pos].a);
   //                SDL_RenderDrawPoint(renderer, x_pos + 1, y_pos);
   //             }
   //             pixels[x_pos][y_pos].temperature += 1;
   //          }
   //          if (pixels[x_pos][y_pos].temperature == 50)
   //          {
   //             pixels[x_pos][y_pos].state_now = burning;
   //          }
   //          if (pixels[x_pos][y_pos].temperature > 510)
   //          {
   //             pixels[x_pos][y_pos].state_now = burnt;
   //             SDL_SetRenderDrawColor(renderer, 178, 190, 181, 255);
   //             SDL_RenderDrawPoint(renderer, x_pos, y_pos);
   //          }
   //       }
   //    }
   //    auto end_time = Clock::now();
   //    if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 16000000)
   //    {
   //       SDL_Delay((16000000 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 100000);
   //       std::cout << "sleep god dam it" << std::endl;
   //    }
   // }
}

void excecution_finished(void)
{

   SDL_DestroyRenderer(renderer);
   SDL_DestroyWindow(window);
   SDL_Quit();
}

void scr_dump()
{
   for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
   {
      for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
      {
         std::cout << "(" << pixels[x_pos][y_pos].pressure << ")";
      }
      std::cout << std::endl;
   }
}

int pressure_total_dump()
{
   int end_pressure = 0;
   for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
   {
      for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
      {
         end_pressure += pixels[x_pos][y_pos].pressure;
      }
      std::cout << std::endl;
   }

   return end_pressure;
}

int noise_gen()
{
   int num = (rand() % 100);

   if (num > noise_density)
   {
      num = 1;
   }
   else
   {
      num = 0;
   }
   return num;
}
