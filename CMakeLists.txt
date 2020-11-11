cmake_minimum_required(VERSION 3.5)
project(auth-web CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


include_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/poco/lib)
include_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/include)
link_directories(${CMAKE_SOURCE_DIR}/lib/macos/openssl/lib)


add_executable(auth-web
  src/main.cc
  src/router.h
  src/router.cc
  src/pods/user.h
  src/pods/user.cc
  src/pods/domain.h
  src/pods/domain.cc
  src/controllers/authcontroller.h
  src/controllers/authcontroller.cc
  src/controllers/errorcontroller.h
  src/controllers/errorcontroller.cc
)

target_link_libraries(auth-web PocoJWT PocoUtil PocoNet PocoJson PocoFoundation PocoData PocoCrypto ssl crypto)
