# Specify the minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(BasicProgram)

# Add the executable target (replace main.cpp with your C++ source file)
add_executable(BasicProgram main.cpp)

# Add a custom target to run the program after building
add_custom_target(
    run
    COMMAND BasicProgram
    DEPENDS BasicProgram
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
