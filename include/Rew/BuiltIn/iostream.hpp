#ifndef REW_BUILTIN_IOSTREAM_HPP
#define REW_BUILTIN_IOSTREAM_HPP

#include <Rew/Reflectable.hpp>

#include <ios> // ios_base

REFLECTABLE_USING(std_ios_base_openmode, std::ios_base::openmode)

REFLECTABLE_DECLARATION(std_ios_base_openmode)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_fmtflags, std::ios_base::fmtflags)

REFLECTABLE_DECLARATION(std_ios_base_fmtflags)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_iostate, std::ios_base::iostate)

REFLECTABLE_DECLARATION(std_ios_base_iostate)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_seekdir, std::ios_base::seekdir)

REFLECTABLE_DECLARATION(std_ios_base_seekdir)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_event, std::ios_base::event)

REFLECTABLE_DECLARATION(std_ios_base_event)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_event_callback, std::ios_base::event_callback)

REFLECTABLE_DECLARATION(std_ios_base_event_callback)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_DECLARATION(std::ios_base)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::ios_base)
    FUNCTION(flags, std_ios_base_fmtflags() const)
    FUNCTION(flags, std_ios_base_fmtflags(std_ios_base_fmtflags))
    FUNCTION(setf, std_ios_base_fmtflags(std_ios_base_fmtflags))
    FUNCTION(setf, std_ios_base_fmtflags(std_ios_base_fmtflags, std_ios_base_fmtflags))
    FUNCTION(unsetf, void(std_ios_base_fmtflags))
    FUNCTION(precision, typename R::streamsize() const)
    FUNCTION(precision, typename R::streamsize(typename R::streamsize))
    FUNCTION(width, typename R::streamsize() const)
    FUNCTION(width, typename R::streamsize(typename R::streamsize))
REFLECTABLE_INIT()

#endif // REW_BUILTIN_IOSTREAM_HPP
