import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch.actions import IncludeLaunchDescription, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.actions import Node

def generate_launch_description():
    # Paths to packages
    pkg_bot_world = get_package_share_directory('bot_world')
    pkg_bot_description = get_package_share_directory('bot_description')
    pkg_gazebo_ros = get_package_share_directory('gazebo_ros')
    world_path = PathJoinSubstitution([pkg_bot_world, 'worlds', 'world1.world'])

    # Launch arguments
    world_arg = DeclareLaunchArgument(
        'world',
        default_value=PathJoinSubstitution([pkg_bot_world, 'worlds', 'world1.world']),
        description='Path to world file'
    )
    
    use_sim_time_arg = DeclareLaunchArgument(
        'use_sim_time',
        default_value='true',
        description='Use simulation time'
    )

    # Gazebo launch
    gazebo = IncludeLaunchDescription(
        PythonLaunchDescriptionSource(
            os.path.join(pkg_gazebo_ros, 'launch', 'gazebo.launch.py')
        ),
        launch_arguments={
            'world': LaunchConfiguration('world'),
            'verbose': 'true',
            'pause': 'false'
        }.items()
    )

    # Robot State Publisher
    robot_state_publisher = Node(
        package='robot_state_publisher',
        executable='robot_state_publisher',
        parameters=[{
            'robot_description': open(os.path.join(pkg_bot_description, 'description', 'bot.urdf')).read(),
            'use_sim_time': LaunchConfiguration('use_sim_time')
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

    return LaunchDescription([
        world_arg,
        use_sim_time_arg,
        gazebo,
        robot_state_publisher,
        spawn_entity
    ])