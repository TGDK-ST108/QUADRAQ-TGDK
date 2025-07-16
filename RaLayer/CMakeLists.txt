cmake_minimum_required(VERSION 3.15)
project(RaLayer)

add_library(RaLayer SHARED RaLayer.cpp)

target_include_directories(RaLayer PRIVATE ../)
set_target_properties(RaLayer PROPERTIES PREFIX "" SUFFIX ".dll")
