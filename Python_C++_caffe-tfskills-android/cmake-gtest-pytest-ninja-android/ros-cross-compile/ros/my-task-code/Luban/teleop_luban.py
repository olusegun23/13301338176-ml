#!/usr/bin/env python
import roslib; roslib.load_manifest('teleop_twist_keyboard')
import rospy
#from PyQt4.QtCore import *  
#from PyQt4.QtGui import * 

#from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from geometry_msgs.msg import *
from tf.msg import *


from python_qt_binding.QtGui import *
from python_qt_binding.QtCore import *

from visualization_msgs.msg import Marker
from visualization_msgs.msg import MarkerArray
import rospy
import math
import tf

import sys, select, termios, tty

msg = """
Reading from the keyboard  and Publishing to Twist!
---------------------------
Moving around:
   u    i    o
   j    k    l
   m    ,    .

For Holonomic mode (strafing), hold down the shift key:
---------------------------
   U    I    O
   J    K    L
   M    <    >

t : up (+z)
b : down (-z)

anything else : stop

q/z : increase/decrease max speeds by 10%
w/x : increase/decrease only linear speed by 10%
e/c : increase/decrease only angular speed by 10%

CTRL-C to quit
"""



'''moveBindings = {
		'i':(1,0,0,0),
		'o':(1,0,0,-1),
		'j':(0,0,0,1),
		'l':(0,0,0,-1),
		'u':(1,0,0,1),
		',':(-1,0,0,0),
		'.':(-1,0,0,1),
		'm':(-1,0,0,-1),
		'O':(1,-1,0,0),
		'I':(1,0,0,0),
		'J':(0,1,0,0),
		'L':(0,-1,0,0),
		'U':(1,1,0,0),
		'<':(-1,0,0,0),
		'>':(-1,-1,0,0),
		'M':(-1,1,0,0),
		't':(0,0,1,0),
		'b':(0,0,-1,0),
	       }

speedBindings={
		'q':(1.1,1.1),
		'z':(.9,.9),
		'w':(1.1,1),
		'x':(.9,1),
		'e':(1,1.1),
		'c':(1,.9),
	      }

def getKey():
	tty.setraw(sys.stdin.fileno())
	select.select([sys.stdin], [], [], 0)
	key = sys.stdin.read(1)
	termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
	return key
'''
speed =0.1 
turn = 1
isPin= 0
current_odom =Odometry()

def x1buttonClick( self ):
	global speed
        global turn
        x =1 
        y =0 
        z =0
        th =0 
        twist = Twist()
        twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
        pub.publish(twist)


def x2buttonClick( self ):
        global speed
        global turn
        x =-1 
        y =0
        z =0
        th =0 
        twist = Twist()
        twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
        pub.publish(twist)

def z1buttonClick( self ):
        global speed
        global turn
        x =0
        y =0
        z =0
        th =1
        twist = Twist()
        twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
        pub.publish(twist)

def z2buttonClick( self ):
        global speed
        global turn
        x =0
        y =0
        z =0
        th =-1
        twist = Twist()
        twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
        twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
        pub.publish(twist)

def x1speedbuttonClick( self ):
        global speed
        global turn
        speed=speed+speed*0.2
        print "speed=",speed

def x2speedbuttonClick( self ):
        global speed
        global turn
        speed=speed-speed*0.2
	print "speed=",speed
def z1turnbuttonClick( self ):
        global speed
        global turn
        turn=turn+turn*0.2
	print "turn=",turn

def z2turnbuttonClick( self ):
        global speed
        global turn
        turn=turn-turn*0.2
	print "turn=",turn
