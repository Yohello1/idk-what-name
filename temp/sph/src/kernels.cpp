#include <concepts>
#include <vector>
#include <iostream>
#include "settings.hpp"

// concepts are like a list of characteristics
template<typename T>
concept IsKernel = requires(T k) {
    { k.density() } -> std::floating_point;
    { k.pressure() } -> std::floating_point;
};

template <typename T>
concept HasViscosity = requires(T k) {
    { k.viscosity() } -> std::floating_point;
};

// you can do 'template <[concept name] T>' to ensure that only types with those
// characterisitcs can be used there, or check etc

template <IsKernel T>
void simulate() {
    // fully boilerplate code
    T kernel;
    
    double d = kernel.get_density();
    double p = kernel.get_pressure();

    if constexpr (HasViscosity<T>) {
        double v = kernel.get_viscosity();
        std::cout << "Using Kernel Viscosity: " << v << std::endl;
    } else {
        double v_approx = d * 0.0001; 
        std::cout << "Using Approximated Viscosity: " << v_approx << std::endl;
    }
}


class poly6_kernel
{
    public:
   
        void density()
        {
            // density calc 
        }

        void pressure()
        {
            // density calc 
        }
        
        void viscosity()
        {
            // density calc 
        }
};
