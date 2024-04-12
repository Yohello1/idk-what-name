#include <sycl/sycl.hpp>

constexpr int N = 42;

int main()
{
    sycl::queue Q;

    int *host_array = malloc_host<int>(N,Q);
    int *shared_array = malloc_host<int>(N,Q);


    for(int i = 0; i < N; i++)
        host_array[i] = i;



    Q.submit([&] (sycl::handler &h){
        h.parallel_for(N, [=](sycl::id<1> i) {
            shared_array[i] = host_array[i] + 1;

        });
    });
    Q.wait();


    for(int i = 0; i < N; i++)
    {
        std::cout << "data " << i << " = " << host_array[i] << std::endl;
    }

    free(host_array, Q);
    free(shared_array, Q);

    return 0;
}