def pinbuttonClick( self ):
        global isPin 
        global current_odom
	global listener
        #print isPin
        if isPin == 0: 
                print current_odom	
	try:
    		now = rospy.Time.now()
    		listener.waitForTransform("map", "/odom", rospy.Time(), rospy.Duration(4.0))
    		#print "+++++++"
    		(trans,rot) = listener.lookupTransform("map", "/odom", now)
    		# not work, why?(trans,rot) = listener.lookupTransform('map', '/odmo', rospy.Time(0))
    		#print "======="
    		#print trans
		current_odom.pose.pose.position.x=current_odom.pose.pose.position.x+trans[0]
                current_odom.pose.pose.position.y=current_odom.pose.pose.position.y+trans[1]
                current_odom.pose.pose.position.z=current_odom.pose.pose.position.z+trans[2]
	
        	markerArray = MarkerArray()
        	marker = Marker()
        	marker.header.frame_id = "map"
        	#marker.type = marker.ARROW
		marker.type = marker.SPHERE
        	marker.action = marker.ADD
        	marker.scale.x = 0.6 
        	marker.scale.y = 0.6 
        	marker.scale.z = 0.6 
        	marker.color.a = 1.0
        	marker.color.r = 1.0
        	marker.color.g = 0.0
        	marker.color.b = 0.0
        	marker.pose.orientation.w = 0.4
        	marker.pose.position.x = current_odom.pose.pose.position.x
        	marker.pose.position.y = current_odom.pose.pose.position.y
        	marker.pose.position.z = current_odom.pose.pose.position.z
        	#marker.pose.position.x = rospy.get_param("pin_x")
        	#marker.pose.position.y = rospy.get_param("pin_y")
        	#marker.pose.position.z = 0
		isPin=1
        	markerArray.markers.append(marker)
        	publisher.publish(markerArray)
		print "##good to end##"
	except (tf.LookupException, tf.ConnectivityException, tf.ExtrapolationException):
            print "####"

def unpinbuttonClick( self ):
        global isPin 
        global current_odom 
        print isPin
        print current_odom
	isPin = 0
def navbuttonClick( self ):
        poseStamped = PoseStamped()
        #print rospy.get_param("pin_x")
        #print rospy.get_param("pin_y")
        poseStamped.header.frame_id ="map"
        #poseStamped.pose.position.x =rospy.get_param("pin_x")
        #poseStamped.pose.position.y =rospy.get_param("pin_y")  
        #poseStamped.pose.position.z = 0
        poseStamped.pose.position.x = current_odom.pose.pose.position.x
        poseStamped.pose.position.y = current_odom.pose.pose.position.y
        poseStamped.pose.position.z = current_odom.pose.pose.position.z
        poseStamped.pose.orientation.x=0
        poseStamped.pose.orientation.y=0
        poseStamped.pose.orientation.z=0
        poseStamped.pose.orientation.w=1
        goal_pub.publish(poseStamped)
    	print poseStamped


def vels(speed,turn):
	return "currently:\tspeed %s\tturn %s " % (speed,turn)

def odometryCb(msg):
    global current_odom
    global isPin
    #print isPin  
    if isPin == 0: 
    	#rospy.set_param('pin_x', msg.pose.pose.position.x)
    	#rospy.set_param('pin_y', msg.pose.pose.position.y)
        current_odom=msg
    
	

