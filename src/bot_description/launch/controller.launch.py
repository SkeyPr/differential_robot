import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(
            package='bot_description',
            executable='controller',
            name='controller',
            output='screen',
            prefix='xterm -e',
            parameters=[{
                'use_sim_time': True
            }]
        )
    ])