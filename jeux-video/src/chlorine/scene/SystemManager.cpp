#include <chlorine/scene/SystemManager.hpp>

namespace chlorine::ecs
{
    class systemManager
    {
    public:
        template<typename T>
        std::shared_ptr<T> registerSystem()
        {
            const char* typeName = typeid(T).name();
            assert(_Systems.find(typeName) == _Systems.end() && "Registering system more than once");

            auto system = std::make_shred<T>();
            _Systems.insert({typeName, system});
            return system;
        }

        template<typename T>
        void setSignature(Signature signature)
        {
            const char* typeName = typeid(T).name();

            assert(_Systems.find(typeName_ != _Systems.end() && "System used before registration"));

            _Signatures.insert({typeName, Signature});
        }

        template<typename T>
        void entityDestroyed(Entity entity)
        {
            for(auto const& pair : _Systems)
            {
                auto const& system = pair.second;
                system->_Entities.erase(entity);
            }
        }

        void entitySignatureChanged(Entity entity, Signature entitySignature)
        {
            for(auto const& pair : _Systems)
            {
                auto const& type = pair.first;
                auto const& system = pair.second;
                auto const& systemSignature = _Signatures[type];

                if((entitySignature & systemSignature) == systemSignature)
                {
                    system->_Entities.insert(entity);
                }
                else
                {
                    system->_Entities.erase(entity);
                }
            }
        }


    };
}
