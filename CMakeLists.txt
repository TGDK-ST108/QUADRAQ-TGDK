cmake_minimum_required(VERSION 3.15)
project(QUADRAQ_TGDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Gather headers and sources
file(GLOB_RECURSE HEADERS
    "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/engine/*.cpp"
)

# Optional AI backend logic
if(EXISTS "${CMAKE_SOURCE_DIR}/AI_Backends/MyCustomAI.cpp")
    message(STATUS "Using MyCustomAI.cpp")
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/AI_Backends/MyCustomAI.cpp)
    list(APPEND HEADERS ${CMAKE_SOURCE_DIR}/AI_Backends/MyCustomAI.hpp)
elseif(EXISTS "${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.cpp")
    message(STATUS "Using OliviaAI.cpp")
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.cpp)
    list(APPEND HEADERS ${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.hpp)
else()
    message(WARNING "No AI backend found (OliviaAI.cpp or MyCustomAI.cpp). Creating StubAI")

    # Write temporary stub fallback
    file(WRITE ${CMAKE_BINARY_DIR}/StubAI.cpp
"#include \"StubAI.hpp\"\nvoid run_stub_ai() {}\n")

    file(WRITE ${CMAKE_BINARY_DIR}/StubAI.hpp
"#pragma once\nvoid run_stub_ai();\n")

    list(APPEND SOURCES ${CMAKE_BINARY_DIR}/StubAI.cpp)
    list(APPEND HEADERS ${CMAKE_BINARY_DIR}/StubAI.hpp)
endif()

# Include directories
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_BINARY_DIR})  # for stub fallback

# === DirectXTK via external ===
add_subdirectory(external/DirectXTK)

# Build shared library
add_library(QUADRAQ SHARED ${SOURCES} ${HEADERS})
target_link_libraries(QUADRAQ PRIVATE DirectXTK d3d11)

# CLI Executable
add_executable(quadraq_cli engine/QUADRAQ_CLI.cpp)
target_link_libraries(quadraq_cli PRIVATE QUADRAQ DirectXTK d3d11)
