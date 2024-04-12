#include <sycl/sycl.hpp>
#include <cmath>

constexpr int N = 100000;

int main()
{
    std::array<int, N> my_data;

    srand(time(NULL));

    for(int i = 0; i < N; i++)
        my_data[i] = rand() % 50000;
    {

        sycl::queue q;
        sycl::buffer my_buffer(my_data);

        q.submit([&] (sycl::handler &h){
            sycl::accessor my_accessor(my_buffer, h);

            h.parallel_for(N, [=] (sycl::id<1> i){
                my_accessor[i]++;
            });
        });

        q.wait();
        sycl::host_accessor host_accessor(my_buffer);

        for(int i = 0; i < N; i++)
        {
            std::cout << host_accessor[i] << " ";
        }
        std::cout << "\n";
    }

    for(int i = 0; i < N; i++)
    {
        std::cout << my_data[i] << " ";
    }
    std::cout << "\n";
}
