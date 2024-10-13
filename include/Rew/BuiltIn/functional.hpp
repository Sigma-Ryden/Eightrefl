#ifndef REW_BUILTIN_FUNCTIONAL_HPP
#define REW_BUILTIN_FUNCTIONAL_HPP

#include <functional> // hash

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_CLEAN
(
    template <typename KeyType>,
    std::hash<KeyType>, std::hash<rew::cleanof<KeyType>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename KeyType>, std::hash<KeyType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::hash<" + rew::nameof<KeyType>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename KeyType>, std::hash<KeyType>)
    #ifdef REW_FULLY_ENABLE
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator())
    #endif // REW_FULLY_ENABLE
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::equal_to<T>, std::equal_to<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::equal_to<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::equal_to<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::equal_to<T>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::less<T>, std::less<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::less<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::less<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::less<T>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN(template <typename T>, std::greater<T>, std::greater<rew::cleanof<T>>)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename T>, std::greater<T>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::greater<" + rew::nameof<T>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename T>, std::greater<T>)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_FUNCTIONAL_HPP
