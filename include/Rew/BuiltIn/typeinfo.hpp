#ifndef REW_BUILT_IN_TYPE_INFO_HPP
#define REW_BUILT_IN_TYPE_INFO_HPP

#include <typeinfo> // type_info

#include <Rew/Reflectable.hpp>

REFLECTABLE_DECLARATION(std::type_info)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::type_info)
    FUNCTION(operator==)
    FUNCTION(before)
    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_TYPE_INFO_HPP
