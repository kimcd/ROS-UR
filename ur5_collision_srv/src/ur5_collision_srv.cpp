/* ur5_collision_srv 

This service checks for UR5 collision

This code borrows from planning_scene_tutorial.cpp 

TODO: turn into a service 
TODO: report results of collision to a topic as a boolean 
TODO: remove dependence on ur5_moveit_config demo.launch 

C. KIM, JHUAPL 21June2020
*/

#include <ros/ros.h>
#include <vector>
#include "ur5_collision_srv/ur5CollisionSrv.h"

//if there are any collisions, stop and return a collision response
//return all data points that are in collision

bool ur5_collision_callback(ur5_collision_srv::ur5CollisionSrv::Request  &req,
         ur5_collision_srv::ur5CollisionSrv::Response &res)
{
  ROS_INFO("I heard");

  //may not need this vv
  std::cout << req;  // spits out entire request message
  std::cout << req.trajectory.joint_trajectory.points.size();  // num data points
  trajectory_msgs::JointTrajectory traj_data_msg; 
  traj_data_msg.joint_names = req.trajectory.joint_trajectory.joint_names; 
  std::cout << "What's up bitch" << traj_data_msg;
  // may no need this ^^
  //ROS_INFO("Hello", req)
  //std::cout << req.trajectory.joint_trajectory.points[1].positions[0]; // works

  std::vector<int>::size_type num_data_points = req.trajectory.joint_trajectory.points.size(); 
  for (unsigned i = 0; i<num_data_points; i++)
  {
      std::vector<double> joint_angle = {req.trajectory.joint_trajectory.points[i].positions[0], 
  	                           	 req.trajectory.joint_trajectory.points[i].positions[1], 
 					 req.trajectory.joint_trajectory.points[i].positions[2], 
                                         req.trajectory.joint_trajectory.points[i].positions[3], 
                                         req.trajectory.joint_trajectory.points[i].positions[4], 
                                         req.trajectory.joint_trajectory.points[i].positions[5]
                                         };
      
      std::cout << "Joint Angle Set " << i+1 << ": ["; 
      for(int i=0; i < joint_angle.size(); i++)
        std::cout <<joint_angle.at(i); 
        std::cout << "] \n";  
  }

  ROS_INFO("sending back response");
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ur5_collision_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("ur5_collision_check", ur5_collision_callback);
  ROS_INFO("Ready to check for collision.");
  ros::spin();

  return 0;
}
