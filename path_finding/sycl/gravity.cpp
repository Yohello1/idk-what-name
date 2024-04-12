#include <sycl/sycl.hpp>
#include <cmath>

#ifndef MAX_VAL
#define MAX_VAL 1000
#endif

constexpr int N = 42;


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

    srand(time(NULL));

    sycl::queue jobQueue{sycl::cpu_selector_v};
    std::cout << "Selected Device: " << jobQueue.get_device().get_info<sycl::info::device::name>() << "\n";
    std::cout << "Selected Vendor: " << jobQueue.get_device().get_info<sycl::info::device::vendor>() << "\n";


    float ClastFrame[N*6];
    float CnextFrame[N*6];
    float Cmass[N];
    for(int i = 0; i < N*6; i++)
    {
        ClastFrame[i] = rand() % MAX_VAL;
        Cmass[i] = rand() % MAX_VAL;
        CnextFrame[i] = 0;
    }

    sycl::buffer<float> lastFrame{ClastFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> nextFrame{CnextFrame, sycl::range{N*6}, {sycl::property::buffer::use_host_ptr()}};
    sycl::buffer<float> mass{Cmass, sycl::range{N}, {sycl::property::buffer::use_host_ptr()}};


    jobQueue.submit([&] (sycl::handler &h){
         sycl::accessor genData{lastFrame, h, sycl::read_only};
         sycl::accessor nexData{nextFrame, h, sycl::write_only};
         sycl::accessor massA{mass, h, sycl::read_only};

         h.parallel_for(sycl::nd_range{global, local}, [=] (sycl::nd_item<1> it){
             int slaveX = it.get_global_id(0);
             int slaveY = it.get_global_id(1);

             nexData[i] = 10/0;
         });
     });

    jobQueue.wait();

    for(int i = 0; i < N; i++)
    {
        std::cout << CnextFrame[i] << " ";
    }

    std::cout << '\n';

    return 0;
}
