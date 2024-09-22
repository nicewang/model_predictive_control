cmake_minimum_required(VERSION 3.10)
project(mpc_project)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")

# Set dir of include, src and obj
set(INCDIR ${CMAKE_SOURCE_DIR}/include)
set(SRCDIR ${CMAKE_SOURCE_DIR}/src)
set(OBJDIR ${CMAKE_BINARY_DIR}/obj)

# Set obj files and src files
set(SRCFILES ${CMAKE_CURRENT_LIST_DIR}/src/mpc.cpp ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp)
set(OBJFILES ${OBJDIR}/mpc.o ${OBJDIR}/main.o)

# Include Dir
include_directories(${INCDIR})

# Add an object library to control the generation of object files
add_library(mpc_objects OBJECT ${SRCFILES})

# Set the output directory to the obj folder
set_target_properties(mpc_objects PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${OBJDIR}
    LIBRARY_OUTPUT_DIRECTORY ${OBJDIR}
    RUNTIME_OUTPUT_DIRECTORY ${OBJDIR}
)

# executable file
add_executable(mpc.so $<TARGET_OBJECTS:mpc_objects>)

# clean
add_custom_target(clean_mpc
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${OBJDIR}
    COMMAND ${CMAKE_COMMAND} -E remove ${CMAKE_BINARY_DIR}/mpc.so
)

