#ifndef REW_BUILTIN_REFERENCE_WRAPPER_HPP
#define REW_BUILTIN_REFERENCE_WRAPPER_HPP

#include <functional> // reference_wrapper

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename ValueType>,
    std::reference_wrapper<ValueType>, std::reference_wrapper<rew::cleanof<ValueType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ValueType>, std::reference_wrapper<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::reference_wrapper<" + rew::nameof<ValueType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ValueType>, std::reference_wrapper<ValueType>)
    FACTORY(R(ValueType&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    NAMED_FUNCTION(("operator " + rew::nameof<ValueType&>()), operator rew::cleanof<ValueType>&)
    #endif // REW_CORE_MINIMAL

    FUNCTION(get)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_REFERENCE_WRAPPER_HPP
