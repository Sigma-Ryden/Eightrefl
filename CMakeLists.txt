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
	include/Rew/Visitor.hpp
	include/Rew/Reflectable.hpp
	include/Rew/Utility.hpp

	include/Rew/Detail/Meta.hpp

#	include/Rew/BuiltIn/Core.hpp
	include/Rew/BuiltIn/allocator.hpp
	include/Rew/BuiltIn/vector.hpp
#	include/Rew/BuiltIn/array.hpp
#	include/Rew/BuiltIn/string.hpp
	include/Rew/BuiltIn/initializer_list.hpp
#	include/Rew/BuiltIn/pair.hpp
#	include/Rew/BuiltIn/tuple.hpp
#	include/Rew/BuiltIn/list.hpp
#	include/Rew/BuiltIn/forward_list.hpp
#	include/Rew/BuiltIn/set.hpp
#	include/Rew/BuiltIn/unique_ptr.hpp
#	include/Rew/BuiltIn/shared_ptr.hpp
#	include/Rew/BuiltIn/weak_ptr.hpp
#	include/Rew/BuiltIn/map.hpp
#	include/Rew/BuiltIn/deque.hpp
#	include/Rew/BuiltIn/stack.hpp
#	include/Rew/BuiltIn/queue.hpp
#	include/Rew/BuiltIn/valarray.hpp
#	include/Rew/BuiltIn/bitset.hpp
#	include/Rew/BuiltIn/atomic.hpp
#	include/Rew/BuiltIn/complex.hpp
#	include/Rew/BuiltIn/optional.hpp
#	include/Rew/BuiltIn/variant.hpp
#	include/Rew/BuiltIn/any.hpp include/Rew/BuiltIn/any.cpp

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

	test/TestBuiltin.cpp
	#test/TestFactory.cpp
	#test/TestExample.cpp

	#test/examples/serialization/SerializationFixture.hpp
	#test/examples/serialization/SerializableVisitor.hpp
	#test/examples/serialization/SerializableVisitor.cpp
	#test/examples/serialization/TestSerialization.cpp

	test/RewTestingBase.hpp

	test/main.cpp
)
