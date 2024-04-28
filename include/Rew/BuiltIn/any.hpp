#ifndef REW_BUILTIN_ANY_HPP
#define REW_BUILTIN_ANY_HPP

#include <any> // any

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as function return type
#include <Rew/BuiltIn/typeinfo.hpp>

REFLECTABLE_DECLARATION(std::any)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::any)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset)

    #ifndef REW_CORE_MINIMAL
    FUNCTION(swap)
    #endif // REW_CORE_MINIMAL

    FUNCTION(has_value)
    FUNCTION(type)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_ANY_HPP
