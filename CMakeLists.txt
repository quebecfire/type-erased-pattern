cmake_minimum_required(VERSION 3.25)
project(type_erased)

set(CMAKE_CXX_STANDARD 23)

add_executable(type_erased main.cpp Shape.h drawing-strategy/draw1.h drawing-strategy/draw2.h shape/Circle.h shape/Square.h)
