#ifndef REW_BUILTIN_REFERENCE_WRAPPER_HPP
#define REW_BUILTIN_REFERENCE_WRAPPER_HPP

#include <functional> // reference_wrapper

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), std::reference_wrapper<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::reference_wrapper<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType>), std::reference_wrapper<ValueType>)
    FACTORY(R(ValueType const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    RAW_FUNCTION("operator=" + NAMEOF(ValueType&), operator ValueType&)
    FUNCTION(get)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_REFERENCE_WRAPPER_HPP
