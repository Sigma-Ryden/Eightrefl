#include <RewTestingBase.hpp>

TEST(TestLibrary, TestNameOf)
{
    EXPECT("const-type", rew::meta::reflectable_traits<const int>::name() == "int const");
    EXPECT("pointer-type", rew::meta::reflectable_traits<int*>::name() == "int*");
    EXPECT("reference-type", rew::meta::reflectable_traits<int&>::name() == "int&");
    EXPECT("function-type", rew::meta::reflectable_traits<void(int, float)>::name() == "void(int, float)");
    EXPECT("function-pointer-type", rew::meta::reflectable_traits<void(*)(int, float)>::name() == "std::type_identity_t<void(int, float)>*");
    EXPECT("function-reference-type", rew::meta::reflectable_traits<void(&)(int, float)>::name() == "std::type_identity_t<void(int, float)>&");
    EXPECT("const-qualified_function-type", rew::meta::reflectable_traits<void(int, float) const>::name() == "void(int, float) const");
    EXPECT("reference-qualified_function-type", rew::meta::reflectable_traits<void(int, float)&>::name() == "void(int, float)&");
    EXPECT("const-reference_qualified_function-type", rew::meta::reflectable_traits<void(int, float) const&>::name() == "void(int, float) const&");
    EXPECT("array-type", rew::meta::reflectable_traits<int[8]>::name() == "int[8]");
    EXPECT("array-pointer-type", rew::meta::reflectable_traits<int(*)[8]>::name() == "std::type_identity_t<int[8]>*");
    EXPECT("array-reference-type", rew::meta::reflectable_traits<int(&)[8]>::name() == "std::type_identity_t<int[8]>&");
    EXPECT("mixed-type", rew::meta::reflectable_traits<char(*(* const*&)[8])(void)>::name() == "std::type_identity_t<std::type_identity_t<char()>*[8]>* const*&");
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
