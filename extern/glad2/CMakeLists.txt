cmake_minimum_required(VERSION 3.22)
project(glad2)


add_library(glad2 include/glad/gles2.h include/KHR/khrplatform.h src/gles2.c)
target_include_directories(glad2 PUBLIC include)