
cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_COMPILER icx)
set(CMAKE_CXX_COMPILER icpx)
set(CMAKE_CXX_FLAGS "-std=c++2b -sycl-std=2020")

set(PROJ "variadic-macros-2")
project(${PROJ})
find_package(IntelDPCPP REQUIRED)

include_directories(
  /opt/intel/oneapi/compiler/2022.2.0/linux/include/sycl)

add_executable(${PROJ}
  main.cc)

target_compile_options(${PROJ}
  PRIVATE
  -Wall
  -O3)

target_link_libraries(${PROJ}
  PRIVATE
  EGL
  OpenCL
  OpenGL)

add_custom_target(run
  DEPENDS ${PROJ}
  COMMAND ${PROJ})
