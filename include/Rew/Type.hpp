#ifndef REW_TYPE_HPP
#define REW_TYPE_HPP

#include <cstddef> // size_t

#include <string> // string
#include <any> // any

#include <functional> // function

namespace rew
{

struct reflection_t;
struct visitor_t;

struct type_t
{
    const std::string name;
    reflection_t *const reflection = nullptr;
    const std::function<void(visitor_t& visitor)> evaluate = nullptr;
    const std::function<std::any(std::any& object)> context = nullptr;
    const std::function<std::any(std::any& context)> ref = nullptr;
    const std::size_t size = 0;
};

} // namespace rew

#endif // REW_TYPE_HPP
