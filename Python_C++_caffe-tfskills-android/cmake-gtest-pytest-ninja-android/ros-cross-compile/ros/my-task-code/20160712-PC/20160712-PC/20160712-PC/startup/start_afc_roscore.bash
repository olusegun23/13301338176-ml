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


afc > /home/linaro/nav/src/startup/afc.log 2>&1  &
sleep 2
roscore > /home/linaro/nav/src/startup/roscore.log 2>&1 & 

