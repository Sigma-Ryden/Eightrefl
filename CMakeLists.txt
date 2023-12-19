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
	include/Rew/Cast.hpp
	include/Rew/Registry.hpp
	include/Rew/Visitor.hpp
	include/Rew/Reflectable.hpp
	include/Rew/Utility.hpp
	include/Rew/Core.hpp
)

set(AUTO
	include/AutoTesting/Core.hpp
	include/AutoTesting/Core.cpp
)

add_executable(${PROJECT_NAME}
	${REW}
	${AUTO}

	test/TestFactory.cpp

	test/RewTestingBase.hpp
	test/main.cpp
)
