#include "SerializableVisitor.hpp"

void saveload_impl(sf::core::IOArchive& archive, rew::Reflectable& reflectable, std::type_index typeindex, const char* mode)
{
    using saveload_function_t = serializable_visitor_t::saveload_function_t;

    auto type = rew::registry.find(typeindex);
    auto reflectable_ptr = std::addressof(reflectable);

    for (auto& [name, parent] : type->reflection->parent.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*parent.meta.find(mode));
        saveload(archive, reflectable_ptr);
    }

    for (auto& [name, property] : type->reflection->property.all)
    {
        auto& saveload = std::any_cast<saveload_function_t&>(*property.meta.find(mode));
        saveload(archive, property.ptr(reflectable_ptr));
    }
}
