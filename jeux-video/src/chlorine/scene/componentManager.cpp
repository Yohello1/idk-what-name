#include <chlorine/scene/componentManager.hpp>

namespace chlorine::scene
{

    // Note: this has already been defined in the hpp file, cause templated func :P
    /*
    template<typename T, typename... Args>
    void orchestra::insertElement(std::string key1, Args&&... args)
    {
        instruments[key1].emplace(typeid(T), std::make_unique<T>(std::forward<Args...>(args...)));
    }
    */
}
