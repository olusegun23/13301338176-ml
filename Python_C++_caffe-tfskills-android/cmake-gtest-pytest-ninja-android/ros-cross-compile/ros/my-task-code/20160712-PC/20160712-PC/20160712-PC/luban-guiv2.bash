#!/bin/bash
#
# Copyright 2016
#rostopic pub localization_status std_msgs/String "good"
#rostopic pub voxel_map_status std_msgs/String "stillness"
# luban-startup ....
#
source /opt/ros/indigo/setup.bash
export ROS_IP=192.168.1.20
export ROS_HOSTNAME=192.168.1.20
export ROS_MASTER_URI=http://192.168.1.1:11311
source /home/robot_crd/20160712-PC/tele-op/devel/setup.bash



result=0
check_hardware()
{
          return 0
  }



while [ $result -ne 0  ]
do
 echo "check_hardware not passed"
 echo $?
 echo "==============="
 echo $result
 check_hardware
                    
done






time=@
time=$time$(date +%s)

echo $time


command1="ssh -v -E ssh.log -t  linaro@192.168.1.1  sudo date -s  $time"
command2="ssh  -v -E ssh.log -t  linaro@192.168.1.1  sudo initctl emit startnav"
command3="ssh  -v -E ssh.log -t  linaro@192.168.1.1  sudo initctl emit start_monitor"
command4="ssh  -v -E ssh.log -t  linaro@192.168.1.1  sudo initctl emit start_localization"
command5="ssh  -v -E ssh.log -t  linaro@192.168.1.1  sudo initctl emit start_voxmap"

expect -c "
  set timeout 20;
  spawn $command1;
  expect {
                \"\password:\" {send \"linaro\r\"; exp_continue}
                }
	spawn $command2;
	expect {
                \"\password:\" {send \"linaro\r\"; exp_continue}
                }
        "
roscore_result=0
while [ $roscore_result -eq 0  ]
do
 echo "=====roscore not started====="
 roscore_result=$(rosnode list | grep rosout |wc -l)

done

        
        
 expect -c "
 set timeout 20;	
 spawn $command3;
  expect {
            \"\password:\" {send \"linaro\r\"; exp_continue}
          }
          "


monitor_result=0
while [ $monitor_result -eq 0  ]
do
 echo "=====monitor not started====="
 monitor_result=$(rostopic list | grep switchmode |wc -l)

done

 expect -c "
 set timeout 20;
 spawn $command4;
  expect {
            \"\password:\" {send \"linaro\r\"; exp_continue}
          }
           "

localization_result=0
while [ $localization_result -eq 0  ]
do
 echo "=====localization not started====="
 localization_result=$(rostopic  list | grep localization_status |wc -l)

done

 
  expect -c "
  set timeout 20;
  spawn $command5;
  expect {
           \"\password:\" {send \"linaro\r\"; exp_continue}
          }
            "

voxmap_result=0
while [ $voxmap_result -eq 0  ]
do
 echo "=====voxmap not started====="
 voxmap_result=$(rostopic  list | grep voxel_map_status |wc -l)

done



rosrun rviz rviz -d /home/robot_crd/20160712-PC/lu-ban-M1.rviz &

rosrun teleop_twist_keyboard  teleop_luban_M1.py &

rosrun  robot_status  status.py  & 
