import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription
from launch.launch_description_sources import PythonLaunchDescriptionSource

def generate_launch_description():
    # Get package directory
    pkg_bot_description = get_package_share_directory('bot_description')
    
    # Include all three launch files
    rviz_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_bot_description, 'launch', 'rviz.launch.py')
    ))
    
    spawn_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_bot_description, 'launch', 'spawn.launch.py')
    ))
    
    controller_launch = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_bot_description, 'launch', 'controller.launch.py')
    ))

    return LaunchDescription([
        rviz_launch,
        spawn_launch,
        controller_launch
    ])