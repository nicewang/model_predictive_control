cmake_minimum_required(VERSION 3.10)
project(mpc_project)

# Find and Link Eigen3
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Compiler Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Directory Paths
set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(SRCDIR ${CMAKE_SOURCE_DIR}/src)
set(BINDIR ${CMAKE_BINARY_DIR}/bin)

# Source Files
set(SRCFILES 
    ${SRCDIR}/mpc.cpp 
    ${SRCDIR}/main.cpp
)

set(HEADERS
    ${INCDIR}/mpc.h
)

# Include Directories
include_directories(${INCDIR})
include_directories(${EIGEN3_INCLUDE_DIR})

# Create Executable
add_executable(mpc ${SRCFILES} ${HEADERS})

# Link Eigen3
target_link_libraries(mpc PRIVATE Eigen3::Eigen)

# Set Source File
set(INCDIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRCDIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(SRCFILES 
    ${SRCDIR}/mpc.cpp 
    ${SRCDIR}/main.cpp
)

set(HEADERS
    ${INCDIR}/mpc.h
)

# Set output directory
set_target_properties(mpc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
)

# # Optional: Build Shared Library
# add_library(libmpc SHARED ${SRCDIR}/mpc.cpp)
# target_link_libraries(libmpc PRIVATE Eigen3::Eigen)
# set_target_properties(libmpc PROPERTIES
#     LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
# )

# Build and run
add_custom_target(run
    COMMAND ${BINDIR}/mpc
    DEPENDS mpc
    COMMENT "Running MPC example..."
)

# # Clean build artifacts (for local run) 
# add_custom_target(clean_all
#     COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
#     COMMENT "Cleaning all build artifacts..."
# )
