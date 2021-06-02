set(
	SSL_SRC
	bio_ssl.c
	bs_ber.c
	bs_cbb.c
	bs_cbs.c
	d1_both.c
	d1_lib.c
	d1_pkt.c
	d1_srtp.c
	pqueue.c
	s3_cbc.c
	s3_lib.c
	ssl_algs.c
	ssl_asn1.c
	ssl_both.c
	ssl_cert.c
	ssl_ciph.c
	ssl_ciphers.c
	ssl_clnt.c
	ssl_err.c
	ssl_init.c
	ssl_kex.c
	ssl_lib.c
	ssl_methods.c
	ssl_packet.c
	ssl_pkt.c
	ssl_rsa.c
	ssl_sess.c
	ssl_sigalgs.c
	ssl_srvr.c
	ssl_stat.c
	ssl_tlsext.c
	ssl_transcript.c
	ssl_txt.c
	ssl_versions.c
	t1_enc.c
	t1_lib.c
	tls12_record_layer.c
	tls13_buffer.c
	tls13_client.c
	tls13_error.c
	tls13_handshake.c
	tls13_handshake_msg.c
	tls13_key_schedule.c
	tls13_key_share.c
	tls13_legacy.c
	tls13_lib.c
	tls13_record.c
	tls13_record_layer.c
	tls13_server.c
)

add_library(re_ssl_static STATIC ${SSL_SRC})

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
	
	target_compile_definitions(re_ssl_static
							PRIVATE ${project_name_defines}
							)
	target_compile_options(re_ssl_static 
							PRIVATE ${project_name_cflags}
	)
endif(MSVC)

target_include_directories(re_ssl_static
	PRIVATE
		.
		../include/compat
	PUBLIC
		../include)

#export_symbol(re_ssl_static ${CMAKE_CURRENT_SOURCE_DIR}/ssl.sym)
target_link_libraries(re_ssl_static re_crypto_static ${PLATFORM_LIBS})
#if (WIN32)
#	set(SSL_POSTFIX -${SSL_MAJOR_VERSION})
#endif()
#set_target_properties(re_ssl_static PROPERTIES
#	OUTPUT_NAME re_ssl_static{SSL_POSTFIX}
#	ARCHIVE_OUTPUT_NAME ssl${SSL_POSTFIX})
#set_target_properties(re_ssl_static PROPERTIES VERSION ${SSL_VERSION}
#	SOVERSION ${SSL_MAJOR_VERSION})

#if(ENABLE_LIBRESSL_INSTALL)
#	install(
#		TARGETS re_ssl_static
#		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
#		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
#	)
#endif(ENABLE_LIBRESSL_INSTALL)
