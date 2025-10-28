// #include <SDL.h>
#include <iostream>
#include <cmath>
#include <cstdint>
#include <iomanip>
#include <raylib.h>


struct ball
{
    double x_p;
    double y_p;
    double y_v;
    double radius;
    double mass;
};

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int TARGET_FPS = 120;
// Scale factor to convert meters (physics units) to pixels (screen units)
const double METERS_TO_PIXELS = 5.0; 
// Offset for the ground line (in pixels from the bottom)
const int GROUND_OFFSET = 50; 
// Fixed time step for physics updates (Runge-Kutta or Euler)
const double PHYSICS_TIMESTEP = 0.005;


int main()
{

    std::cout << "hello world" << std::endl;
    
    double t_delta;

    uint32_t itterations;

    std::cout << "How many itterations?" << std::endl;
    std::cin >> itterations;

    std::cout << "T-delta?" << std::endl;
    std::cin >> t_delta;

InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib Bouncing Ball with Drag");
    SetTargetFPS(TARGET_FPS);
    double gravity = -2.5;

    struct ball myBall;
    myBall.x_p = 100.0;
    myBall.y_p = 100.0;
    myBall.y_v = 0.0;
    myBall.mass = 5.0;
    myBall.radius = 1.0;

    for(int i = 0; i < itterations; i++)
    {
        // Hit floor
        if(myBall.y_p <= 0)
        {
            myBall.y_v *= -0.95;
            myBall.y_p = 0.1;

        }

        // Update velocity according to gravity
        // myBall.y_v += t_delta*gravity;

        // Update according to air resistance
        double drag = -0.1*sgn(myBall.y_v)*.225*0.47*myBall.radius*myBall.radius*3.14*std::pow(std::abs(myBall.y_v), 2);
        double gravity_force = gravity*myBall.mass;    

        double f_net = drag + gravity_force;

        myBall.y_v += (f_net/myBall.mass)*t_delta;

        myBall.y_p += myBall.y_v*t_delta;

        std::cout << std::setw(5) << myBall.y_p  << ' '  << myBall.y_v << '\n';


BeginDrawing();

            ClearBackground(BLACK);

            // Draw Ground (Green line)
            Color ground_color = GREEN;
            int ground_y_pixel = SCREEN_HEIGHT - GROUND_OFFSET;
            DrawLine(0, ground_y_pixel, SCREEN_WIDTH, ground_y_pixel, ground_color);
            
            // Draw Ball (White circle)
            
            // Convert physics coordinates (meters) to screen coordinates (pixels)
            int ball_x_pixel = static_cast<int>(myBall.x_p * METERS_TO_PIXELS);
            
            // Y pixel is inverted (Raylib Y=0 is top) and offset for the ground
            int ball_y_pixel = SCREEN_HEIGHT - static_cast<int>(myBall.y_p * METERS_TO_PIXELS) - GROUND_OFFSET;
            
            int ball_radius_pixel = static_cast<int>(myBall.radius * METERS_TO_PIXELS);

            DrawCircle(ball_x_pixel, ball_y_pixel, (float)ball_radius_pixel, WHITE);
            
            // Draw FPS for debugging
            DrawFPS(10, 10);
            
        EndDrawing();
    }



}
