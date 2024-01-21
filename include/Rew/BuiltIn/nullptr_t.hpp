#ifndef REW_BUILT_IN_NULLPTR_T_HPP
#define REW_BUILT_IN_NULLPTR_T_HPP

#include <cstddef> // nullptr_t

#include <Rew/Reflectable.hpp>

REFLECTABLE_DECLARATION(std::nullptr_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

#endif // REW_BUILT_IN_NULLPTR_T_HPP
