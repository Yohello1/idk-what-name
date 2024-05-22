#include <chlorine/scene/EventManager.hpp>

namespace chlorine::ecs
{
    class EventManger
    {
    public:
        void addListener(EventId eventId, std::function<void(Event&)> const& listener)
        {
            listeners[eventId].push_back(listener);
        }

        void sendEvent(Event& event)
        {
            uint32_t type = event.getType();
            for(auto const& listener : listeners[type])
            {
                listener(event);
            }
        }

        void sendEvent(EventId eventId)
        {
            Event event(eventId);

            for(auto const& listener : listeners[eventId])
            {
                listener(event);
            }
        }

    }
}
