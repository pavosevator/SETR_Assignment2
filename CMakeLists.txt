cmake_minimum_required(VERSION 3.10)

# Set project
project(CommandProcessing_Project VERSION 1.0 LANGUAGES C)

# Set C complier
set(CMAKE_C_COMPILER "gcc")

# Add MyDLL folder to the build
add_subdirectory(CmdProc)

# Generate variables with set of libs and include folders
# Dependencies are automatically set
list(APPEND EXTRA_LIBS CmdProcLib)
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/CmdProc")

# set source folder variable and add it in directories 
set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${SRC_DIR})

# collect all source files from from source folder 
set(SOURCES ${SRC_DIR}/main.c)

#set the executable
add_executable(cmdproc ${SOURCES})

# Specify libraries or flags to use when linking a given target and/or its dependents
target_link_libraries(cmdproc PUBLIC ${EXTRA_LIBS})

# Specifies include directories to use when compiling a given target
target_include_directories(cmdproc PUBLIC ${EXTRA_INCLUDES})

#set target compile options
target_compile_options(cmdproc PRIVATE -Wall -Wextra -Wpedantic)