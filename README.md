# ROS 2 Mobile Robot Simulation Stack

A complete ROS 2 (Humble/Iron) simulation stack for a mobile robot with LIDAR and RGB camera, featuring URDF modeling, Gazebo worlds, and sensor data processing.

## 📦 Packages Overview

| Package | Description |
|---------|-------------|
| [`bot_description`](#bot_description) | Robot URDF, controllers, and keyboard teleop |
| [`bot_world`](#bot_world) | Gazebo worlds and robot spawning |
| [`bot_control`](#bot_control) | LIDAR data processing and visualization |

---

## 🚀 Quick Start
```bash
# Clone and build
mkdir -p ~/ros2_ws/src
cd ~/ros2_ws/src
git clone https://github.com/your_username/your_repo.git
cd ..
colcon build --symlink-install
source install/setup.bash

# Launch in Gazebo (example)
ros2 launch bot_world spawn_bot.launch.py

bot_description/
├── urdf/               # Robot URDF/xacro files
│   ├── robot.urdf.xacro
│   ├── sensors/        # LIDAR and camera definitions
├── launch/
│   ├── spawn_robot.launch.py
│   ├── display.launch.py
├── src/
│   ├── keyboard_teleop.cpp  # Teleoperation node

# Visualize in RViz
ros2 launch bot_description view_bot.launch.py

# Spawn in Gazebo (standalone)
ros2 launch bot_description spawn.launch.py

Customization

Add new worlds to:

bot_world/
├── worlds/
│   ├── custom.world     # Your custom Gazebo world
├── models/             # Custom Gazebo models

bot_control

Sensor Processing and Visualization
Features

    LIDAR scan filtering (0-120° FOV)

    RViz configurations for sensor visualization

    Camera image processing (future)

Nodes
Node	Topic Subscriptions	Publications
laser_processor	/scan	/filtered_scan
image_processor	/camera/image_raw	/processed_image
Visualization
bash

# Launch LIDAR visualization
ros2 launch bot_control laser_visualization.launch.py
