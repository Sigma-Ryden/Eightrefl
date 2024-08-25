#ifndef REW_META_HPP
#define REW_META_HPP

// .meta(external_name, expression)
#define META(external_name, ...)                                                                        \
    {                                                                                                   \
        auto xxiometa = xxmeta->find(external_name);                                                    \
        if (xxiometa == nullptr) xxiometa = &xxmeta->add(external_name, { external_name, __VA_ARGS__ });\
        injection.template meta<CleanR, decltype(__VA_ARGS__)>(*xxiometa);                              \
        xxsubmeta = &xxiometa->meta;                                                                    \
    }

// .submeta(external_name, expression)
#define SUBMETA(external_name, ...)                                                                     \
    {                                                                                                   \
        auto xxiosubmeta = xxsubmeta->find(external_name);                                              \
        if (xxiosubmeta == nullptr) xxiosubmeta = &xxsubmeta->add(external_name, __VA_ARGS__);          \
        injection.template submeta<CleanR, decltype(__VA_ARGS__)>(external_name, *xxiosubmeta);         \
    }

namespace rew
{

struct meta_t
{
    std::string const name;
    std::any value;
    attribute_t<std::any> meta;
};

} // namespace rew

#endif // REW_META_HPP
