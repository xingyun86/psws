###################################################################################################
## These variables are passed to oatpp-module-install.cmake script
## use these variables to configure module installation

set(OATPP_THIS_MODULE_NAME oatpp-libressl) ## name of the module (also name of folders in installation dirs)
set(OATPP_THIS_MODULE_VERSION "1.0.0") ## version of the module (also sufix of folders in installation dirs)
set(OATPP_THIS_MODULE_LIBRARIES oatpp-libressl) ## list of libraries to find when find_package is called
set(OATPP_THIS_MODULE_TARGETS oatpp-libressl) ## list of targets to install
set(OATPP_THIS_MODULE_DIRECTORIES oatpp-libressl) ## list of directories to install

set(OATPP_LIBRESSL_DEBUG "0" CACHE STRING "The debug-level of libressl")

# 项目信息
set(project_name oatpp-libressl) ## rename your project here

project(${project_name})

##############################
## Find MbedTLS dependency

include_directories(
	${CMAKE_SOURCE_DIR} 
	${CMAKE_SOURCE_DIR}/vendor/oatpp 
	${CMAKE_SOURCE_DIR}/vendor/libressl-3.3.3/include
)
set(OPENSSL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/vendor/libressl-3.3.3/include)
# 把第三方库的.a文件和.so文件放到代码的lib文件夹
link_directories(
	${CMAKE_BINARY_DIR}/oatpp 
	${CMAKE_BINARY_DIR}/vendor/libressl-3.3.3/ssl 
	${CMAKE_BINARY_DIR}/vendor/libressl-3.3.3/tls 
	${CMAKE_BINARY_DIR}/vendor/libressl-3.3.3/crypto
)

if (MSVC)
	list(APPEND project_name_cflags /W4)
	list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
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
	
endif(MSVC)

add_library(${OATPP_THIS_MODULE_NAME} STATIC
        oatpp-libressl/Callbacks.cpp
        oatpp-libressl/Callbacks.hpp
        oatpp-libressl/Config.cpp
        oatpp-libressl/Config.hpp
        oatpp-libressl/Connection.cpp
        oatpp-libressl/Connection.hpp
        oatpp-libressl/client/ConnectionProvider.cpp
        oatpp-libressl/client/ConnectionProvider.hpp
        oatpp-libressl/server/ConnectionProvider.cpp
        oatpp-libressl/server/ConnectionProvider.hpp
        oatpp-libressl/TLSObject.cpp
        oatpp-libressl/TLSObject.hpp
)

set_target_properties(${OATPP_THIS_MODULE_NAME} PROPERTIES
        CXX_STANDARD 11
        CXX_EXTENSIONS OFF
        CXX_STANDARD_REQUIRED ON
)

if(OATPP_MODULES_LOCATION STREQUAL OATPP_MODULES_LOCATION_EXTERNAL)
    add_dependencies(${OATPP_THIS_MODULE_NAME} ${LIB_OATPP_EXTERNAL})
endif()

if (MSVC)
	list(APPEND project_name_cflags /W4)
	list(APPEND project_name_defines WIN32_LEAN_AND_MEAN _WIN32_WINNT=0x0501)
	MESSAGE(STATUS "CMAKE_BUILD_TYPE=" ${CMAKE_BUILD_TYPE})

	target_compile_definitions(${OATPP_THIS_MODULE_NAME}
							PRIVATE ${project_name_defines}
							)
	target_compile_options(${OATPP_THIS_MODULE_NAME} 
							PRIVATE ${project_name_cflags}
	)
endif(MSVC)

target_link_libraries(${OATPP_THIS_MODULE_NAME})

target_include_directories(${OATPP_THIS_MODULE_NAME}
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_link_libraries(${OATPP_THIS_MODULE_NAME}
        PUBLIC oatpp
        PUBLIC oatpp-test
        PUBLIC re_tls_static
        PUBLIC re_ssl_static
        PUBLIC re_crypto_static
)

#######################################################################################################
## install targets

#if(OATPP_INSTALL)
#    include("../cmake/module-install.cmake")
#endif()
