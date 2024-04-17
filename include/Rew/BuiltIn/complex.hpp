#ifndef REW_BUILTIN_COMPLEX_HPP
#define REW_BUILTIN_COMPLEX_HPP

#include <complex> // complex

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

template <typename> struct __rew_std_complex_pass_value;

template <typename ValueType>
struct __rew_std_complex_pass_value<std::complex<ValueType>>
{
    using type = ValueType const&;
};

template <> struct __rew_std_complex_pass_value<std::complex<float>> { using type = float; };
template <> struct __rew_std_complex_pass_value<std::complex<double>> { using type = double; };
template <> struct __rew_std_complex_pass_value<std::complex<long double>> { using type = long double; };

TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename ValueType>), (std::complex<ValueType>),
    std::complex<CLEANOF(ValueType)>
)

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), std::complex<ValueType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::complex<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType>), std::complex<ValueType>)
    FACTORY(R())
    FACTORY(R(typename __rew_std_complex_pass_value<R>::type, typename __rew_std_complex_pass_value<R>::type))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator+=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator+=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator-=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator-=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator*=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator*=, R&(R const&))

    #ifndef REW_CORE_MINIMAL
    FUNCTION(operator/=, R&(typename __rew_std_complex_pass_value<R>::type))
    #endif // REW_CORE_MINIMAL

    FUNCTION(operator/=, R&(R const&))
    PROPERTY(real)
    PROPERTY(imag)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_COMPLEX_HPP
