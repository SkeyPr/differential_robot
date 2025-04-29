from launch import LaunchDescription
from launch_ros.actions import Node
from launch.actions import DeclareLaunchArgument
from launch.substitutions import LaunchConfiguration, PathJoinSubstitution
from launch_ros.substitutions import FindPackageShare

def generate_launch_description():
    # Arguments
    rviz_config_arg = DeclareLaunchArgument(
        'rviz_config',
        default_value=PathJoinSubstitution([
            FindPackageShare('bot_control'),
            'rviz',
            'laser_visualization.rviz'
        ]),
        description='Path to RViz config file'
    )

    return LaunchDescription([
        rviz_config_arg,
        
        # Your laser processor node
        Node(
            package='bot_control',
            executable='reading_laser',
            name='laser_processor',
            output='screen',
            parameters=[{
                'min_angle': 0.0,      # 0 degrees
                'max_angle': 2.0944,    # 120 degrees (~2.0944 radians)
            }],
            emulate_tty=True,
        ),
        
        # RViz2 node
        Node(
            package='rviz2',
            executable='rviz2',
            name='rviz2',
            output='screen',
            arguments=['-d', LaunchConfiguration('rviz_config')],
        )
    ])