#pragma once

#include <chlorine/scene/System.hpp>
#include <chlorine/scene/Types.hpp>
#include <cassert>
#include <memory>
#include <unordered_map>

namespace chlorine::ecs
{
    class systemManager
    {
    public:
        template<typename T>
        std::shared_ptr<T> registerSystem();

        template<typename T>
        void setSignature(Signature signature);

        template<typename T>
        void entityDestroyed(Entity entity);

        void entitySignatureChanged(Entity entity, Signature entitySignature);

    private:
        std::unordered_map<const char*, Signature> _Signatures{};
        std::unordered_map<const char*, std::shared_ptr<System>> _Systems{};
    };
}
