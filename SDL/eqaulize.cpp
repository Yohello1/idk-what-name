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
   burnt,
   fixed_pos
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
void excecution_finished();
void eqaulize();
int noise_gen();
void scr_dump();
int pressure_total_dump();

// this script is gonn hurtme

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

   SDL_Delay(5000);

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

   for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
   {
      for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
      {
         if (noise_gen() == 1)
         {
            pixels[x_pos][y_pos].pressure = 255;
            pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = 255;
            start_pressure += 225;
            // SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, 255);
            // SDL_RenderDrawPoint(renderer, x_pos, y_pos);
            // SDL_RenderPresent(renderer);
         }
      }
   }

   std::thread pressure_pointer(eqaulize);
   // scr_dump();

   while (1)
   {
      if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      {
         break;
      }
      auto start_time = Clock::now();
      // eqaulize();
      for (int x_pos = 0; x_pos < LOGICAL_WINDOW_WIDTH; x_pos++)
      {
         for (int y_pos = 0; y_pos < LOGICAL_WINDOW_WIDTH; y_pos++)
         {
            pixels[x_pos][y_pos].a = pixels[x_pos][y_pos].g = pixels[x_pos][y_pos].b = pixels[x_pos][y_pos].r = pixels[x_pos][y_pos].pressure;
            SDL_SetRenderDrawColor(renderer, pixels[x_pos][y_pos].r, pixels[x_pos][y_pos].g, pixels[x_pos][y_pos].b, pixels[x_pos][y_pos].a);
            SDL_RenderDrawPoint(renderer, x_pos, y_pos);
         }
      }

      std::cout << "Next Itteration" << std::endl;
      SDL_RenderPresent(renderer);

      // 1/60 - time to run minus
      auto end_time = Clock::now();
      if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 16000000)
      {
         SDL_Delay((16000000 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 1000000000);
      }
   }
   int end_pressure = pressure_total_dump();
   std::cout << "Start pressure: " << start_pressure << "\nEnd pressure: " << end_pressure << std::endl
             << "Total difference " << end_pressure - start_pressure << std::endl;
   pressure_pointer.join();
   // scr_dump();
   excecution_finished();
}

void eqaulize()
{

   // What the heck is happening here???
   /*
   So, get average of surrounding cells this becomes target
   if cell is below the target, find cell with above target amount, and subtract one from it and add to cell

   If above target, subtract one from cell and add to cell below the average
   */

   while (1)
   {

      if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
      {
         break;
      }
      auto start_time = Clock::now();
      for (int x_pos = 1; x_pos < LOGICAL_WINDOW_WIDTH - 1; x_pos++)
      {
         for (int y_pos = 1; y_pos < LOGICAL_WINDOW_WIDTH - 1; y_pos++)
         {
            double target_pressure;
            // Fetch average

            {
               /*
                  0 1 0
                  2 X 3
                  0 4 0
               */
               target_pressure = (pixels[x_pos][y_pos - 1].pressure + pixels[x_pos - 1][y_pos].pressure + pixels[x_pos + 1][y_pos].pressure + pixels[x_pos][y_pos + 1].pressure) / 4;
               // std::cout << "Target Pressure" << target_pressure << std::endl;
            }

            // Below target
            if (pixels[x_pos][y_pos].pressure < target_pressure)
            {
               /*
                  5 1 6
                  2 X 3
                  7 4 8
               */
               if (pixels[x_pos][y_pos - 1].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos][y_pos - 1].pressure--;
                  //  std::cout << "Change be made" << std::endl;
               }

               else if (pixels[x_pos - 1][y_pos].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos - 1][y_pos].pressure--;
                  // std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos + 1][y_pos].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos + 1][y_pos].pressure--;
                  // std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos][y_pos + 1].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos][y_pos + 1].pressure--;
                  //std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos - 1][y_pos - 11].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos - 1][y_pos - 1].pressure--;
                  //std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos + 1][y_pos - 1].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos + 1][y_pos - 1].pressure--;
                  // std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos - 1][y_pos + 1].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos - 1][y_pos + 1].pressure--;
                  //std::cout << "Change be made" << std::endl;
               }
               else if (pixels[x_pos + 1][y_pos + 1].pressure > target_pressure)
               {
                  pixels[x_pos][y_pos].pressure++;
                  pixels[x_pos + 1][y_pos + 1].pressure--;
                  //std::cout << "Change be made" << std::endl;
               }
            }
         }
      }
      auto end_time = Clock::now();
      if (std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count() < 16000000)
      {
         SDL_Delay((16000000 - std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count()) / 1000000000);
      }
   }
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
