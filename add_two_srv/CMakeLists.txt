cmake_minimum_required(VERSION 2.8.3)
project(add_two_srv)

message("Trace for PROJECT_NAME variable: ${PROJECT_NAME}")

## Find catkin and any catkin packages
find_package(catkin REQUIRED COMPONENTS roscpp rospy std_msgs genmsg)

## Declare ROS messages and services
add_message_files(FILES Num.msg)
add_service_files(FILES AddTwoInts.srv)

## Generate added messages and services
generate_messages(DEPENDENCIES std_msgs)

## Declare a catkin package
catkin_package()

## Build talker and listener
include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})
add_dependencies(talker add_two_srv_generate_messages_cpp)

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})
## This: 
## add_dependencies(target ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
## is equivalent to this:
add_dependencies(listener add_two_srv_generate_messages_cpp)
## In the above line, "add_two_srv_generate_messages_cpp" is a cmake target - it's not a complete package (which is why you won't find it in catkin_ws/devel/lib/) - created by the "generate_mssages" command. The target name is based onhte package name declared with the package() command. 

## Build service client and server
# %Tag(SRVCLIENT)%
add_executable(add_two_ints_server src/add_two_srv.cpp)
target_link_libraries(add_two_ints_server ${catkin_LIBRARIES})
add_dependencies(add_two_ints_server add_two_srv_gencpp)

add_executable(add_two_ints_client src/add_two_client.cpp)
target_link_libraries(add_two_ints_client ${catkin_LIBRARIES})
add_dependencies(add_two_ints_client add_two_srv_gencpp)

# %EndTag(SRVCLIENT)%

# %EndTag(FULLTEXT)%
