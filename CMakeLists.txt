cmake_minimum_required(VERSION 3.10)

project(BasicProgram) # Set the project name

# The pinned doctest asks for CMake 2.8, which CMake 4 refuses.
# This lets its CMakeLists.txt configure under current CMake.
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)
add_subdirectory(external/doctest)
add_executable(BasicProgram main.cpp) # Add the executable target (replace main.cpp with your C++ source file)
target_link_libraries(BasicProgram PRIVATE doctest)

add_executable(tests tests/main.cpp) # Add the test executable
target_include_directories(tests PRIVATE ${CMAKE_SOURCE_DIR}/external/doctest/doctest)
target_link_libraries(tests PRIVATE doctest) # Link with doctest (if you added it as a subdirectory)
enable_testing() # Enable CTest if not already enabled
add_test(NAME doctest_tests COMMAND $<TARGET_FILE:tests> --success)


# Add a custom target to run the program after building
add_custom_target(
    run
    COMMAND BasicProgram
    DEPENDS BasicProgram
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
