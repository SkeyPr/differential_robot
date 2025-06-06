cmake_minimum_required(VERSION 3.8)
project(bot_control)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(std_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(sensor_msgs REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  set(ament_cmake_copyright_FOUND TRUE)
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

add_executable(reading_laser 
    scripts/reading_laser.cpp
)
ament_target_dependencies(reading_laser
    rclcpp
    sensor_msgs
)

# Install the executable
install(TARGETS reading_laser
    DESTINATION lib/${PROJECT_NAME}
)

# Install Python scripts if you have any
install(DIRECTORY scripts/
    DESTINATION lib/${PROJECT_NAME}
    PATTERN "*.py"
)

install(
  DIRECTORY 
  launch
  DESTINATION share/${PROJECT_NAME}/
)

ament_package()
