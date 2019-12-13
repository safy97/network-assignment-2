cmake_minimum_required(VERSION 3.15)
project(uServer)

set(CMAKE_CXX_STANDARD 14)
SET(CMAKE_CXX_FLAGS -pthread)

add_executable(uServer main.cpp DieWithError.cpp ReadInput.cpp ReadFile.cpp ProcessRequest.cpp SlowStart.cpp FastRecovery.cpp CongetionAvoidance.cpp PrintUtil.cpp RandomGenerator.cpp)
