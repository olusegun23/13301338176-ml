#!/bin/bash
#
# Copyright 2016
#
# luban-startup ....
#

source /opt/ros/indigo/setup.bash
export ROS_IP=192.168.1.1
export ROS_HOSTNAME=192.168.1.1
export ROS_MASTER_URI=http://192.168.1.1:11311
source /home/linaro/nav/devel/setup.bash

roslaunch voxel_map  VoxelMap.launch > /home/linaro/nav/src/startup/voxel_map.log 2>&1 & 
