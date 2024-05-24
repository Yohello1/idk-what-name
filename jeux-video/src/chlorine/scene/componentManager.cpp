#include <chlorine/scene/componentManager.hpp>

namespace chlorine::scene
{

    template<typename compT> // component Template
    bool orchestrator::setValue(std::string key, compT value, std::unique_ptr<chlorine::logging::logBase> const &logOut)
    {
        std::type_info const& type_info = typeid(compT);
        map.insert({key, {type_info.hash_code(), value}});

        logOut->log("Logged without crashing!");

        return true;
    }

}
