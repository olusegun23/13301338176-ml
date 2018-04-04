cmake_minimum_required(VERSION 3.10)
project(cmc)

enable_testing()
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

file(GLOB SRCS *.cpp)
add_executable(testcmc ${SRCS})
target_include_directories(testcmc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(testcmc GTest::GTest GTest::Main)

add_test(AllTestsIntestcmc testcmc)
#add_test(NAME testcmc COMMAND testcmc)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



