cmake_minimum_required(VERSION 3.15)
project(TaraAI)

add_library(TaraAI SHARED TaraAI.cpp)

target_include_directories(TaraAI PRIVATE ../)
set_target_properties(TaraAI PROPERTIES PREFIX "" SUFFIX ".dll")
