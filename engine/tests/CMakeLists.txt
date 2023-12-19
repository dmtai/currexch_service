cmake_minimum_required(VERSION 3.12)
project(engine_tests)

enable_testing()
find_package(GTest REQUIRED)

file(GLOB_RECURSE SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/http_server/*_test.*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/utils/*_test.*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/components/*_test.*pp
  ${CMAKE_CURRENT_SOURCE_DIR}/mocks/http_server/.*pp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} 
  PRIVATE 
    ../
    ../src
    Boost::Boost 
    third_party
    third_party/ozo/include
    third_party/ozo/contrib/resource_pool/include
)

target_link_libraries(${PROJECT_NAME} 
  PRIVATE 
    engine
    Boost::Boost 
    fmt::fmt
    Threads::Threads
    yaml-cpp
    spdlog::spdlog
    nlohmann_json::nlohmann_json
    PostgreSQL::PostgreSQL
    url_router
    GTest::gtest_main
    GTest::gmock_main
)

add_test(NAME ${PROJECT_NAME} 
         COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME})
set_tests_properties(${PROJECT_NAME} PROPERTIES TIMEOUT 20)