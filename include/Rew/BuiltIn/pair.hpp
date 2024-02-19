#ifndef REW_BUILTIN_PAIR_HPP
#define REW_BUILTIN_PAIR_HPP

#include <utility> // pair

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::pair<" + NAMEOF(FirstType) + ", " + NAMEOF(SecondType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename FirstType, typename SecondType>), std::pair<FirstType, SecondType>)
    FACTORY(R())
    FACTORY(R(typename R::first_type const&, typename R::second_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(swap)
    PROPERTY(first)
    PROPERTY(second)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_PAIR_HPP
