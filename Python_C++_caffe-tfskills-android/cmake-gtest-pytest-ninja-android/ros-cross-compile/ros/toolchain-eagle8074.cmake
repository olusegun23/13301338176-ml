
cmake_minimum_required(VERSION 3.2)

###############################################################for Cmake #############################

add_definitions(-DENABLE_LOG=1)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_SYSTEM_VERSION 1)
#set(CMAKE_SYSROOT /local/mnt/workspace/cross/cross-root/eagle_sdk/sysroots/eagle8074) 

get_filename_component(SDK_DIR "${CMAKE_CURRENT_LIST_DIR}" ABSOLUTE)
set(CMAKE_SYSROOT "${SDK_DIR}/sysroots/eagle8074")
message( STATUS "================================: " ${CMAKE_SYSROOT})

set(PKG_CONFIG_SYSROOT_DIR ${CMAKE_SYSROOT})
set(PKG_CONFIG_PATH ${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/local/lib/pkgconfig:${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig)

set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT} ${CMAKE_SYSROOT}/opt/ros/indigo )
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

set(MY_CROSS_DIR ${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/bin)
set(MY_CROSS_prefix arm-linux-gnueabihf-)
set(CMAKE_AR ${MY_CROSS_DIR}/${MY_CROSS_prefix}ar)
SET(CMAKE_CXX_COMPILER ${MY_CROSS_DIR}/${MY_CROSS_prefix}g++)
SET(CMAKE_C_COMPILER  ${MY_CROSS_DIR}/${MY_CROSS_prefix}gcc)
SET(CMAKE_LINKER ${MY_CROSS_DIR}/${MY_CROSS_prefix}ld)
SET(CMAKE_NM ${MY_CROSS_DIR}/${MY_CROSS_prefix}nm)
SET(CMAKE_OBJCOPY ${MY_CROSS_DIR}/${MY_CROSS_prefix}objcopy)
SET(CMAKE_OBJDUMP ${MY_CROSS_DIR}/${MY_CROSS_prefix}objdump)
SET(CMAKE_RANLIB ${MY_CROSS_DIR}/${MY_CROSS_prefix}ranlib)

########for ros 's marco to find pthread##########
set(CMAKE_LIBRARY_PATH ${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf ${CMAKE_SYSROOT}/lib ${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf ${CMAKE_SYSROOT}/usr/lib ${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf)
set(CMAKE_INCLUDE_PATH ${CMAKE_SYSROOT}/usr/lib/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/include ${CMAKE_SYSROOT}/usr/lib/usr/include ${CMAKE_SYSROOT}/usr/include/arm-linux-gnueabihf)
set(CMAKE_PREFIX_PATH ${CMAKE_SYSROOT} ${CMAKE_SYSROOT}/opt/ros/indigo)



###################################### for CMake pass########################################################################

######################################for compiler pass ###################################################################################
#INCLUDE_DIRECTORIES(${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/include ${CMAKE_SYSROOT}/usr/include ${CMAKE_SYSROOT}/usr/include/arm-linux-gnueabihf)

set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -v --sysroot=${CMAKE_SYSROOT} -I${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/include -I${CMAKE_SYSROOT}/usr/include -I${CMAKE_SYSROOT}/usr/include/arm-linux-gnueabihf")

########################################for compiler pass ####################################################################################

#LINK_DIRECTORIES(${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf ${CMAKE_SYSROOT}/lib ${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf ${CMAKE_SYSROOT}/usr/lib ${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf)

set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS}  -v --sysroot=${CMAKE_SYSROOT} -L${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf -L${CMAKE_SYSROOT}/lib -L${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf -L${CMAKE_SYSROOT}/usr/lib -L${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihfa ") 
# -Wl,-rpath,${CMAKE_SYSROOT}/opt/gcc-linaro-arm-linux-gnueabihf-4.8-2013.08_linux/arm-linux-gnueabihf/libc/usr/lib/arm-linux-gnueabihf -Wl,-rpath,${CMAKE_SYSROOT}/lib -Wl,-rpath,${CMAKE_SYSROOT}/lib/arm-linux-gnueabihf -Wl,-rpath,${CMAKE_SYSROOT}/usr/lib ${CMAKE_SYSROOT}/usr/lib/arm-linux-gnueabihf -l:libboost_signals.so")
########################################for compiler pass ####################################################################################





#########for linker pass,some CMake module authnors will override standard CAMEK PATH,and generate /lib/XXX/XX.so we have to post process##

macro(change_inc_path arg)
set(cross__INCLUDE_DIRS "")
FOREACH(src ${${arg}})
    IF(src MATCHES "^/usr")
        MESSAGE("..APP..find /usr in {catkin_INCLUDE_DIRS}:" ${src})
        set(dst "")
        string(REGEX REPLACE "^/usr" "${CMAKE_SYSROOT}/usr" dst ${src})
        set(src ${dst})
        MESSAGE("..APP.. after repleace..{catkin_INCLUDE_DIRS}:" ${src})
    ENDIF()
set(cross_INCLUDE_DIRS ${cross_INCLUDE_DIRS} ${src})
ENDFOREACH()
set(${arg} ${cross_INCLUDE_DIRS})
endmacro()





macro(change_lib_path arg)
set(cross_LIBRARIES "")
FOREACH(src ${${arg}})
    IF(src MATCHES "^/usr")
        MESSAGE("..APP..find /usr in {catkin_LIBRARIES}:" ${src})
        set(dst "")
        string(REGEX REPLACE "^/usr" "${CMAKE_SYSROOT}/usr" dst ${src})
        set(src ${dst})
        MESSAGE("..APP.. after repleace..{catkin_LIBRARIES}:" ${src})
    ENDIF()
      #STRING( REGEX REPLACE "(.*/.*)/.*/.cpp$" "/1" dirName ${src} )
    IF(src MATCHES "^/lib")
        MESSAGE("..APP..find /lib in {catkin_LIBRARIES}:" ${src})
        set(dst "")
        string(REGEX REPLACE "^/lib" "${CMAKE_SYSROOT}/lib" dst ${src})
        set(src ${dst})
        MESSAGE("..APP.. after repleace..{catkin_LIBRARIES}:" ${src})
    ENDIF()

    IF(src MATCHES "^/opt")
        MESSAGE("..APP..find /opt in {catkin_LIBRARIES}:" ${src})
        set(dst "")
        string(REGEX REPLACE "^/opt" "${CMAKE_SYSROOT}/opt" dst ${src})
        set(src ${dst})
        MESSAGE("..APP.. after repleace..{catkin_LIBRARIES}:" ${src})
    ENDIF()

set(cross_LIBRARIES ${cross_LIBRARIES} ${src})
ENDFOREACH()
#set(catkin_LIBRARIES ${cross_LIBRARIES})
set(${arg} ${cross_LIBRARIES})
endmacro()
# Declare the version of the CMake API for forward-compatibility

