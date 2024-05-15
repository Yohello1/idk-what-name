#include <chlorine/scene/scene.hpp>

namespace chlorine::scene
{
    scene::scene(std::unique_ptr<::chlorine::logging::logBase> const &logOut)
    {
        logOut->log("Created Scene\n");
    }
}
