#ifndef REW_TYPE_HPP
#define REW_TYPE_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <functional> // function

#include <Rew/Attribute.hpp>
#include <Rew/Injection.hpp>

namespace rew
{

struct reflection_t;

struct type_t
{
    std::string const name;
    reflection_t *const reflection = nullptr;
    std::size_t const size = 0;
    std::function<std::any(std::any& object)> const context = nullptr;

    attribute_t<injection_t> injection;
};

} // namespace rew

// quick type access, use after all registrations only
template <typename ReflectableType>
inline ::rew::type_t* xxrew_type = nullptr;

#endif // REW_TYPE_HPP
