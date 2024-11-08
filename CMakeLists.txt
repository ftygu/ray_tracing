# 指定最低的 CMake 版本要求
cmake_minimum_required(VERSION 3.10)

# 定义项目名
project(ray_tracer)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -ftree-vectorize -funsafe-math-optimizations -fopenmp")

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 查找 SDL2 库
find_package(SDL2 REQUIRED)

# 包含 SDL2 的头文件
include_directories(${SDL2_INCLUDE_DIRS})

# 添加源文件
add_executable(ray_tracer 
    src/main.cpp 
    src/image.cpp 
    src/ray.cpp 
    src/camera.cpp 
    src/hittable.cpp 
    src/sphere.cpp
    src/triangle.cpp
    src/hittable_list.cpp
    src/random_generator.cpp
    src/material.cpp
)

# 链接 SDL2 库
target_link_libraries(ray_tracer ${SDL2_LIBRARIES})
