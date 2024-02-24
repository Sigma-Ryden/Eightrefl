#ifndef REW_BUILTIN_ITERATOR_HPP
#define REW_BUILTIN_ITERATOR_HPP

#include <Rew/Reflectable.hpp>

TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_const_iterator,
    (std_const_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::const_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_const_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::const_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_const_iterator<StdContainerType>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_iterator,
    (std_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_iterator<StdContainerType>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_const_reverse_iterator,
    (std_const_reverse_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::const_reverse_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_const_reverse_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::const_reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_const_reverse_iterator<StdContainerType>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_reverse_iterator,
    (std_reverse_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::reverse_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_reverse_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::reverse_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_reverse_iterator<StdContainerType>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_local_iterator,
    (std_local_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::local_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_local_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::local_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_local_iterator<StdContainerType>)
REFLECTABLE_INIT()


TEMPLATE_REFLECTABLE_USING
(
    (template <class StdContainerType>), std_const_local_iterator,
    (std_const_local_iterator<StdContainerType>), typename CLEANOF(StdContainerType)::const_local_iterator
)

TEMPLATE_REFLECTABLE_DECLARATION((template <class StdContainerType>), std_const_local_iterator<StdContainerType>)
    BUILTIN_REFLECTABLE()
    REFLECTABLE_NAME(NAMEOF(StdContainerType) + "::const_local_iterator")
REFLECTABLE_DECLARATION_INIT()

TEMPLATE_REFLECTABLE((template <class StdContainerType>), std_const_local_iterator<StdContainerType>)
REFLECTABLE_INIT()

#endif // REW_BUILTIN_ITERATOR_HPP
