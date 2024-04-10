#include <sycl/sycl.hpp>

const std::string secret {
"Ifmmp-!xpsme\"\012J(n!tpssz-!Ebwf/!"
"J(n!bgsbje!J!dbo(u!ep!uibu/!.!IBM\01"
};
const auto sz = secret.size();

int main()
{
    sycl::queue Q;

    char* result = malloc_shared<char>(sz, Q);

    std::memcpy(result, secret.data(), sz);

    Q.parallel_for(sz, [=] (auto&i){
        result[i] -= 1;
    }).wait();

    std::cout << result << "\n";
    return 0;
}
