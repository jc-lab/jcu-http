cmake_minimum_required (VERSION 2.8)
project(jcu-http CXX)

set(SRC_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/client.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/request.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/response.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/request-prepare.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/http-get.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/http-post.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/ssl-cert.h
		${CMAKE_CURRENT_SOURCE_DIR}/include/jcu-http/default-cacerts.h
        ${CMAKE_CURRENT_SOURCE_DIR}/src/client.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/request-prepare.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/request.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/response.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/http-get.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/http-post.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/ssl-cert.cc
		${CMAKE_CURRENT_SOURCE_DIR}/src/default-cacerts.cc
)
add_library(${PROJECT_NAME} ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

find_package(CURL REQUIRED)
target_link_libraries(${PROJECT_NAME} libcurl)

find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto OpenSSL::SSL)
