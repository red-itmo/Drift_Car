#!/usr/bin/env python

##   drift_car_driver.py
##   Created on: 03.03.2018
##        Email: Nicko_Dema@protonmail.com
##               ITMO University

##   python2.7

import os, sys

import rospy, tf, tf2_ros

from geometry_msgs.msg import Twist
from std_msgs.msg import String
#from tf2_msgs.msg import TFMessage

dpath = os.path.dirname(os.path.realpath(__file__))
sys.path.append(dpath + "/../include")
print '\n'.join(sys.path)

from drift_car_modules import drift_car_com

sign = lambda a: 1 if a>0 else -1 if a<0 else 0

drift_car_com.seyhello()

def cmd_cb(data):
    if(drift_car_driver.is_open()):
        if (abs(data.linear.x) > 1.0):
            x = 100*sign(data.linear.x)
        else:
            x = data.linear.x*100
        if (abs(data.angular.z) > 1.0):
            z = 100*sign(data.angular.z)
        else:
            z = data.angular.z*100
        drift_car_driver.set_cmd(str(int(x)), str(int(z)))
    else:
        print "[car]: Can't sent cmd | port is down"
#       rospy.loginfo("I heard")#,"%s" data.data)

def init():
    rospy.init_node('driver', anonymous=True)
    rospy.Subscriber("cmd_vel", Twist, cmd_cb)

def loop():
    #pub = rospy.Publisher('tf', Type , queue_size=10)
    pub_ticks = rospy.Publisher('ticks', String, queue_size=10)
    rate = rospy.Rate(100) # 100hz
    while not rospy.is_shutdown():
        if (drift_car_driver.get_enc_data()):
            ticks_str = str(drift_car_driver.ticks_l) + " " \
                      + str(drift_car_driver.ticks_r) + " " \
                      + str(drift_car_driver.time) + " " \
                      + str(drift_car_driver.err)
            pub_ticks.publish(ticks_str)
        #hello_str = "hello world %s" % rospy.get_time
        #rospy.loginfo(hello_str)
        #pub.publish(hello_str)
        rate.sleep()


if __name__ == '__main__':
    init()
    drift_car_driver = drift_car_com.serial_transceiver()
    if (drift_car_driver.open_port()):
        print "open?"
    else:
        print "can't open"

    try:
        loop()
    except rospy.ROSInterruptException:
        pass


# finding the module for serial work
# import os, re, sys
# ppath = os.environ['ROS_PACKAGE_PATH']
# serObj = re.search(r'/home.+/src', ppath, re.I)
# if serObj:
#     mpath = serObj.group() + "/mrp/mrp_driver/include"
#     print mpath
#
# else:
#     print "[mrp_driver]: Can't find the catkin workspace directory!"
#     sys.exit()
