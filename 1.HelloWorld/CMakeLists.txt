project(HELLO_ICUB)

find_package(YARP REQUIRED)

add_executable(hello main.cpp)

# we now add the YARP libraries to our project.
target_link_libraries(hello ${YARP_LIBRARIES})
