#pragma once

#include <bitset>
#include <cstdint>

inline constexpr std::uint32_t fnv1a_32(char const *s, std::size_t count)
{
    return count ? (fnv1a_32(s, count - 1) ^ s[count - 1]) * 16777619u : 2166136261u; // https://gist.github.com/Lee-R/3839813?permalink_comment_id=4018536#gistcomment-4018536
}


constexpr std::uint32_t operator "" _hash(char const* s, std::size_t count)
{
    return fnv1a_32(s, count);
}


using Entity = std::uint32_t;
const Entity MAX_ENTITIES = 65536; // idk more seems sketch
using ComponentType = std::uint8_t;
const ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;

using EventId = std::uint32_t;
using ParamId = std::uint32_t;
