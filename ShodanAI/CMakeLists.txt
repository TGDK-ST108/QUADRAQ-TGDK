cmake_minimum_required(VERSION 3.15)
project(ShodanAI)

add_library(ShodanAI SHARED ShodanAI.cpp)

target_include_directories(ShodanAI PRIVATE ../)
set_target_properties(ShodanAI PROPERTIES PREFIX "" SUFFIX ".dll")
