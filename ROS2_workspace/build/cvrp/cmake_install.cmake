# Install script for directory: /home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/src/cvrp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/install/cvrp")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp")
    file(RPATH_CHECK
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp"
         RPATH "")
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cvrp" TYPE EXECUTABLE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/cvrp")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp")
    file(RPATH_CHANGE
         FILE "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp"
         OLD_RPATH "/home/steags/ros2_eloquent/install/rclcpp/lib:/home/steags/ros2_eloquent/install/rcl/lib:/home/steags/ros2_eloquent/install/rcl_interfaces/lib:/home/steags/ros2_eloquent/install/rmw_implementation/lib:/home/steags/ros2_eloquent/install/rmw/lib:/home/steags/ros2_eloquent/install/rcutils/lib:/home/steags/ros2_eloquent/install/rcl_logging_spdlog/lib:/home/steags/ros2_eloquent/install/rcpputils/lib:/home/steags/ros2_eloquent/install/rosgraph_msgs/lib:/home/steags/ros2_eloquent/install/rcl_yaml_param_parser/lib:/home/steags/ros2_eloquent/install/tracetools/lib:/home/steags/ros2_eloquent/install/geometry_msgs/lib:/home/steags/ros2_eloquent/install/builtin_interfaces/lib:/home/steags/ros2_eloquent/install/std_msgs/lib:/home/steags/ros2_eloquent/install/rosidl_typesupport_cpp/lib:/home/steags/ros2_eloquent/install/rosidl_typesupport_c/lib:/home/steags/ros2_eloquent/install/rosidl_typesupport_introspection_cpp/lib:/home/steags/ros2_eloquent/install/rosidl_typesupport_introspection_c/lib:/home/steags/ros2_eloquent/install/rosidl_generator_c/lib:/home/steags/ros2_eloquent/install/visualization_msgs/lib:"
         NEW_RPATH "")
    if(CMAKE_INSTALL_DO_STRIP)
      execute_process(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cvrp/cvrp")
    endif()
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/package_run_dependencies" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_index/share/ament_index/resource_index/package_run_dependencies/cvrp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/parent_prefix_path" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_index/share/ament_index/resource_index/parent_prefix_path/cvrp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp/environment" TYPE FILE FILES "/home/steags/ros2_eloquent/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/ament_prefix_path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp/environment" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/ament_prefix_path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp/environment" TYPE FILE FILES "/home/steags/ros2_eloquent/install/ament_cmake_core/share/ament_cmake_core/cmake/environment_hooks/environment/path.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp/environment" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/path.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/local_setup.bash")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/local_setup.sh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/local_setup.zsh")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/local_setup.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_environment_hooks/package.dsv")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/ament_index/resource_index/packages" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_index/share/ament_index/resource_index/packages/cvrp")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp/cmake" TYPE FILE FILES
    "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_core/cvrpConfig.cmake"
    "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/ament_cmake_core/cvrpConfig-version.cmake"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/share/cvrp" TYPE FILE FILES "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/src/cvrp/package.xml")
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/steags/Documents/Jussieu/M2/MAOA/PDI_Project/ROS2_workspace/build/cvrp/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
