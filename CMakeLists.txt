cmake_minimum_required(VERSION 3.5)
project(auth-web LANGUAGES CXX VERSION 1.2.3 HOMEPAGE_URL https://github.com/passy1977/auth-web)

#lanuguiade standard definition
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#macro area
#add_compile_definitions(POCO_LOG_DEBUG)


#lib include
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/mysql-connector/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/mysql-connector/lib)

#source definitions
add_executable(
    auth-web
    src/main.cpp
    src/constants.h
    src/application.h
    src/application.cpp
    src/globals.h
    src/globals.cpp
    src/router.h
    src/router.cpp
    src/daos/dao.h
    src/daos/userdao.h
    src/daos/userdao.cpp
    src/daos/domaindao.h
    src/daos/domaindao.cpp
    src/pods/pod.h
    src/pods/user.h
    src/pods/user.cpp
    src/pods/domain.h
    src/pods/domain.cpp
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
target_link_libraries(auth-web PocoJWT PocoUtil PocoNet PocoJson PocoFoundation PocoData PocoCrypto mysqlcppconn8 ssl crypto)
