cmake_minimum_required(VERSION 3.15)
project(uClient)

set(CMAKE_CXX_STANDARD 14)

add_executable(uClient main.cpp DieWithError.cpp ReadInput.cpp ProcessRequest.cpp PrintUtil.cpp)
