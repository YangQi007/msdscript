cmake_minimum_required(VERSION 3.20)
project(msdscript)

set(CMAKE_CXX_STANDARD 14)

add_executable(msdscript main.cpp cmdline.cpp cmdline.h expr.cpp expr.h parse.cpp parse.h val.cpp val.h pointer.h env.cpp env.h step.cpp step.h cont.cpp cont.h)
