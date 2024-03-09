#ifndef REW_BUILTIN_TYPE_INDEX_HPP
#define REW_BUILTIN_TYPE_INDEX_HPP

#include <typeindex> // type_index

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/typeinfo.hpp>

REFLECTABLE_DECLARATION(std::type_index)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::type_index)
    FACTORY(R(std::type_info const&))
#if __cplusplus < 202002L
    FUNCTION(operator==)
    FUNCTION(operator!=)
    FUNCTION(operator<)
    FUNCTION(operator<=)
    FUNCTION(operator>)
    FUNCTION(operator>=)
#else
    // TODO: add support for std::strong_ordering
#endif // if
    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_TYPE_INDEX_HPP
