cmake_minimum_required(VERSION 3.5)
project(auth-web LANGUAGES CXX VERSION 1.2.3 HOMEPAGE_URL https://github.com/passy1977/auth-web)

#lanuguiade standard definition
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#macro area
#add_compile_definitions(POCO_LOG_DEBUG)

#add mariadbpp cmake extension path
#list(APPEND CMAKE_PREFIX_PATH ${CMAKE_SOURCE_DIR}/3thparty/mariadbpp/cmake)

#lib include
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/mariadb/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/mariadb/lib)

# add mariadbclientpp 3thparty sources
include_directories(${CMAKE_SOURCE_DIR}/src/3thparty/mariadbpp/include)
file(GLOB mariadbclientpp_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/src/3thparty/mariadbpp/include/mariadb++/*)
file(GLOB mariadbclientpp_PRIVATE_HEADERS ${CMAKE_SOURCE_DIR}/src/3thparty/mariadbpp/src/*.hpp)
file(GLOB mariadbclientpp_SOURCES ${CMAKE_SOURCE_DIR}/src/3thparty/mariadbpp/src/*.cpp)

#source definitions
add_executable(
    auth-web
    ${mariadbclientpp_SOURCES}
    ${mariadbclientpp_PUBLIC_HEADERS}
    ${mariadbclientpp_PRIVATE_HEADERS}
    src/main.cpp
    src/constants.h
    src/application.h
    src/application.cpp
    src/globals.h
    src/globals.cpp
    src/router.h
    src/router.cpp
    src/utils.h
    src/utils.cpp
    src/daos/dao.h
    src/daos/userdao.h
    src/daos/userdao.cpp
    src/daos/domaindao.h
    src/daos/domaindao.cpp
    src/pods/user.h
    src/pods/domain.h
    src/crypto/aes.h
    src/crypto/aes.cpp
    src/crypto/bytearray.h
    src/crypto/bytearray.cpp
    src/services/lockservice.h
    src/services/lockservice.cpp
    src/services/logservice.h
    src/services/logservice.cpp
    src/services/authservice.h
    src/services/authservice.cpp
    src/services/domainservice.h
    src/services/domainservice.cpp
    src/services/userservice.h
    src/services/userservice.cpp
    src/controllers/controller.h
    src/controllers/authcontroller.h
    src/controllers/authcontroller.cpp
    src/controllers/httpstatuscontroller.h
    src/controllers/httpstatuscontroller.cpp
    src/controllers/usercontroller.h
    src/controllers/usercontroller.cpp
    src/controllers/domaincontroller.h
    src/controllers/domaincontroller.cpp
)

#linking
target_link_libraries(auth-web PocoJWT PocoUtil PocoNet PocoJson PocoFoundation PocoData PocoCrypto mariadb ssl crypto)
