import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch_ros.actions import Node

def generate_launch_description():
    # Get package directories
    pkg_bot_description = get_package_share_directory('bot_description')
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')

    # URDF file path
    urdf_file = os.path.join(pkg_bot_description, 'description', 'bot.urdf')
    
    # Read URDF contents
    with open(urdf_file, 'r') as file:
        robot_description = file.read()

    # Gazebo launch - using direct path from package share
    gazebo_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gazebo.launch.py')
        ),
        launch_arguments={
            'verbose': 'false',
            'pause': 'false',
            'world': 'empty.world'
        }.items()
    )

    # Robot State Publisher
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{
            'robot_description': robot_description,
            'use_sim_time': True
        }]
    )

    # Spawn Robot
    spawn_entity = Node(
        package='gazebo_ros',
        executable='spawn_entity.py',
        arguments=[
            '-entity', 'bot',
            '-topic', 'robot_description',
            '-x', '0.0',
            '-y', '0.0',
            '-z', '0.1'
        ],
        output='screen'
    )

    diffdrive = Node(
        package='bot_description',
        executable='diffdrive',
        name='diffdrive',
        output='screen'
    )

    return LaunchDescription([
        gazebo_launch,
        robot_state_publisher,
        spawn_entity
    ])