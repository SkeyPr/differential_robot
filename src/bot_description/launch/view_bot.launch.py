import os
from ament_index_python.packages import get_package_share_path
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    # Get the package share directory as a Path object
    pkg_share_path = get_package_share_path('bot_description')
    
    # Path to your URDF file (using pathlib)
    urdf_path = pkg_share_path / 'description' / 'bot.urdf'
    
    # Verify the file exists
    if not urdf_path.exists():
        raise FileNotFoundError(f"URDF file not found at {urdf_path}")
    
    # Read the URDF file
    with open(urdf_path, 'r') as file:
        robot_description = file.read()
    
    # Robot State Publisher Node
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{'robot_description': robot_description}]
    )
    
    # RViz Node
    rviz_node = Node(
        package='rviz2',
        executable='rviz2',
        arguments=['-d', str(pkg_share_path / 'rviz' / 'bot.rviz')]
    )
    
    return LaunchDescription([
        robot_state_publisher,
        rviz_node
    ])