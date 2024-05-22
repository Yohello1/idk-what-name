#include <chlorine/scene/component.hpp>
#pragma once

class boxes : public ::chlorine::scene::component
{
    public:
    std::pair<uint32_t, uint32_t> position;

    void dumpData(std::unique_ptr<::chlorine::logging::logBase>)
};
