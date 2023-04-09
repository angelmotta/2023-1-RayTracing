cmake_minimum_required(VERSION 3.19)
project(CG2023)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS -I/opt/X11/include)
find_package(X11 REQUIRED)

add_executable(CG2023 main.cpp vec3.cpp vec3.h Rayo.cpp Rayo.h
        Objeto.cpp Objeto.h Camara.cpp Camara.h)
target_link_libraries(CG2023 ${X11_LIBRARIES})