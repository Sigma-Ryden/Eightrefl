#ifndef REW_ATTRIBUTE_HPP
#define REW_ATTRIBUTE_HPP

#include <string> // string
#include <unordered_map> // unordered_map

namespace rew
{

template <class MetaType>
struct attribute_t
{
    MetaType* find(std::string const& name)
    {
        auto it = all.find(name);
        return it != all.end() ? &it->second : nullptr;
    }

    MetaType& add(std::string const& name, MetaType const& meta)
    {
        return all.emplace(name, meta).first->second;
    }

    bool remove(std::string const& name)
    {
        return all.erase(name)>0;
    }

    std::unordered_map<std::string, MetaType> all;
};

} // namespace rew

#endif // REW_ATTRIBUTE_HPP
