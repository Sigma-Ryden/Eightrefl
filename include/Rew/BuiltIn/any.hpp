#ifndef REW_BUILT_IN_ANY_HPP
#define REW_BUILT_IN_ANY_HPP

#include <any> // any

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

// as return type of the type() function
#include <Rew/BuiltIn/typeinfo.hpp>

REFLECTABLE_DECLARATION(std::any)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::any)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset)
    FUNCTION(swap)
    FUNCTION(has_value)
    FUNCTION(type)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_ANY_HPP
