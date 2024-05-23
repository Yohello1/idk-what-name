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
