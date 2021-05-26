# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("psws")

if (MSVC)
	MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})
	set(CompilerFlags
		CMAKE_CXX_FLAGS
		CMAKE_CXX_FLAGS_DEBUG
		CMAKE_CXX_FLAGS_RELEASE
		CMAKE_C_FLAGS
		CMAKE_C_FLAGS_DEBUG
		CMAKE_C_FLAGS_RELEASE
		)
	foreach(CompilerFlag ${CompilerFlags})
		string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
	endforeach()
	add_compile_options("/IGNORE:4200" "/bigobj")
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
	add_link_options("/IGNORE:4006,4075,4088,4099")
else(MSVC)
endif(MSVC)

# Include sub-projects.
add_subdirectory ("vendor/oatpp-master")
add_subdirectory ("vendor/oatpp-websocket-master")
add_subdirectory ("psws")
