#ifndef REW_BUILTIN_IOSTREAM_HPP
#define REW_BUILTIN_IOSTREAM_HPP

#include <Rew/Reflectable.hpp>

#include <Rew/BuiltIn/locale.hpp>

#include <ios> // ios_base

REFLECTABLE_USING(std_streamsize, std::streamsize)

CUSTOM_REFLECTABLE_DECLARATION(std_streamsize)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::streamsize")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_openmode, std::ios_base::openmode)

CUSTOM_REFLECTABLE_DECLARATION(std_ios_base_openmode)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ios_base::openmode")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_USING(std_ios_base_fmtflags, std::ios_base::fmtflags)

CUSTOM_REFLECTABLE_DECLARATION(std_ios_base_fmtflags)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ios_base::fmtflags")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_USING(std_ios_base_iostate, std::ios_base::iostate)

CUSTOM_REFLECTABLE_DECLARATION(std_ios_base_iostate)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ios_base::iostate")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE_USING(std_ios_base_seekdir, std::ios_base::seekdir)

CUSTOM_REFLECTABLE_DECLARATION(std_ios_base_seekdir)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ios_base::seekdir")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_DECLARATION(std::ios_base::event)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_ios_base_event_callback, std::ios_base::event_callback)

CUSTOM_REFLECTABLE_DECLARATION(std_ios_base_event_callback)
    REFLECTABLE_REGISTRY(&rew::global)
    REFLECTABLE_NAME("std::ios_base::callback")
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_DECLARATION(std::ios_base::failure)
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
    FUNCTION(precision, std::streamsize() const)
    FUNCTION(precision, std::streamsize(std::streamsize))
    FUNCTION(width, std::streamsize() const)
    FUNCTION(width, std::streamsize(std::streamsize))
    FUNCTION(imbue)
    FUNCTION(getloc)
    FUNCTION(xalloc)
    FUNCTION(iword)
    FUNCTION(pword)
    FUNCTION(register_callback, void(std_ios_base_event_callback, int))
    FUNCTION(sync_with_stdio, bool(bool))
    PROPERTY(app, std_ios_base_openmode const)
    PROPERTY(binary, std_ios_base_openmode const)
    PROPERTY(in, std_ios_base_openmode const)
    PROPERTY(out, std_ios_base_openmode const)
    PROPERTY(trunc, std_ios_base_openmode const)
    PROPERTY(ate, std_ios_base_openmode const)
    PROPERTY(dec, std_ios_base_fmtflags const)
    PROPERTY(oct, std_ios_base_fmtflags const)
    PROPERTY(hex, std_ios_base_fmtflags const)
    PROPERTY(basefield, std_ios_base_fmtflags const)
    PROPERTY(left, std_ios_base_fmtflags const)
    PROPERTY(right, std_ios_base_fmtflags const)
    PROPERTY(internal, std_ios_base_fmtflags const)
    PROPERTY(adjustfield, std_ios_base_fmtflags const)
    PROPERTY(scientific, std_ios_base_fmtflags const)
    PROPERTY(fixed, std_ios_base_fmtflags const)
    PROPERTY(floatfield, std_ios_base_fmtflags const)
    PROPERTY(boolalpha, std_ios_base_fmtflags const)
    PROPERTY(showbase, std_ios_base_fmtflags const)
    PROPERTY(showpoint, std_ios_base_fmtflags const)
    PROPERTY(showpos, std_ios_base_fmtflags const)
    PROPERTY(skipws, std_ios_base_fmtflags const)
    PROPERTY(unitbuf, std_ios_base_fmtflags const)
    PROPERTY(uppercase, std_ios_base_fmtflags const)
    PROPERTY(goodbit, std_ios_base_iostate const)
    PROPERTY(badbit, std_ios_base_iostate const)
    PROPERTY(failbit, std_ios_base_iostate const)
    PROPERTY(eofbit, std_ios_base_iostate const)
    PROPERTY(beg, std_ios_base_seekdir const)
    PROPERTY(end, std_ios_base_seekdir const)
    PROPERTY(cur, std_ios_base_seekdir const)
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
