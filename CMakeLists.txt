cmake_minimum_required(VERSION 3.18)
project(mpc_python LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
message(STATUS "Eigen3 include dirs: ${EIGEN3_INCLUDE_DIR}")

find_package(pybind11 REQUIRED)

pybind11_add_module(mpc_core 
    mpc_python/cpp/bindings.cpp
    mpc_python/cpp/mpc.cpp
    mpc_python/cpp/utils/PgdUtils.h
    mpc_python/cpp/utils/PgdUtils.cpp
)

target_include_directories(mpc_core PUBLIC
    ${EIGEN3_INCLUDE_DIR}
    "${CMAKE_CURRENT_SOURCE_DIR}/mpc_python/cpp" 
)

target_link_libraries(mpc_core PRIVATE Eigen3::Eigen)

install(TARGETS mpc_core DESTINATION mpc_python)