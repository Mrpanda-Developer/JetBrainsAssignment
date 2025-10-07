cmake_minimum_required(VERSION 3.12)
project(JetBrainsAssignment LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Where to find headers and src files
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Build the static library
add_library(tree STATIC
    src/AVLUtils.cpp
    src/ByteComparator.cpp
    src/Locking.cpp
    src/SortedByteTree.cpp
)

# Build the test executable
add_executable(main_test
    test/main.cpp
)

# Link the library to the test executable
target_link_libraries(main_test PRIVATE tree)

set_target_properties(main_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

if (MSVC)
    target_compile_options(tree PRIVATE /W4)
    target_compile_options(main_test PRIVATE /W4)
else()
    target_compile_options(tree PRIVATE -Wall -Wextra -pedantic)
    target_compile_options(main_test PRIVATE -Wall -Wextra -pedantic)
endif()
