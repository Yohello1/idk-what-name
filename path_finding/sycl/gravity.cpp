#include <sycl/sycl.hpp>
#include <cmath>

#ifndef MAX_VAL
#define MAX_VAL 1000
#endif

constexpr int N = 5 ;


// Pos Vel
// x,0 x,1
// y,2 y,3
// z,4 z,5
//
// mass : m

int main()
{
    std::cout << "Starting Gravity thingy" << std::endl;

    // Init stuff
    //

    srand(10);

    sycl::queue jobQueue{sycl::cpu_selector_v};
    std::cout << "Selected Device: " << jobQueue.get_device().get_info<sycl::info::device::name>() << "\n";
    std::cout << "Selected Vendor: " << jobQueue.get_device().get_info<sycl::info::device::vendor>() << "\n";


    float ClastFrame[N*6];
    float CnextFrame[N*6];
    float Cmass[N];
    for(int i = 0; i < N*6; i++)
    {
        ClastFrame[i] = 10 + (rand()%MAX_VAL)/10;
        Cmass[i] = rand() % MAX_VAL+1;
        CnextFrame[i] = 0;
    }

    sycl::buffer<float> lastFrame{ClastFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> nextFrame{CnextFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> mass{Cmass, sycl::range{N}, {sycl::property::buffer::use_host_ptr()}};


    // Fx = Force X axis
    // Fy = Force Y axis, etc
    //


    jobQueue.submit([&] (sycl::handler &h){
         sycl::accessor genData{lastFrame, h, sycl::read_only};
         sycl::accessor nexData{nextFrame, h, sycl::write_only};
         sycl::accessor massA{mass, h, sycl::read_only};

         sycl::stream debugOut(4096, 1024, h);

         h.parallel_for(sycl::range{N}, [=] (sycl::id<1> it){
             int slaveX = it;
             float Fx = 0, Fy = 0, Fz = 0;

             float G = 1;
             uint32_t mask1 = 2147483650;
             uint32_t mask2 = 1;

             for(int i = 0; i < N; i++)
             {
                 // F = GMm/r^2
                 // G = 1-
                 // X-axis
                 float Mm  =     (massA[i]*massA[slaveX]);
                 float R2x = pow((genData[slaveX*6+0] - genData[i*6+0]),2) + 1;
                 float R2y = pow((genData[slaveX*6+2] - genData[i*6+2]),2) + 1;
                 float R2z = pow((genData[slaveX*6+4] - genData[i*6+4]),2) + 1;

                 float diffX = (genData[slaveX*6+0] - genData[i*6+0]);
                 float diffY = (genData[slaveX*6+2] - genData[i*6+4]);

                 int32_t SignX = (int) ((int)diffX & (int)mask1);
                 SignX = (SignX | mask2);

                 int32_t SignY = (int) ((int)diffY & (int)mask1);
                 SignY = (SignY | mask2);

                 int32_t SignZ = 1;


                 Fx += Mm/(R2x )*1*SignX*G;
                 Fy += Mm/(R2y )*1*SignY*G;
                 Fz += Mm/(R2z )*1*SignZ*G;
                 debugOut << "dataX " << diffX << ' ' << SignX << ' ' << Fx << "\n";
             }


             nexData[slaveX*6+1] = Fx/massA[slaveX] + genData[slaveX*6+1];
             nexData[slaveX*6+3] = Fy/massA[slaveX] + genData[slaveX*6+3];
             nexData[slaveX*6+5] = Fz/massA[slaveX] + genData[slaveX*6+5];

             nexData[slaveX*6+0] = genData[slaveX*6+0] + genData[slaveX*6+1];
             nexData[slaveX*6+2] = genData[slaveX*6+2] + genData[slaveX*6+3];
             nexData[slaveX*6+4] = genData[slaveX*6+4] + genData[slaveX*6+1];
         });
     });

    jobQueue.wait();

    std::cout << '\n';
    std::cout << '\n';
    std::cout << '\n';


    for(int i = 0; i < N*5; i++)
    {
        std::cout << CnextFrame[i] << " ";
        if(i%6  == 0)
            std::cout << "\n";
    }

    std::cout << '\n';

    return 0;
}
