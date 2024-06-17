#ifndef REW_BUILTIN_IOSTREAM_HPP
#define REW_BUILTIN_IOSTREAM_HPP

#include <Rew/Reflectable.hpp>

#include <Rew/BuiltIn/locale.hpp>

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


REFLECTABLE_USING(std_ios_base_event_failure, std::ios_base::failure)

REFLECTABLE_DECLARATION(std_ios_base_event_failure)
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
    FUNCTION(imbue)
    FUNCTION(getloc)
    FUNCTION(xalloc)
    FUNCTION(iword)
    FUNCTION(pword)
    FUNCTION(register_callback, void(std_ios_base_event_callback, int))
    FUNCTION(sync_with_stdio, bool(bool))
    PROPERTY(app, std_ios_base_openmode)
    PROPERTY(binary, std_ios_base_openmode)
    PROPERTY(in, std_ios_base_openmode)
    PROPERTY(out, std_ios_base_openmode)
    PROPERTY(trunc, std_ios_base_openmode)
    PROPERTY(ate, std_ios_base_openmode)
    PROPERTY(dec, std_ios_base_fmtflags)
    PROPERTY(oct, std_ios_base_fmtflags)
    PROPERTY(hex, std_ios_base_fmtflags)
    PROPERTY(basefield, std_ios_base_fmtflags)
    PROPERTY(left, std_ios_base_fmtflags)
    PROPERTY(right, std_ios_base_fmtflags)
    PROPERTY(internal, std_ios_base_fmtflags)
    PROPERTY(adjustfield, std_ios_base_fmtflags)
    PROPERTY(scientific, std_ios_base_fmtflags)
    PROPERTY(fixed, std_ios_base_fmtflags)
    PROPERTY(floatfield, std_ios_base_fmtflags)
    PROPERTY(boolalpha, std_ios_base_fmtflags)
    PROPERTY(showbase, std_ios_base_fmtflags)
    PROPERTY(showpoint, std_ios_base_fmtflags)
    PROPERTY(showpos, std_ios_base_fmtflags)
    PROPERTY(skipws, std_ios_base_fmtflags)
    PROPERTY(unitbuf, std_ios_base_fmtflags)
    PROPERTY(uppercase, std_ios_base_fmtflags)
    PROPERTY(goodbit, std_ios_base_iosstate)
    PROPERTY(badbit, std_ios_base_iosstate)
    PROPERTY(failbit, std_ios_base_iosstate)
    PROPERTY(eofbit, std_ios_base_iosstate)
    PROPERTY(beg, std_ios_base_seekdir)
    PROPERTY(end, std_ios_base_seekdir)
    PROPERTY(cur, std_ios_base_seekdir)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_CLEAN
(
    (template <typename CharT, typename Traits>),
    (std::basic_ios<CharT, Traits>), std::basic_ios<rew::cleanof<CharT>, rew::cleanof<Traits>>
)

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::ios)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::ios")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION(template <>, std::wios)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::wios")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE_DECLARATION((template <typename CharT, typename Traits>), std::basic_ios<CharT, Traits>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME("std::basic_ios<" + rew::nameof<CharT>() + ", " + rew::nameof<Traits>() + ">")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <typename CharT, typename Traits>), std::basic_ios<CharT, Traits>)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_IOSTREAM_HPP
