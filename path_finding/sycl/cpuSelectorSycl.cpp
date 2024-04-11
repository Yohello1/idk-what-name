#include <sycl/sycl.hpp>
#include <iostream>

int main()
{
    sycl::queue Q{sycl::cpu_selector_v};

    std::cout << "Selected Device: " << Q.get_device().get_info<sycl::info::device::name>() << "\n";
    std::cout << "Selected Vendor: " << Q.get_device().get_info<sycl::info::device::vendor>() << "\n";

    return 0;
}
