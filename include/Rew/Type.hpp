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
    const std::string name;
    reflection_t *const reflection = nullptr;
    const std::size_t size = 0;
    const std::function<std::any(std::any& object)> context = nullptr;

    attribute_t<injection_t> injection;
};

} // namespace rew

#endif // REW_TYPE_HPP
