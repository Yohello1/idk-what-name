#include <sycl/sycl.hpp>
#include <SFML/Graphics.hpp>
#include <cmath>
#include <unistd.h>
#include <chrono>
#include <thread>

#ifndef MAX_VAL
#define MAX_VAL 200
#endif

constexpr int N =9999;

sf::RenderWindow window(sf::VideoMode(1024, 1024), "draw2");

void drawPoints(float ClastFrame[]);

// Pos Vel
// x,0 x,1
// y,2 y,3
// z,4 z,5
//
// mass : m

// Clang doesnt work :(

int main()
{
    std::cout << "Starting Gravity thingy" << std::endl;

    // Init stuff

    srand(10);

    sycl::queue jobQueue{sycl::cpu_selector_v};
    std::cout << "Selected Device: " << jobQueue.get_device().get_info<sycl::info::device::name>() << "\n";
    std::cout << "Selected Vendor: " << jobQueue.get_device().get_info<sycl::info::device::vendor>() << "\n";

    float ClastFrame[N*6];
    float CnextFrame[N*6];
    float Cmass[N];


    std::cout << "HELLO WORLD" << std::endl;

    sycl::buffer<float> lastFrame{ClastFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> nextFrame{CnextFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> mass{Cmass, sycl::range{N}, {sycl::property::buffer::use_host_ptr()}};

    for(int i = 0; i < N; i++)
    {
        ClastFrame[i*6+0] = 10 + (rand()%MAX_VAL);
        ClastFrame[i*6+2] = 10 + (rand()%MAX_VAL);
        ClastFrame[i*6+4] = 10;

        ClastFrame[i*6+1] = 0;
        ClastFrame[i*6+3] = 0;
        ClastFrame[i*6+5] = 0;

        Cmass[i] = 10;
        CnextFrame[i] = 0;
    }

    // Fx = Force X axis
    // Fy = Force Y axis, etc
    // D = Delta


    while(window.isOpen())
    {
        // std::cout << "next: "  << ClastFrame[7] << ' ' << ClastFrame[9] << ' ' << ClastFrame[11] << '\n';
        jobQueue.submit([&] (sycl::handler &h){
            sycl::accessor genData{lastFrame, h, sycl::read_only};
            sycl::accessor nexData{nextFrame, h, sycl::write_only};
            sycl::accessor massA{mass, h, sycl::read_only};

            sycl::stream debugOut(4096, 1024, h);

            h.parallel_for(sycl::range{N}, [=] (sycl::id<1> it){
                int slaveX = it;
                float Fx = 0, Fy = 0, Fz = 0;

                float G = 0.001;

                for(int i = 0; i < N; i++)
                {
                    // F = GMm/r^2
                    // G = 1-
                    // X-axis
                    float Mm  =     (massA[i]*massA[slaveX]);
                    float R2x = pow((genData[slaveX*6+0] - genData[i*6+0]),2);
                    float R2y = pow((genData[slaveX*6+2] - genData[i*6+2]),2);
                    float R2z = pow((genData[slaveX*6+4] - genData[i*6+4]),2);

                    float DFx = (Mm/R2x)*1*G;
                    float DFy = (Mm/R2y)*1*G;
                    float DFz = (Mm/R2z)*1*G;

                    if(sycl::isinf(DFx))
                        DFx = 0;
                    if(sycl::isinf(DFy))
                        DFy = 0;
                    if(sycl::isinf(DFz))
                        DFz = 0;

                    float diffx = (genData[slaveX*6+0] - genData[i*6+0])*1;
                    float diffy = (genData[slaveX*6+2] - genData[i*6+2])*1;
                    float diffz = (genData[slaveX*6+4] - genData[i*6+4])*1;

                    DFx = sycl::copysign(DFx, diffx)*-1;
                    DFy = sycl::copysign(DFy, diffy)*-1;
                    DFz = sycl::copysign(DFz, diffz)*-1;

                    Fx += DFx;
                    Fy += DFy;
                    Fz += DFz;

                }

                nexData[slaveX*6+1] = Fx/massA[slaveX] + genData[slaveX*6+1];
                nexData[slaveX*6+3] = Fy/massA[slaveX] + genData[slaveX*6+3];
                nexData[slaveX*6+5] = Fz/massA[slaveX] + genData[slaveX*6+5];

                nexData[slaveX*6+0] = genData[slaveX*6+0] + genData[slaveX*6+1];
                nexData[slaveX*6+2] = genData[slaveX*6+2] + genData[slaveX*6+3];
                nexData[slaveX*6+4] = genData[slaveX*6+4] + genData[slaveX*6+5];
            });
        });

        jobQueue.wait();

        std::memcpy(ClastFrame, CnextFrame, sizeof(ClastFrame));

        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }
        window.clear();
        drawPoints(ClastFrame);
        window.display();

        //sleep(1);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    }

    std::cout << '\n';

    return 0;
}

void drawPoints(float ClastFrame[])
{
    sf::CircleShape shape(2);

    shape.setFillColor(sf::Color(255, 255, 255));

    for(int i = 0; i < N; i++)
    {
        shape.setPosition(ClastFrame[i*6]/10, ClastFrame[i*6+2]/10);
        window.draw(shape);
    }
}
