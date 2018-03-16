###############################################################################

# MVSAMPLEVISLAMEAGLE build

###############################################################################

 

project(QROBOT)

cmake_minimum_required(VERSION 2.6)

###############################################################################

# Project directories

###############################################################################

get_filename_component(MV_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../3rdlib/mv" ABSOLUTE)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../" ABSOLUTE)

 

###############################################################################

# Target platform-specific settings

###############################################################################

set(TARGET_ARCH_DIR armeabi-v7a)

###############################################################################

# Status

###############################################################################

message( STATUS "Target Architecture: " ${PROJECT_ROOT_DIR}) 


###############################################################################

# Build configuration

###############################################################################

# include directories

include_directories("${MV_ROOT_DIR}/inc")
include_directories("${MV_ROOT_DIR}/ext/apq8074/inc")

include_directories("${PROJECT_ROOT_DIR}/FRAMEWORK")
include_directories("${PROJECT_ROOT_DIR}/VSLAM")
include_directories("${PROJECT_ROOT_DIR}/DFS")
include_directories("${PROJECT_ROOT_DIR}/AE")
include_directories("${PROJECT_ROOT_DIR}/2DMAP")
include_directories("${PROJECT_ROOT_DIR}/SF")

# Make sure we include lib dir for Linux build

link_directories(${MV_ROOT_DIR}/ext/apq8074/lib)


# source files

file(GLOB QROBOT_LIB_SRC ${PROJECT_ROOT_DIR}/INTERFACE/lib/INTERFACE_QROBOT_H.cpp)
                             

# add the executable
#add_executable(qrobot_dynamic ${QROBOT_SRC})


add_library(qrobot_dynamic SHARED ${QROBOT_LIB_SRC})
add_library(qrobot_static STATIC ${QROBOT_LIB_SRC})


# libraries

add_library(mv SHARED IMPORTED)
set_property(TARGET mv PROPERTY IMPORTED_LOCATION ${MV_ROOT_DIR}/bin/lin/${TARGET_ARCH_DIR}/libmv.so)

target_link_libraries(qrobot_dynamic mv)
target_link_libraries(qrobot_dynamic pthread)
target_link_libraries(qrobot_dynamic adsprpc)
target_link_libraries(qrobot_dynamic ${MV_ROOT_DIR}/ext/apq8074/lib/rpcmem.a)
target_link_libraries(qrobot_dynamic camera)
target_link_libraries(qrobot_dynamic camparams)
set_target_properties(qrobot_dynamic PROPERTIES COMPILE_FLAGS "-std=c++11" )


target_link_libraries(qrobot_static mv)
target_link_libraries(qrobot_static pthread)
target_link_libraries(qrobot_static adsprpc)
target_link_libraries(qrobot_static ${MV_ROOT_DIR}/ext/apq8074/lib/rpcmem.a)
target_link_libraries(qrobot_static camera)
target_link_libraries(qrobot_static camparams)
set_target_properties(qrobot_static PROPERTIES COMPILE_FLAGS "-std=c++11" )


