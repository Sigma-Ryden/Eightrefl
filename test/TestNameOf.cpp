#include <RewTestingBase.hpp>

TEST(TestLibrary, TestNameOf)
{
    EXPECT("const-type", rew::meta::reflectable_traits<const int>::name() == "int const");
    EXPECT("reference-type", rew::meta::reflectable_traits<int&>::name() == "int&");
    EXPECT("const_reference-type", rew::meta::reflectable_traits<const int&>::name() == "int const&");
    EXPECT("function-type", rew::meta::reflectable_traits<void(int, float)>::name() == "void(int, float)");
    EXPECT("const-function-type", rew::meta::reflectable_traits<void(int, float) const>::name() == "void(int, float) const");
    EXPECT("reference-function-type", rew::meta::reflectable_traits<void(int, float)&>::name() == "void(int, float)&");
    EXPECT("const_reference-function-type", rew::meta::reflectable_traits<void(int, float) const&>::name() == "void(int, float) const&");
}

// Patterns:
// array<T, N> ~ T[N]
// array<T, N>& ~ T(&)[N]
// array<T, N> const& ~ T( const&)[N]
// array<T, N>* ~ T(*)[N]
// array<T, N> const* ~ T( const*)[N]
// array<T, N>* const ~ T(* const)[N]
// array<T, N> const* const ~ T( const* const)[N]

// function<R(Args...)>& ~ R(&)(Args...)
// function<R(Args...)> const& ~ R( const&)(Args...)
// function<R(Args...) ~ R(Args...)
// function<R(Args...)>* ~ R(*)(Args...)
// function<R(Args...)> const* ~ R( const*)(Args...)
// function<R(Args...)>* const ~ R(* const)(Args...)
// function<R(Args...)> const* const ~ R( const* const)(Args...)

// Pointers/References as T/R:
// if T is type pointer/reference of function or array,
// we should wrapp Pattern to '(' and ')' braces, examples:
// array<array<T, InnerN>*, N>* ~ T(*(*)[N])[InnerN]
// array<function<R(Args...)>*, N>* ~ R(*(*)[N])(Args...)

// if R is type pointer/reference of function or array,
// we should wrapp Pattern to '(' and ')' braces, examples:
// function<function<R(InnerArgs...)>*(Args...)>* ~ InnerR(*(*)(Args...))(InnerArgs...)
// function<array<T, N>*(Args...)>* ~ T(*(*)(Args...))[N]
