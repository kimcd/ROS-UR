/* 
UR5_MOTION_VIZ node subscribes to /traj_data. The incoming trajectory
is checked for self-collision on the UR5. The output is a ROS_INFO message 
indicating whether a particular set of joint data is in self-collision or not.

This code borrows heavily from planning_scene_tutorial.cpp 

TODO: 
-turn into a service 
-report results of collision to a topic as a boolean 
-remove dependence on ur5_moveit_config demo.launch 

C. KIM, JHUAPL 14June2020
*/

#include "ros/ros.h"
#include "ur5_collision_srv/ur5CollisionSrv.h"

bool ur5_collision_callback(ur5_collision_srv::ur5CollisionSrv::Request  &req,
         ur5_collision_srv::ur5CollisionSrv::Response &res)
{
  ROS_INFO("I heard");
  std::cout << req;
  //ROS_INFO("Hello", req)
  ROS_INFO("sending back response");
  return true;
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ur5_collision_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("ur5_collision_check", ur5_collision_callback);
  ROS_INFO("Ready to add two ints.");
  ros::spin();

  return 0;
}
