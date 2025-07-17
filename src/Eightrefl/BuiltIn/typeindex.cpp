#include <Eightrefl/BuiltIn/typeindex.hpp>

#include <Eightrefl/Common.hpp>

// as function argument type
#include <Eightrefl/BuiltIn/typeinfo.hpp>

#if __cplusplus > 201703L
#include <Eightrefl/BuiltIn/compare.hpp>
#endif // if

REFLECTABLE(std::type_index)
    FACTORY(R(std::type_info const&))

#if __cplusplus > 201703L
    FUNCTION(operator<=>)
#else
    FUNCTION(operator==)
    FUNCTION(operator!=)

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(operator<)
    FUNCTION(operator<=)
    FUNCTION(operator>)
    FUNCTION(operator>=)
    #endif // EIGHTREFL_FULLY_ENABLE
#endif // if
    FUNCTION(hash_code)
    FUNCTION(name)
REFLECTABLE_INIT()
