#include <Eightrefl/Standard/any.hpp>

#include <Eightrefl/BuiltIn/Core.hpp>

// as function return type
#include <Eightrefl/Standard/typeinfo.hpp>

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
