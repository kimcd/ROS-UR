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
