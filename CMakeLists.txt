cmake_minimum_required (VERSION 2.8)
project(jcu-http CXX)

set(LIB_NAME jcu_http)

option(JCU_HTTP_BUILD_SHARED_LIBS "jcu-http Build shared libraries" ON)

set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/client.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/request.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/response.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/request-prepare.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/http-get.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/http-post.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/http-entity.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/ssl-cert.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/default-cacerts.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/callback-request-entity.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/string-entity.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/request-prepare.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/request.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/response.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/http-get.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/http-post.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/ssl-cert.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/default-cacerts.cc
)

if(JCU_HTTP_BUILD_SHARED_LIBS)
	add_library(${LIB_NAME} SHARED ${SRC_FILES})
else()
	add_library(${LIB_NAME} STATIC ${SRC_FILES})
endif()

if(NOT JCU_HTTP_BUILD_SHARED_LIBS)
	set_target_properties(${LIB_NAME} PROPERTIES INTERFACE_COMPILE_DEFINITIONS JCU_HTTP_STATICLIB)
endif()

target_include_directories(${LIB_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

find_package(CURL REQUIRED)
if (CURL_LIBRARIES)
	target_link_libraries(${LIB_NAME} ${CURL_LIBRARIES})
else()
	target_link_libraries(${LIB_NAME} CURL::libcurl)
endif()

find_package(OpenSSL REQUIRED)
if (OPENSSL_LIBRARIES)
	target_link_libraries(${LIB_NAME} ${OPENSSL_LIBRARIES})
else()
	target_link_libraries(${LIB_NAME} OpenSSL::Crypto OpenSSL::SSL)
endif()
