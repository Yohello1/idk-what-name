#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector> // remember

namespace chlorine::scene
{
    // Dont ask what these functions do... they scare me

    template<typename C>
    void addComponentIdOne(std::unordered_map<decltype(std::declval<std::type_info>().name()), std::pair<decltype(std::declval<std::type_info>().hash_code()), std::size_t>>& map, std::size_t index)
    {
        std::type_info const& type_info = typeid(C);
        map.insert({type_info.name(), {type_info.hash_code(), index}});
    }

    template<typename C, typename... Cs>
    void addComponentId(std::unordered_map<decltype(std::declval<std::type_info>().name()), std::pair<decltype(std::declval<std::type_info>().hash_code()), std::size_t>>& map, std::size_t index)
    {
        addComponentIdOne<C>(map, index);

        std::size_t const new_index = index + 1;
        if constexpr(sizeof...(Cs) >= 1)
        {
            addComponentId<Cs...>(map, new_index);
        }
    }

    template<typename... Cs>
    std::vector<std::uint64_t> transformEntIds(std::vector<std::pair<std::uint64_t, std::variant<Cs...>>> const& input)
    {
        std::vector<std::uint64_t> result;
        std::transform(begin(input), end(input), std::back_inserter(result), [](auto const& pair){
            return pair.first;
        });
        return result;
    }

    // EntityId = std::uint64_t;
    // componentId = decltype(std::declval<std::type_info>().hash_code())
    // componentNameType = decltype(std::declval<std::type_info>().name())


    template<typename... Cs>
    class conductor
    {
        // I dont like using `using`, but here it actually helps :P
        using component = std::variant<Cs...>;

    public:
        conductor() : _typeArrays{&typeid(Cs)...}
        {
            addComponentId<Cs...>(_componentIds, 0);
        }

        std::vector<std::uint64_t> get(std::vector<decltype(std::declval<std::type_info>().hash_code())> components)
        {
            // there's gotta be a better way to do this :P
            if(components.size() < 1)
            {
                return {};
            }

            std::vector<uint64_t> result;
            // if(auto const found = _entities.find(components[0]); found == end(_entities))
            for(auto const componentId : components)
            {
                std::vector<std::uint64_t> nextComponent;
                std::vector<std::uint64_t> tempResult;

                if(auto const found = _entities.find(componentId); found == end(_entities))
                {
                    return {};
                }
                else
                {
                    nextComponent = transformEntIds(found->second);
                }

                std::set_intersection(begin(result), end(result), begin(nextComponent), end(nextComponent), back_inserter(tempResult));
                result = tempResult;
            }
        }

        template<typename C>
        std::optional<std::uint64_t> id()
        {
            // Gotta also be a better way
            auto const& typeInfo = typeid(C);

            for(std::size_t i = 0; i < _typeArrays.size(); ++i)
            {
                auto const type_hash = _typeArrays[i]->hash_code();
                if(type_hash == typeInfo.hash_code())
                {
                    return std::make_optional<decltype(std::declval<std::type_info>().hash_code())>(type_hash);
                }
            }

            return std::nullopt;
        }

        void set(std::uint64_t entId, std::variant<Cs...> const& component)
        {
            auto const index = component.index();
            auto const componentHash = _typeArrays[index]->hash_code();

            auto found = _entities.find(componentHash);
            if(found != end(_entities))
            {
                found->second.push_back({entId, component});
            }

            _entities.insert({componentHash, {{entId, component}}});
        }
    private:
        // Storing ids, and variants
        std::unordered_map<decltype(std::declval<std::type_info>().name()), std::pair<decltype(std::declval<std::type_info>().hash_code()), std::size_t>> _componentIds;

        // actual entity data
        // std::unordered_map<decltype(std::declval<std::type_info>().hash_code()), std::vector<std::pair<std::uint64_t, component<Cs...>>>> _entities;
        std::unordered_map<decltype(std::declval<std::type_info>().hash_code()), std::vector<std::pair<std::uint64_t, std::variant<Cs...>>>> _entities;

        // typesids
        std::array<std::type_info const*, sizeof...(Cs)> _typeArrays;
    };
}