if __name__=="__main__":


        rospy.init_node('teleop_twist_keyboard')
 	settings = termios.tcgetattr(sys.stdin)
	#pub = rospy.Publisher('cmd_vel', Twist, queue_size = 1)
        pub = rospy.Publisher('/cmd_vel_mux/input/teleop', Twist, queue_size = 1)
        publisher = rospy.Publisher('visualization_marker_array', MarkerArray,queue_size = 1)
        goal_pub = rospy.Publisher('/move_base_simple/goal', PoseStamped, queue_size = 1)
        
        rospy.Subscriber('odom',Odometry,odometryCb)
        listener = tf.TransformListener()

	#rospy.init_node('teleop_twist_keyboard')
        '''x =0
	y = 0
	z = 0
	th = 0
	status = 0'''


	
        app = QApplication( sys.argv )
        myviz = QWidget()
	myviz.resize(800, 800)
	myviz.move(200, 200)
	myviz.setWindowTitle('Simple')
        vlayout = QVBoxLayout()
        hlayout = QHBoxLayout()
        hlayout1 = QHBoxLayout()
        hlayout2 = QHBoxLayout()

        x1button = QPushButton( "forward")
	x1button.setAutoRepeat(True)
	x1button.setFixedSize(150,150)
        x1button.clicked.connect(x1buttonClick )
	
        x2button = QPushButton( "back" )
	x2button.setAutoRepeat(True)
	x2button.setFixedSize(150,150)
        x2button.clicked.connect(x2buttonClick )
        z1button = QPushButton( "left" )
	z1button.setAutoRepeat(True)
	z1button.setFixedSize(150,150)
        z1button.clicked.connect(z1buttonClick )
        z2button = QPushButton( "right" )
	z2button.setAutoRepeat(True)
	z2button.setFixedSize(150,150)
        z2button.clicked.connect(z2buttonClick )
        hlayout.addWidget( x1button )
        hlayout.addWidget( x2button )
        hlayout.addWidget( z1button )
	hlayout.addWidget( z2button )


        x1speedbutton = QPushButton( "speed increase 20%" )
	x1speedbutton.setFixedSize(150,150)
        x1speedbutton.clicked.connect(x1speedbuttonClick )
        x2speedbutton = QPushButton( "speed decrease 20%" )
	x2speedbutton.setFixedSize(150,150)
        x2speedbutton.clicked.connect(x2speedbuttonClick )
        z1turnbutton = QPushButton( "angle increase 20%" )
	z1turnbutton.setFixedSize(150,150)
        z1turnbutton.clicked.connect(z1turnbuttonClick )
        z2turnbutton = QPushButton( "angle decrease 20%" )
	z2turnbutton.setFixedSize(150,150)
        z2turnbutton.clicked.connect(z2turnbuttonClick )
        hlayout1.addWidget(x1speedbutton)
        hlayout1.addWidget(x2speedbutton)
        hlayout1.addWidget(z1turnbutton)
        hlayout1.addWidget(z2turnbutton)

        x1=current_odom.pose.pose.position.x
        y1=current_odom.pose.pose.position.y

        pinbutton = QPushButton( "pin")
	pinbutton.setFixedSize(150,150)
        pinbutton.clicked.connect(pinbuttonClick)
        #pinbutton.clicked.connect(lambda: on_button(x1,y1))
        unpinbutton = QPushButton( "unset pin")
	unpinbutton.setFixedSize(150,150)
        unpinbutton.clicked.connect(unpinbuttonClick)
        navbutton = QPushButton( "nav") 
	navbutton.setFixedSize(150,150)
        navbutton.clicked.connect(navbuttonClick) 
	hlayout2.addWidget(pinbutton)
        hlayout2.addWidget(unpinbutton)
        hlayout2.addWidget(navbutton)



        vlayout.addLayout(hlayout )
        vlayout.addLayout( hlayout1 )
        vlayout.addLayout( hlayout2 )

        myviz.setLayout( vlayout)
        myviz.show()
        app.exec_()



	'''try:
		print msg
		print vels(speed,turn)
		while(1):
			key = getKey()
			if key in moveBindings.keys():
				x = moveBindings[key][0]
				y = moveBindings[key][1]
				z = moveBindings[key][2]
				th = moveBindings[key][3]
			elif key in speedBindings.keys():
				speed = speed * speedBindings[key][0]
				turn = turn * speedBindings[key][1]

				print vels(speed,turn)
				if (status == 14):
					print msg
				status = (status + 1) % 15
			else:
				x = 0
				y = 0
				z = 0
				th = 0
				if (key == '\x03'):
					break

			twist = Twist()
			twist.linear.x = x*speed; twist.linear.y = y*speed; twist.linear.z = z*speed;
			twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = th*turn
			pub.publish(twist)

	except:
		print e

	finally:
		twist = Twist()
		twist.linear.x = 0; twist.linear.y = 0; twist.linear.z = 0
		twist.angular.x = 0; twist.angular.y = 0; twist.angular.z = 0
		pub.publish(twist)

    		termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)'''


