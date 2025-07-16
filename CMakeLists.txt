cmake_minimum_required(VERSION 3.15)
project(QUADRAQ_TGDK)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# === Gather engine and include files ===
file(GLOB_RECURSE HEADERS "${CMAKE_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/engine/*.cpp")

# === AI Backend Logic (No Default.cpp, No MyCustomAI.cpp) ===
set(BACKEND_FOUND FALSE)

if(EXISTS "${CMAKE_SOURCE_DIR}/AI_Backends/MaraAI.cpp")
    message(STATUS "Using MaraAI.cpp")
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/AI_Backends/MaraAI.cpp)
    list(APPEND HEADERS ${CMAKE_SOURCE_DIR}/AI_Backends/MaraAI.hpp)
    set(BACKEND_FOUND TRUE)
elseif(EXISTS "${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.cpp")
    message(STATUS "Using OliviaAI.cpp")
    list(APPEND SOURCES ${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.cpp)
    list(APPEND HEADERS ${CMAKE_SOURCE_DIR}/AI_Backends/OliviaAI.hpp)
    set(BACKEND_FOUND TRUE)
endif()

if(NOT BACKEND_FOUND)
    message(WARNING "No AI backend found (MaraAI.cpp or OliviaAI.cpp). Creating StubAI")

    # Fallback stub written directly to build dir
    file(WRITE ${CMAKE_BINARY_DIR}/StubAI.cpp
"#include \"StubAI.hpp\"\nvoid run_stub_ai() {}\n")

    file(WRITE ${CMAKE_BINARY_DIR}/StubAI.hpp
"#pragma once\nvoid run_stub_ai();\n")

    list(APPEND SOURCES ${CMAKE_BINARY_DIR}/StubAI.cpp)
    list(APPEND HEADERS ${CMAKE_BINARY_DIR}/StubAI.hpp)
endif()

# === Include Paths ===
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/AI_Backends
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
)

# === DirectXTK dependency ===
add_subdirectory(external/DirectXTK)

# === Build QUADRAQ Shared Library ===
add_library(QUADRAQ SHARED ${SOURCES} ${HEADERS} "AI_Backends/KerfumplInterceptor.cpp")
target_link_libraries(QUADRAQ PRIVATE DirectXTK d3d11)

# === Build CLI Executable ===
add_executable(quadraq_cli engine/QUADRAQ_CLI.cpp "AI_Backends/KerfumplInterceptor.cpp")
target_link_libraries(quadraq_cli PRIVATE QUADRAQ DirectXTK d3d11)
