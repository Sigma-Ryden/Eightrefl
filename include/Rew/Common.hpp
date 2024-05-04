#ifndef REW_COMMON_HPP
#define REW_COMMON_HPP

#include <cstddef> // nullptr_t, size_t, ptrdiff_t

#include <string> // to_string

#include <Rew/Reflectable.hpp>

// incomplete type
REFLECTABLE_DECLARATION(void)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(void)
REFLECTABLE_INIT()

// pointer type
TEMPLATE_REFLECTABLE_CLEAN((template <typename ElementType>), (ElementType*), CLEANOF(ElementType)*)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), ElementType*)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ElementType) + "*")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ElementType>), ElementType*)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

// nullptr type
REFLECTABLE_DECLARATION(std::nullptr_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::nullptr_t)
REFLECTABLE_INIT()

// qualified types
TEMPLATE_REFLECTABLE_CLEAN((template <typename ElementType>), (ElementType&), CLEANOF(ElementType)&)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), ElementType&)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ElementType) + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN((template <typename ElementType>), (ElementType const), CLEANOF(ElementType) const)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType>), ElementType const)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ElementType) + " const")
REFLECTABLE_DECLARATION_INIT()
// ~ qualified types

// static array type
TEMPLATE_REFLECTABLE_CLEAN((template <typename ElementType, std::size_t N>), (ElementType[N]), CLEANOF(ElementType)[N])

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ElementType) + "[" + std::to_string(N) + "]")
REFLECTABLE_DECLARATION_INIT()

// static array pointer type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType(*)[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + NAMEOF(ElementType[N]) + ">*")
REFLECTABLE_DECLARATION_INIT()

// static array reference type
TEMPLATE_REFLECTABLE_DECLARATION((template <typename ElementType, std::size_t N>), ElementType(&)[N])
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + NAMEOF(ElementType[N]) + ">&")
REFLECTABLE_DECLARATION_INIT()

// function types
TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)), CLEANOF(ReturnType)(CLEANOF(ArgumentTypes)...)
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ReturnType>), ReturnType())
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ReturnType) + "()")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename ArgumentType, typename... ArgumentTypes>),
    ReturnType(ArgumentType, ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ReturnType) + "(" + ( NAMEOF(ArgumentType) + ... + (", " + NAMEOF(ArgumentTypes)) ) + ")")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...)&), CLEANOF(ReturnType(ArgumentTypes...))&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...)&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ReturnType(ArgumentTypes...)) + "&")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const), CLEANOF(ReturnType(ArgumentTypes...)) const
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ReturnType(ArgumentTypes...)) + " const")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    (ReturnType(ArgumentTypes...) const&), CLEANOF(ReturnType(ArgumentTypes...)) const&
)

TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>),
    ReturnType(ArgumentTypes...) const&
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(ReturnType(ArgumentTypes...)) + " const&")
REFLECTABLE_DECLARATION_INIT()
// ~ function types

// function pointer type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(*)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + NAMEOF(ReturnType(ArgumentTypes...)) + ">*")
REFLECTABLE_DECLARATION_INIT()

// function reference type
TEMPLATE_REFLECTABLE_DECLARATION
(
    (template <typename ReturnType, typename... ArgumentTypes>), ReturnType(&)(ArgumentTypes...)
)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::type_identity_t<" + NAMEOF(ReturnType(ArgumentTypes...)) + ">&")
REFLECTABLE_DECLARATION_INIT()

// aliasing
REFLECTABLE_USING(std_size_t, std::size_t)

RAW_REFLECTABLE_DECLARATION(std_size_t)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::size_t")
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std_size_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_USING(std_ptrdiff_t, std::ptrdiff_t)

RAW_REFLECTABLE_DECLARATION(std_ptrdiff_t)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ptrdiff_t")
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std_ptrdiff_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ aliasing

// boolean type
REFLECTABLE_DECLARATION(bool)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(bool)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ boolean type

// char types
REFLECTABLE_DECLARATION(char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(wchar_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(wchar_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

#if __cplusplus >= 202002L
REFLECTABLE_DECLARATION(char8_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char8_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
#endif // if

REFLECTABLE_DECLARATION(char16_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char16_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(char32_t)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(char32_t)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ char types

// integral types
REFLECTABLE_DECLARATION(signed char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(signed char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned char)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned char)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(int)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned int)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned int)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(unsigned long long)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(unsigned long long)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ integral types

// floating point types
REFLECTABLE_DECLARATION(float)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(float)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(double)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()

REFLECTABLE_DECLARATION(long double)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(long double)
    FACTORY(R())
    FACTORY(R(R))
REFLECTABLE_INIT()
// ~ floating point types

#endif // REW_COMMON_HPP
