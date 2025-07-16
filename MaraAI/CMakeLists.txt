cmake_minimum_required(VERSION 3.15)
project(MaraAI)

add_library(MaraAI SHARED MaraAI.cpp)

target_include_directories(MaraAI PRIVATE ../)
set_target_properties(MaraAI PROPERTIES PREFIX "" SUFFIX ".dll")
