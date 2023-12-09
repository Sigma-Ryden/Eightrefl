#ifndef REW_META_HPP
#define REW_META_HPP

#include <string> // string
#include <map> // map
#include <any> // any

#include <Rew/Attribute.hpp>

#define META(name, ...)                                                                                 \
    visitor.meta(name, __VA_ARGS__);

namespace rew
{

// TODO:
// improve meta class for better registration info
struct meta_t : attribute_t<meta_t>
{
    std::map<std::string, std::any> all;
};

} // namespace rew

#endif // REW_META_HPP
