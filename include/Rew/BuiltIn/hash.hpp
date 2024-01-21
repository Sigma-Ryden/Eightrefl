#ifndef REW_BUILT_IN_HASH_HPP
#define REW_BUILT_IN_HASH_HPP

#include <functional> // hash

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename KeyType>), (std::hash<KeyType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::hash<" + NAMEOF(KeyType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename KeyType>), (std::hash<KeyType>))
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator())
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_HASH_HPP
