cmake_minimum_required(VERSION 3.12)
project(engine VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

set(THREADS_PREFER_PTHREAD_FLAG ON)
include(cmake/sanitizers.cmake)

# required by ozo library
add_definitions( 
  -DBOOST_ASIO_USE_TS_EXECUTOR_AS_DEFAULT 
  -DBOOST_HANA_CONFIG_ENABLE_STRING_UDL
)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)
find_package(fmt REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PostgreSQL REQUIRED) # required by ozo library
find_package(libcuckoo REQUIRED)

add_subdirectory(third_party/router)

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/http_server/*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/utils/*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/postgres/*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/components/*pp
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} 
  PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ozo/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third_party/ozo/contrib/resource_pool/include  
    Boost::Boost
)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
    libcuckoo::libcuckoo
    Boost::Boost 
    fmt::fmt
    Threads::Threads
    yaml-cpp
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    PostgreSQL::PostgreSQL    
    url_router
)

add_subdirectory(tests)