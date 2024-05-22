#pragma once

#include <chlorine/scene/Types.hpp>
#include <set>

namespace chlorine::ecs
{
    class System
    {
    public:
        std::set<Entity> _Entities;
    }
}
