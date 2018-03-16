#!/usr/bin/env python
import rospy
import os
import subprocess
from std_msgs.msg import String
import psutil

global status
status="map"
def callback(data):
    global status 
    print status 
    rospy.loginfo(rospy.get_caller_id() + "%s:*****:", data.data)
    if data.data=="nav" and status=="map":
 	#os.system("sudo initctl emit start_movebase  2>&1  &")	
	cmd = subprocess.Popen('roslaunch  motion_planning  move_base.launch > /home/linaro/nav/src/startup/motion_planning.log 1>&2  &', shell=True, stdout=subprocess.PIPE)
	#global status
        status="nav"
	
    
def listener():

    # In ROS, nodes are uniquely named. If two nodes with the same
    # node are launched, the previous one is kicked off. The
    # anonymous=True flag means that rospy will choose a unique
    # name for our 'listener' node so that multiple listeners can
    # run simultaneously.
    rospy.init_node('monitor', anonymous=True)

    rospy.Subscriber("switchmode", String, callback)

    # spin() simply keeps python from exiting until this node is stopped
    pub_cpu = rospy.Publisher('cpu', String, queue_size=1)
    pub_mem = rospy.Publisher('mem', String, queue_size=1)
    pub_disk = rospy.Publisher('disk', String, queue_size=1)
    pub_movebase = rospy.Publisher('movebase_status', String, queue_size=1)
	

    rate = rospy.Rate(0.5) # 10hz
    while not rospy.is_shutdown():
        result = psutil.cpu_percent()
        rospy.loginfo(result)
        pub_cpu.publish("cpu: "+str(result))
        result = psutil.virtual_memory()
        rospy.loginfo(result)
        pub_mem.publish("mem: "+str(result))
        result = psutil.disk_usage('/')
        rospy.loginfo(result)
        pub_disk.publish("disk: "+str(result))
        cmd = subprocess.Popen('tail -600l /home/linaro/nav/src/startup/motion_planning.log  | grep WARN', shell=True, stdout=subprocess.PIPE)
        #out, err = cmd.communicate()
	for line in cmd.stdout:
            if "WARN" in line:
                pub_movebase.publish("movebase :WARN ")
            else:
                pub_movebase.publish("movebase :OK ")

        
        rate.sleep()

if __name__ == '__main__':
    
    try:
        listener()
    except rospy.ROSInterruptException:
        pass
