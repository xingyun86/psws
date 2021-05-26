# CMakeList.txt : CMake project for psws, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)


set(project_name "psws")
project(${project_name})

if (CMAKE_HOST_WIN32)
	message(STATUS "Platform=Windows")
	list(APPEND PLATFORM_DEFINE -DTARGET_HOST_MS_WINDOWS=1)
elseif (CMAKE_HOST_UNIX)
	message(STATUS "Platform=Linux")
	list(APPEND PLATFORM_DEFINE -DTARGET_HOST_POSIX_X11=1)
elseif (CMAKE_HOST_APPLE)
	message(STATUS "Platform=Macos")
endif ()

if(MSVC)
	if (CMAKE_BUILD_TYPE STREQUAL Debug)
		set(SUFFIX_T d)
		list(APPEND LINK_LIBS_FILE_PATH ${CMAKE_SOURCE_DIR}/vendor/windows_lib/x64-Debug)
	else()
		set(SUFFIX_T )
		list(APPEND LINK_LIBS_FILE_PATH ${CMAKE_SOURCE_DIR}/vendor/windows_lib/x64-Release)
	endif()
else(MSVC)
	set(SUFFIX_T )
	list(APPEND LINK_LIBS_FILE_PATH ${CMAKE_SOURCE_DIR}/vendor/linux_lib)
endif(MSVC)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_SOURCE_DIR}
	${CMAKE_SOURCE_DIR}/vendor/oatpp-master/src
	${CMAKE_SOURCE_DIR}/vendor/oatpp-websocket-master/src
	${CMAKE_BINARY_DIR}
)

link_directories(
	${CMAKE_CURRENT_BINARY_DIR}
	${LINK_LIBS_FILE_PATH}
	${CMAKE_BINARY_DIR}/vendor/oatpp-master
	${CMAKE_BINARY_DIR}/vendor/oatpp-websocket-master
)

if(MSVC)
list(APPEND EXT_LIB oatpp oatpp-websocket)
else(MSVC)
list(APPEND EXT_LIB oatpp oatpp-websocket)
endif(MSVC)

file(GLOB SRCS "*.cpp")

# Add source to this project's executable.
list(APPEND common_ui_defs )
if(MSVC)
	list(APPEND common_ui_opts "/std:c++17")
else(MSVC)
endif(MSVC)

add_executable (${project_name} ${SRCS})
target_compile_definitions(${project_name} PRIVATE ${common_ui_defs} PRIVATE ${PLATFORM_DEFINE})
target_compile_options(${project_name} PRIVATE ${common_ui_opts})
target_link_libraries(${project_name} PRIVATE ${EXT_LIB})

# TODO: Add tests and install targets if needed.
