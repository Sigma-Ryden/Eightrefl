#ifndef REW_BUILTIN_LOCALE_HPP
#define REW_BUILTIN_LOCALE_HPP

#include <Rew/Reflectable.hpp>

#include <Rew/BuiltIn/string.hpp>

REFLECTABLE_USING(std_locale_id, std::locale::id)

REFLECTABLE_DECLARATION(std_locale_id)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_locale_facet, std::locale::facet)

REFLECTABLE_DECLARATION(std_locale_facet)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_USING(std_locale_category, std::locale::category)

REFLECTABLE_DECLARATION(std_locale_category)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()


REFLECTABLE_DECLARATION(std::locale)
    BUILTIN_REFLECTABLE()
REFLECTABLE_DECLARATION_INIT()

REFLECTABLE(std::locale)
    FACTORY(R())
    FACTORY(R(R const&))
    FACTORY(R(char const*))
    FACTORY(R(std::string const&))
    FACTORY(R(R const&, char const*, std_locale_category))
    FACTORY(R(R const&, std::string const&, std_locale_category))
    FACTORY(R(R const&, R const&, std_locale_category))
    FUNCTION(operator=, R const&(R const&))
    FUNCTION(name)

#if __cplusplus <= 201703L
    FUNCTION(operator==)
    FUNCTION(operator!=)
#endif // if

    FUNCTION(operator(), bool(std::string const&, std::string const&) const)
    FUNCTION(operator(), bool(std::wstring const&, std::wstring const&) const)
    FUNCTION(global)
    FUNCTION(classic)
    PROPERTY(none)
    PROPERTY(collate)
    PROPERTY(ctype)
    PROPERTY(monetary)
    PROPERTY(numeric)
    PROPERTY(time)
    PROPERTY(messages)
    PROPERTY(all)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_LOCALE_HPP
