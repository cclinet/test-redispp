cmake_minimum_required(VERSION 3.10.0)
project(
  redispp
  VERSION 0.1.0
  LANGUAGES C CXX)

set (CMAKE_CXX_STANDARD 20)

add_executable(redispp main.cpp)

find_path(BSHOSHANY_THREAD_POOL_INCLUDE_DIRS "BS_thread_pool.hpp")
target_include_directories(redispp
                           PRIVATE ${BSHOSHANY_THREAD_POOL_INCLUDE_DIRS})

find_package(redis++ CONFIG REQUIRED)
target_link_libraries(redispp PRIVATE redis++::redis++_static)
