cmake_minimum_required(VERSION 3.5)
project(auth-web LANGUAGES CXX VERSION 1.2.3 HOMEPAGE_URL https://github.com/passy1977/auth-web)

#lanuguiade standard definition
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#macro area
add_compile_definitions(NO_CHECK_PID)

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
#include_directories(${CMAKE_SOURCE_DIR}/src/3thparty/jwt-cpp/include)
#file(GLOB jwt-cpp_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/src/3thparty/jwt-cpp/include/jwt-cpp/*)
#file(GLOB nlohmann_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/src/3thparty/jwt-cpp/include/nlohmann/*)
#file(GLOB picojson_PUBLIC_HEADERS ${CMAKE_SOURCE_DIR}/src/3thparty/jwt-cpp/include/picojson/*)

#source definitions
add_executable(
    auth-web
    ${mariadbclientpp_SOURCES}
    ${mariadbclientpp_PUBLIC_HEADERS}
    ${mariadbclientpp_PRIVATE_HEADERS}
#    ${jwt-cpp_PUBLIC_HEADERS}
#    ${nlohmann_PUBLIC_HEADERS}
#    ${picojson_PUBLIC_HEADERS}
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
    src/jwthelper.h
    src/jwthelper.cpp
    src/daos/dao.h
    src/daos/userdao.h
    src/daos/userdao.cpp
    src/daos/domaindao.h
    src/daos/domaindao.cpp
    src/pods/user.h
    src/pods/user.cpp
    src/pods/domain.h
    src/pods/domain.cpp
    src/helpers/domainhelper.h
    src/helpers/domainhelper.cpp
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
target_link_libraries(auth-web PocoCrypto PocoEncodings PocoFoundation PocoJson PocoJWT PocoNet PocoUtil mariadb ssl crypto)
