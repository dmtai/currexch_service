cmake_minimum_required(VERSION 3.12)
project(url_router VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/impl/*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/detail/impl/*pp
)

add_library(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE Boost::Boost)
target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
    Boost::Boost 
    Threads::Threads
)