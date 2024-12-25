#include <iostream>
#include <variant>
#include <unordered_map>
#include <string>
#include <vector>
#include <cstdint>



template<typename... Cs>
using Component = std::variant<Cs...>;

using ComponentId = decltype(std::declval<std::type_info>().hash_code());
using EntityId = std::uint64_t;

template<typename... Ts>
struct variant_unwrapper {
    std::variant<Ts...> & var;
    template <typename T>
    operator T() { return std::get<T>(var); }
};

template<typename... Cs>
class ecsSystem
{
public:
    // std::unordered_map<ComponentId, std::vector<std::pair<EntityId, Component<Cs...>>>> _entities;
    // std::array<std::type_info const*, sizeof...(Cs)> _type_arrays;

    // You need c++20 at least to compile this

    std::unordered_map<std::string, std::unordered_map<std::string, Component<Cs...>>> tempArray;

    void insertElement(std::string key1, std::string key2, Component<Cs...> const& component)
    {
        tempArray[key1][key2] = component;
    }


    int getElement(std::string key1, std::string key2)
    {
        return std::get<int>(tempArray[key1][key2]);
    }

    template<typename T>
    T getE(std::string key1, std::string key2)
    {
        return variant_unwrapper{ tempArray[key1][key2] };
    }
};


int main()
{
    std::cout << "Hello world" << std::endl;

    ecsSystem<int> mySystem;
    mySystem.insertElement("hi", "no", 55);
    mySystem.insertElement("hi", "why", 78);
    mySystem.insertElement("holdin", "why", 33);

    // std::cout << "Value: " << mySystem.getElement("hi", "why") << std::endl;
    auto temppp =  mySystem.getElement("hi", "why");
    auto temp1 = mySystem.getElement("hi", "no");
    auto temp2 = mySystem.getElement("holdin", "why");


    std::variant<float, int> v[2] = { 1, 1.0f };
    int a = variant_unwrapper{ mySystem.tempArray["hi"]["why"] };
    // int b = variant_unwrapper{ mySystem.getElement("hi", "why")};
    // std::cout << "Values: " <<  ' ' << temppp << ' ' << temp2;
    int temp3 = mySystem.getE<int>("hi", "why");
}
