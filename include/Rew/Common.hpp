#ifndef REW_COMMON_HPP
#define REW_COMMON_HPP

#include <cstddef> // nullptr_t, size_t, ptrdiff_t

#include <string> // to_string

#include <Rew/Reflectable.hpp>

// incomplete type
REFLECTABLE_DECLARATION(void)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

// pointer type
TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType*, rew::cleanof<ElementType>*)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType*)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "*")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE(template <typename ElementType>, ElementType*)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

// nullptr type
REFLECTABLE_DECLARATION(std::nullptr_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

// qualified types
TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType&, rew::cleanof<ElementType>&)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType&)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN(template <typename ElementType>, ElementType const, rew::cleanof<ElementType> const)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ElementType>, ElementType const)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + " const")
REFLECTABLE_DECLARATION_INIT()
// ~ qualified types

// static array type
TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ElementType, std::size_t SizeValue>),
    ElementType[SizeValue], rew::cleanof<ElementType>[SizeValue]
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t SizeValue>), ElementType[SizeValue])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ElementType>() + "[" + std::to_string(SizeValue) + "]")
REFLECTABLE_DECLARATION_INIT()

// static array pointer type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t SizeValue>), ElementType(*)[SizeValue])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ElementType[SizeValue]>() + ">*")
REFLECTABLE_DECLARATION_INIT()

// static array reference type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t SizeValue>), ElementType(&)[SizeValue])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ElementType[SizeValue]>() + ">&")
REFLECTABLE_DECLARATION_INIT()

// function types
TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)), rew::cleanof<ReturnType>(rew::cleanof<ArgumentTypes>...)
)

TEMPLATE_REFLECTABLE_DECLARATION(template <typename ReturnType>, ReturnType())
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType>() + "()")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename ArgumentType, typename... ArgumentTypes>),
    ReturnType(ArgumentType, ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType>() + "(" + ( rew::nameof<ArgumentType>() + ... + (", " + rew::nameof<ArgumentTypes>()) ) + ")")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)&), rew::cleanof<ReturnType(ArgumentTypes...)>&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...)&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const), rew::cleanof<ReturnType(ArgumentTypes...)> const
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + " const")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const&), rew::cleanof<ReturnType(ArgumentTypes...)> const&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(rew::nameof<ReturnType(ArgumentTypes...)>() + " const&")
REFLECTABLE_DECLARATION_INIT()
// ~ function types

// function pointer type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(*)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ReturnType(ArgumentTypes...)>() + ">*")
REFLECTABLE_DECLARATION_INIT()

// function reference type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(&)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + rew::nameof<ReturnType(ArgumentTypes...)>() + ">&")
REFLECTABLE_DECLARATION_INIT()

// aliasing
REFLECTABLE_USING(std_size_t, std::size_t)

CUSTOM_REFLECTABLE_DECLARATION(std_size_t)
    REFLECTABLE_NAME("std::size_t")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_USING(std_ptrdiff_t, std::ptrdiff_t)

CUSTOM_REFLECTABLE_DECLARATION(std_ptrdiff_t)
    REFLECTABLE_NAME("std::ptrdiff_t")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
// ~ aliasing

// boolean type
REFLECTABLE_DECLARATION(bool)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
// ~ boolean type

// char types
REFLECTABLE_DECLARATION(char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(wchar_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

#if __cplusplus >= 202002L
REFLECTABLE_DECLARATION(char8_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
#endif // if

REFLECTABLE_DECLARATION(char16_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(char32_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
// ~ char types

// integral types
REFLECTABLE_DECLARATION(signed char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(unsigned char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(short)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(unsigned short)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(unsigned int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(unsigned long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(unsigned long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
// ~ integral types

// floating point types
REFLECTABLE_DECLARATION(float)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_DECLARATION(long double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()
// ~ floating point types

#endif // REW_COMMON_HPP
