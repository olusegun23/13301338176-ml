#!/usr/bin/env python
import roslib; roslib.load_manifest('robot_status')
#import roslib
import rospy
#from PyQt4.QtCore import *  
#from PyQt4.QtGui import * 

#from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from geometry_msgs.msg import *
from std_msgs.msg import String
import rospy
from std_msgs.msg import String



"""
Module implementing Form.
"""

from PyQt4.QtGui import QWidget
from PyQt4.QtCore import pyqtSignature
from Ui_status import Ui_Form

import PyQt4, PyQt4.QtGui, sys  
import global_vars




class Form(QWidget, Ui_Form):
    """
    Class documentation goes here.
    """
    
    def cpucallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.cpulabel.setText(str(data.data))
    def memcallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.memlabel.setText(str(data.data))
    def diskcallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.disklabel.setText(str(data.data))
    def localizationcallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.localizationlabel.setText("localization "+str(data.data))
    def voxelmapcallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.voxmaplabel.setText("voxmap "+str(data.data))
    def movebasecallback(self, data):
        rospy.loginfo(rospy.get_caller_id() + "I heard ::::%s", data.data)
        self.movebaselabel.setText("movebase "+str(data.data))   
    
    
    
    def __init__(self, parent = None):
        """
        Constructor
        """
        rospy.init_node('status')
        rospy.Subscriber('cpu',String, self.cpucallback)
        rospy.Subscriber('mem',String, self.memcallback)
        rospy.Subscriber('disk',String, self.diskcallback)
        rospy.Subscriber('localization_status',String, self.localizationcallback)
        rospy.Subscriber('voxel_map_status',String, self.voxelmapcallback)
        rospy.Subscriber('movebase_status',String, self.movebasecallback)
        
        QWidget.__init__(self, parent)
        self.setupUi(self)
        
          
    
    
    
    
    @pyqtSignature("")
    def on_pushButton_clicked(self):
        """
        Slot documentation goes here.
        """
        # TODO: not implemented yet
        raise NotImplementedError


if __name__ == "__main__":   
   
     
    app = PyQt4.QtGui.QApplication(sys.argv)     
    dlg = Form()     
    dlg.show()     
    sys.exit(app.exec_())   
