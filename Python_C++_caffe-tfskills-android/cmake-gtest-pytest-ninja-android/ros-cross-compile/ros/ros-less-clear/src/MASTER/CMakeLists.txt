
 

project(QROBOT)

cmake_minimum_required(VERSION 2.6)

set(QROBOT_TARGET_NAME QROBOT_MASTER_MV_TASK)



###############################################################################

# Project directories

###############################################################################

get_filename_component(MV_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/../../3rdlib/mv" ABSOLUTE)

get_filename_component(PROJECT_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE)

 

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

file(GLOB QROBOT_SRC ${PROJECT_ROOT_DIR}/MASTER/MASTER_MV_QROBOT_TASK.cpp)
#                                  ${PROJECT_ROOT_DIR}/VSLAM/ReadImages.cpp
#                                  ${MV_ROOT_DIR}/ext/apq8074/src/EagleCamera.cpp)
                             

# add the executable

add_executable(${QROBOT_TARGET_NAME} ${QROBOT_SRC})

 

# libraries

add_library(mv SHARED IMPORTED)

set_property(TARGET mv PROPERTY IMPORTED_LOCATION ${MV_ROOT_DIR}/bin/lin/${TARGET_ARCH_DIR}/libmv.so)

target_link_libraries(${QROBOT_TARGET_NAME} mv)

target_link_libraries(${QROBOT_TARGET_NAME} pthread)

target_link_libraries(${QROBOT_TARGET_NAME} adsprpc)

target_link_libraries(${QROBOT_TARGET_NAME} ${MV_ROOT_DIR}/ext/apq8074/lib/rpcmem.a)

target_link_libraries(${QROBOT_TARGET_NAME} camera)

target_link_libraries(${QROBOT_TARGET_NAME} camparams)

 

set_target_properties(${QROBOT_TARGET_NAME} PROPERTIES COMPILE_FLAGS "-std=c++11" )

