// TODO: add specializations for: complex<float>, complex<double>, complex<long double>
#ifndef REW_BUILT_IN_COMPLEX_HPP
#define REW_BUILT_IN_COMPLEX_HPP

#include <complex> // complex

#include <Rew/Reflectable.hpp>
#include <Rew/Common.hpp>

TEMPLATE_REFLECTABLE_DECLARATION((template <typename ValueType>), (std::complex<ValueType>))
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::complex<" + NAMEOF(ValueType) + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename ValueType>), (std::complex<ValueType>))
    FACTORY(R())
    FACTORY(R(typename R::value_type const&, typename R::value_type const&))
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(operator+=, R&(typename R::value_type const&))
    FUNCTION(operator+=, R&(R const&))
    FUNCTION(operator-=, R&(typename R::value_type const&))
    FUNCTION(operator-=, R&(R const&))
    FUNCTION(operator*=, R&(typename R::value_type const&))
    FUNCTION(operator*=, R&(R const&))
    FUNCTION(operator/=, R&(typename R::value_type const&))
    FUNCTION(operator/=, R&(R const&))
    PROPERTY(real)
    PROPERTY(imag)
REFLECTABLE_INIT()

#endif // REW_BUILT_IN_COMPLEX_HPP
