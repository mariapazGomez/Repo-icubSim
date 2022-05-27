project(arm_control)

find_package(YARP REQUIRED)

add_executable(armControl tutorial_arm.cpp)

# we now add the YARP libraries to our project.
target_link_libraries(armControl ${YARP_LIBRARIES})
