cmake_minimum_required(VERSION 3.5)
project(auth-web CXX LANGUAGES VERSION 1.2.3 HOMEPAGE_URL https://github.com/passy1977/auth-web)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/lib)

add_executable(auth-web
  src/main.cpp
  src/constants.h
  src/application.h
  src/application.cpp
  src/globals.h
  src/globals.cpp
  src/router.h
  src/router.cpp
  src/pods/user.h
  src/pods/user.cpp
  src/pods/domain.h
  src/pods/domain.cpp
  src/controllers/authcontroller.h
  src/controllers/authcontroller.cpp
  src/controllers/httpstatuscontroller.h
  src/controllers/httpstatuscontroller.cpp
)

target_link_libraries(auth-web PocoJWT PocoUtil PocoNet PocoJson PocoFoundation PocoData PocoCrypto PocoMongoDB ssl crypto)
