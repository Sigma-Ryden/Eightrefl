#ifndef REW_BUILTIN_TYPE_INDEX_HPP
#define REW_BUILTIN_TYPE_INDEX_HPP

#include <typeindex> // type_index

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function argument type
#include <Rew/BuiltIn/typeinfo.hpp>

#if __cplusplus > 201703L
#include <Rew/BuiltIn/compare.hpp>
#endif // if

REFLECTABLE_DECLARATION(std::type_index)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::type_index)
    FACTORY(R(std::type_info const&))

#if __cplusplus > 201703L
    FUNCTION(operator<=>)
#else
    FUNCTION(operator==)
    FUNCTION(operator!=)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator<)
    FUNCTION(operator<=)
    FUNCTION(operator>)
    FUNCTION(operator>=)
    #endif // REW_CORE_MINIMAL
#endif // if
    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_TYPE_INDEX_HPP
