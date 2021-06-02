set(
	TLS_SRC
	tls.c
	tls_bio_cb.c
	tls_client.c
	tls_config.c
	tls_conninfo.c
	tls_keypair.c
	tls_server.c
	tls_ocsp.c
	tls_peer.c
	tls_util.c
	tls_verify.c
)

if(WIN32)
	set(
		TLS_SRC
		${TLS_SRC}
		compat/ftruncate.c
		compat/pread.c
		compat/pwrite.c
	)
endif()

if(NOT "${OPENSSLDIR}" STREQUAL "")
	add_definitions(-DTLS_DEFAULT_CA_FILE=\"${OPENSSLDIR}/cert.pem\")
else()
	add_definitions(-DTLS_DEFAULT_CA_FILE=\"${CMAKE_INSTALL_PREFIX}/etc/ssl/cert.pem\")
endif()

add_library(re_tls_static STATIC ${TLS_SRC})

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
	
	target_compile_definitions(re_tls_static
							PRIVATE ${project_name_defines}
							)
	target_compile_options(re_tls_static 
							PRIVATE ${project_name_cflags}
	)
endif(MSVC)

target_include_directories(re_tls_static
	PRIVATE
		.
		../include/compat
	PUBLIC
		../include)

#export_symbol(re_tls_static ${CMAKE_CURRENT_SOURCE_DIR}/tls.sym)
target_link_libraries(re_tls_static re_ssl_static re_crypto_static ${PLATFORM_LIBS})
#if (WIN32)
#	set(TLS_POSTFIX -${TLS_MAJOR_VERSION})
#endif()
#set_target_properties(re_tls_static PROPERTIES
#	OUTPUT_NAME re_tls_static{TLS_POSTFIX}
#	ARCHIVE_OUTPUT_NAME re_tls_static{TLS_POSTFIX})
#set_target_properties(re_tls_static PROPERTIES VERSION ${TLS_VERSION}
#	SOVERSION ${TLS_MAJOR_VERSION})

#if(ENABLE_LIBRESSL_INSTALL)
#	install(
#		TARGETS re_tls_static
#		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#	)
#endif(ENABLE_LIBRESSL_INSTALL)
