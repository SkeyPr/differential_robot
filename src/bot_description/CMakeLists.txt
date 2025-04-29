cmake_minimum_required(VERSION 3.8)
project(bot_description)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Find ROS 2 packages
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

# Find ncurses for keyboard control
find_package(PkgConfig REQUIRED)
pkg_check_modules(NCURSES REQUIRED ncurses)

# Add your executables
add_executable(controller src/controller.cpp)
target_include_directories(controller PRIVATE ${NCURSES_INCLUDE_DIRS})
target_link_libraries(controller ${NCURSES_LIBRARIES})
ament_target_dependencies(controller 
  rclcpp 
  geometry_msgs
)

add_executable(diffdrive src/diffdrive.cpp)
ament_target_dependencies(diffdrive 
  rclcpp 
  geometry_msgs
)

# Installation
install(TARGETS
  controller
  diffdrive
  DESTINATION lib/${PROJECT_NAME}
)

install(
  DIRECTORY 
  launch 
  rviz
  description
  DESTINATION share/${PROJECT_NAME}/
  PATTERN "*.py" PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

ament_package()