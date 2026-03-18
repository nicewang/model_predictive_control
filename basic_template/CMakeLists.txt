cmake_minimum_required(VERSION 3.10)
project(mpc_project)

# Dependencies
find_package(Eigen3 3.3 REQUIRED NO_MODULE)

# Compiler Settings
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Source Files (Using relative paths to current directory)
set(SRCFILES 
    src/mpc.cpp
    src/PgdUtils.cpp
    src/main.cpp
)
set(HEADERS
    include/mpc.h
    include/PgdUtils.h
)

# Include Directories
include_directories(include)
include_directories(${EIGEN3_INCLUDE_DIR})

# Create Executable (One-time definition)
add_executable(mpc ${SRCFILES} ${HEADERS})

# Linking
target_link_libraries(mpc PRIVATE Eigen3::Eigen)

# Output directory settings
set(BINDIR ${CMAKE_BINARY_DIR}/bin)
set_target_properties(mpc PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${BINDIR}
)

# Custom target to run the executable
if(NOT TARGET run)
    add_custom_target(run
        COMMAND ${BINDIR}/mpc
        DEPENDS mpc
        COMMENT "Running MPC example..."
    )
endif()
