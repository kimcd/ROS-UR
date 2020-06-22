/* ur5_collision_srv 

This service checks for UR5 collision

This code borrows from planning_scene_tutorial.cpp 

TODO: turn into a service 
TODO: report results of collision to a topic as a boolean 
TODO: remove dependence on ur5_moveit_config demo.launch
TODO: fix server warning about unloading library  

C. KIM, JHUAPL 21June2020
*/

#include <ros/ros.h>
#include <vector>
#include "ur5_collision_srv/ur5CollisionSrv.h"

#include <moveit/robot_model/robot_model.h>
#include <moveit/robot_state/robot_state.h>
#include <moveit/robot_model_loader/robot_model_loader.h> // what do?
#include <moveit/planning_interface/planning_interface.h> // what do?
#include <moveit/planning_scene/planning_scene.h>
#include <moveit/kinematic_constraints/utils.h>
#include <moveit_msgs/PlanningScene.h>
#include <moveit_visual_tools/moveit_visual_tools.h>
#include <moveit/collision_detection_fcl/collision_world_fcl.h>
#include <moveit/collision_detection_fcl/collision_robot_fcl.h>
#include <moveit/collision_detection/collision_tools.h>

planning_scene::PlanningScene* g_planning_scene = 0; // null pointer

bool ur5_collision_callback(ur5_collision_srv::ur5CollisionSrv::Request  &req,
         ur5_collision_srv::ur5CollisionSrv::Response &res)
{
  ROS_INFO("I heard");

  // PLANNING GROUP
  robot_model_loader::RobotModelLoader robot_model_loader("robot_description");
  robot_model::RobotModelPtr robot_model = robot_model_loader.getModel();
  robot_state::RobotStatePtr robot_state(new robot_state::RobotState(robot_model));
  const std::string PLANNING_GROUP = "manipulator";
  const robot_state::JointModelGroup* joint_model_group = 
    robot_state->getJointModelGroup(PLANNING_GROUP);
  g_planning_scene = new planning_scene::PlanningScene(robot_model);
  g_planning_scene->getCurrentStateNonConst().setToDefaultValues(joint_model_group, "ready");
  //^^ not sure what this is doing  
  collision_detection::CollisionRequest c_req;
  collision_detection::CollisionResult c_res; 
  robot_state::RobotState& current_state = g_planning_scene->getCurrentStateNonConst();
  
  // may not need this vv
  //std::cout << req;  // spits out entire request message
  //std::cout << req.trajectory.joint_trajectory.points.size();  // num data points
  //trajectory_msgs::JointTrajectory traj_data_msg; 
  //traj_data_msg.joint_names = req.trajectory.joint_trajectory.joint_names; 
  //std::cout << traj_data_msg;
  // may no need this ^^
  //ROS_INFO("Hello", req)
  //std::cout << req.trajectory.joint_trajectory.points[1].positions[0]; // works
  
  std::vector<int>::size_type num_data_points = req.trajectory.joint_trajectory.points.size();
  bool self_collision_flag = false;  
  for (unsigned i = 0; i<num_data_points; i++)
  {    
    // STORE JOINT DATA 
    std::vector<double> joint_angle = {req.trajectory.joint_trajectory.points[i].positions[0], 
  	                               req.trajectory.joint_trajectory.points[i].positions[1], 
 				       req.trajectory.joint_trajectory.points[i].positions[2], 
                                       req.trajectory.joint_trajectory.points[i].positions[3], 
                                       req.trajectory.joint_trajectory.points[i].positions[4], 
                                       req.trajectory.joint_trajectory.points[i].positions[5]
                                       };
    // PRINT JOINT DATA  
    
    //std::cout << "Joint Angle Set " << i+1 << ": ["; 
    //for(int j=0; j < joint_angle.size(); j++)
    //{  
    /*  
      std::cout <<joint_angle.at(i); 
      std::cout << "] \n";  
    */

    // COLLISION CHECKING
    const robot_model::JointModelGroup* joint_model_group = 
      current_state.getJointModelGroup(PLANNING_GROUP); 
    current_state.setJointGroupPositions(joint_model_group, joint_angle);
    c_req.contacts = true; 
    c_req.max_contacts = 1000;
    c_res.clear(); 
    g_planning_scene->checkSelfCollision(c_req, c_res);
    if (c_res.collision)  
    // if there's a collision, 1) throw a collision flag 2) return result as true & joint data
    {
      self_collision_flag = true; 
      std::cout << "Joint Angle Set " << i+1 << ":";
      std::cout << "Is in self collision";
      std::cout << c_res.collision << std::endl;

     }  // end if statement
      //std::cout << c_res.collision; 
      //ROS_INFO_STREAM("Current state is " << 
      //  (c_res.collision ? "in" : "not in") << " self collision");
    }  // end for loop

    if (self_collision_flag) 
    {
      std::cout << "there was collision";
      res.result = true; 
    } 
      else
      {
        std::cout << "there was no collision";
        res.result = false;    
      } 
  //return all data points that are in collision

  ROS_INFO("sending back response");
  return true;  // what do here?
}  // end function

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ur5_collision_server");
  ros::NodeHandle n;

  ros::ServiceServer service = n.advertiseService("ur5_collision_check", ur5_collision_callback);
  ROS_INFO("Ready to check for collision.");
  ros::spin();

  return 0;
}
