#! /usr/bin/env python

"""ur5_collision_client

This function accepts a commandline argument for a filename and 
initializes a ROS client that will parse .csv trajectory data 
and send to the service server as a moveit_msgs/RobotTrajectory type. 

This file contains the following functions: 

    *has_header - returns a bool with TRUE when the .csv trajectory file
		  has a header and FALSE when it does not. This allows 
		  read_goal_traj to extract the appropriate columns 

    *read_goal_traj - uses csv reader to extract trajectory data from the 
		      .csv file and return as variables traj_t and traj_q

    *robot_traj_generate - uses traj_t and traj_q to generate a 
			   moveit.msgs/RobotTrajectory message and return 
   			   it as the variable path

    *main - the main function

TODO: add functionality to search for .csv files in a user-specified path
      rather than the folder the user is in

C. KIM, JHUAPL 21JUNE2020
"""

import csv
import numpy
import sys
import rospy
import geometry_msgs.msg
from trajectory_msgs.msg import JointTrajectoryPoint
from moveit_msgs.msg import RobotTrajectory
from ur5_collision_srv.srv import *
#from import_trajectory import read_goal_traj
client = None

JOINT_NAMES = ['shoulder_pan_joint', 'shoulder_lift_joint', 'elbow_joint',
               'wrist_1_joint', 'wrist_2_joint', 'wrist_3_joint']

def has_header(csv_file):
    """ check if the file has a header """ 
    sample_bytes = 2000
    header_log = csv.Sniffer().has_header(csv_file.read(sample_bytes))
    csv_file.seek(0)
    # print 'header logic:', header_log
    return header_log 

def read_goal_traj(filename):
    """ parse file and return traj_q and traj_t """
    with open(filename) as csv_file:
        header_log = has_header(csv_file)
        csv_read = csv.reader(csv_file, delimiter=',')
	if header_log: 
	    next(csv_read)
            traj_data = numpy.array(list(csv_read), dtype=float)
            traj_t = traj_data[:,0]
            traj_q = traj_data[:,1:7]
        else: 
            traj_data = numpy.array(list(csv_read), dtype=float)
	    n_col = traj_data.shape[1]
            if n_col == 6: 
	        traj_t = []
 		traj_q = traj_data 
    	    # elif n_col == 7: 
		# traj_q = traj.data[:,1:7]
		# traj_t = traj_data[:,0]
    return traj_t, traj_q

def robot_traj_generate(filename):
    """ generate a message of RobotTrajectory type using read_goal_traj and return as "path" """
    [traj_t, traj_q] = read_goal_traj(filename)
    path = RobotTrajectory()
    path.joint_trajectory.header.frame_id = "/world"
    path.joint_trajectory.joint_names = JOINT_NAMES
    path.joint_trajectory.points = [JointTrajectoryPoint(positions=traj_q[1,:], 
				    velocities=[0]*6, time_from_start=rospy.Duration(0.0))]
    d = .001
    for i in range(traj_q.shape[0]):
         path.joint_trajectory.points.append(JointTrajectoryPoint(positions=traj_q[i,:],
                                             velocities=[0]*6, 
                                             time_from_start=rospy.Duration(d)))
         d += .001  # seconds between each point
    return path

#    rate = rospy.Rate(1)
#    ctrl_c = False
#    connections = pub.get_num_connections()
     
#    while not ctrl_c and not rospy.is_shutdown():
#        print "Number of connections: ", connections 
#        if connections > 0: 
#            pub.publish(path) 
#            print "Message Published. Closing Program..."
#            ctrl_c = True
#        else: 
#            rate.sleep()
# catches a rospy.ROSInterruptExcept exception, whihc can be thrown by rospy.sleep() and rospy.Rate.sleep() methods when Ctrl-C is pressed or your Node is otherwise shutdown. The reason this exception is raised is so that you don't accidentally contiue executing code after the sleep().

def ur5_collision_client(filename):
    path = robot_traj_generate(filename) 
    rospy.wait_for_service('ur5_collision_check')
    try:
        ur5_collision_check = rospy.ServiceProxy('ur5_collision_check', ur5CollisionSrv)
        resp = ur5_collision_check(path)
        return resp.result
	print(" ")
        print("Path has been sent to collision check service.")
	print(" ")
	#resp1 = ur5_collision_check
	#return resp1.result
    except rospy.ServiceException: 
	print ("Service call failed: %s"%e)
    
def usage(): 
    return "Enter Valid .csv File."

if __name__=='__main__':
    if len(sys.argv) == 2:
        filename = sys.argv[1]
    else:
	print(usage())
	sys.exit(1)
    print(" ")
    print "Requesting collision check on:", filename
    if ur5_collision_client(filename): 
        print("This trajectory contains self-collision.")
    else: 
	print("This trajectory does not contain self-collision.")
    
