#pragma once

#include <chlorine/scene/Event.hpp>
#include <chlorine/scene/Types.hpp>
#include <functional>
#include <list>
#include <unordered_map>

namespace chlorine::ecs
{
    class EventManger
    {
    public:
        void addListener(EventId eventId, std::function<void(Event&)> const& listener);
        void sendEvent(Event& event);
        void sendEvent(EventId eventId);

    private:
        std::unordered_map<EventId, std::list<std::function<void(Event&)>>> listeners;
    }
}
