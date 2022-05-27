project(arm_torque)

find_package(YARP REQUIRED)

add_executable(armTorque arm_joint_impedance.cpp)

# we now add the YARP libraries to our project.
target_link_libraries(armTorque ${YARP_LIBRARIES})