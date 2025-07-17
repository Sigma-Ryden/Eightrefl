#include <Eightrefl/BuiltIn/any.hpp>

#include <Eightrefl/Common.hpp>

// as function return type
#include <Eightrefl/BuiltIn/typeinfo.hpp>

REFLECTABLE(std::any)
    FACTORY(R())
    FACTORY(R(R const&))
    FUNCTION(operator=, R&(R const&))
    FUNCTION(reset)

    #ifdef EIGHTREFL_FULLY_ENABLE
    FUNCTION(swap)
    #endif // EIGHTREFL_FULLY_ENABLE

    FUNCTION(has_value)
    FUNCTION(type)
REFLECTABLE_INIT()
