cmake_minimum_required(VERSION 3.5)

project(Rew LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(REW
    include/Rew/Attribute.hpp
    include/Rew/Type.hpp
    include/Rew/Parent.hpp
    include/Rew/Function.hpp
    include/Rew/Factory.hpp
    include/Rew/Property.hpp
    include/Rew/Meta.hpp
    include/Rew/Reflection.hpp
    include/Rew/Registry.hpp
    include/Rew/Injection.hpp
    include/Rew/Reflectable.hpp
    include/Rew/Utility.hpp
    include/Rew/Common.hpp

    include/Rew/Detail/Meta.hpp
    include/Rew/Detail/Macro.hpp

    include/Rew/BuiltIn/function.hpp
    include/Rew/BuiltIn/allocator.hpp

    include/Rew/BuiltIn/vector.hpp
    include/Rew/BuiltIn/array.hpp
    include/Rew/BuiltIn/string.hpp
    include/Rew/BuiltIn/pair.hpp
    include/Rew/BuiltIn/tuple.hpp
    include/Rew/BuiltIn/list.hpp
    include/Rew/BuiltIn/forward_list.hpp
    include/Rew/BuiltIn/set.hpp
    include/Rew/BuiltIn/unordered_set.hpp
    include/Rew/BuiltIn/unique_ptr.hpp
    include/Rew/BuiltIn/shared_ptr.hpp
    include/Rew/BuiltIn/weak_ptr.hpp
    include/Rew/BuiltIn/map.hpp
    include/Rew/BuiltIn/unordered_map.hpp
    include/Rew/BuiltIn/deque.hpp
    include/Rew/BuiltIn/stack.hpp
    include/Rew/BuiltIn/queue.hpp
    include/Rew/BuiltIn/bitset.hpp
    include/Rew/BuiltIn/complex.hpp
    include/Rew/BuiltIn/optional.hpp
    include/Rew/BuiltIn/variant.hpp
    include/Rew/BuiltIn/any.hpp

    include/Rew/BuiltIn/initializer_list.hpp
    include/Rew/BuiltIn/reference_wrapper.hpp
    include/Rew/BuiltIn/iterator.hpp
    include/Rew/BuiltIn/functional.hpp

    include/Rew/BuiltIn/typeinfo.hpp
    include/Rew/BuiltIn/typeindex.hpp

    include/Rew/BuiltIn/compare.hpp
    include/Rew/BuiltIn/char_traits.hpp

    include/Rew/Core.hpp
)

set(AUTO
    include/AutoTesting/Core.hpp
    include/AutoTesting/Core.cpp
)

include_directories(test)

add_executable(${PROJECT_NAME}
    ${REW}
    ${AUTO}

    test/RewTestingBase.hpp
    test/main.cpp

    test/TestReflectionImpl.cpp
    test/TestReflectionImpl.hpp

    test/TestCommon.cpp
    test/TestEnum.cpp
    test/TestNameOf.cpp
    test/TestRegistryFunction.cpp
    test/TestRegistryProperty.cpp
    test/TestFactory.cpp
    test/TestInjection.cpp
    test/TestUsing.cpp
    test/TestLibrary.cpp
    test/TestWithoutMacro.cpp

    test/TestAllocator.cpp
    test/TestAny.cpp
    test/TestArray.cpp
    test/TestBitset.cpp
    test/TestComplex.cpp
    test/TestDeque.cpp
    test/TestForwardList.cpp
    test/TestFunction.cpp
    test/TestFunctional.cpp
    test/TestInitializerList.cpp
    test/TestList.cpp
    test/TestMap.cpp
    test/TestOptional.cpp
    test/TestPair.cpp
    test/TestQueue.cpp
    test/TestReferenceWrapper.cpp
    test/TestSet.cpp
    test/TestSharedPtr.cpp
    test/TestStack.cpp
    test/TestString.cpp
    test/TestTuple.cpp
    test/TestTypeIndex.cpp
    test/TestTypeInfo.cpp
    test/TestUniquePtr.cpp
    test/TestUnorderedMap.cpp
    test/TestUnorderedSet.cpp
    test/TestVariant.cpp
    test/TestVector.cpp
    test/TestWeakPtr.cpp
)
