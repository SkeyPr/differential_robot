#include <rclcpp/rclcpp.hpp>
#include <gazebo_msgs/srv/spawn_entity.hpp>
#include <fstream>
#include <sstream>

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    auto node = rclcpp::Node::make_shared("spawn_bot_node");

    auto client = node->create_client<gazebo_msgs::srv::SpawnEntity>("/spawn_entity");

    // Wait for service to be available
    while (!client->wait_for_service(std::chrono::seconds(1))) {
        RCLCPP_INFO(node->get_logger(), "Waiting for /spawn_entity service...");
    }

    // Read URDF file
    std::ifstream urdf_file("/home/your_user/ros2_ws/src/bot_description/urdf/bot.urdf");
    std::stringstream urdf_ss;
    urdf_ss << urdf_file.rdbuf();

    auto request = std::make_shared<gazebo_msgs::srv::SpawnEntity::Request>();
    request->name = "bot";
    request->xml = urdf_ss.str();
    request->robot_namespace = "bot_namespace";
    request->initial_pose.position.x = 0.0;
    request->initial_pose.position.y = 0.0;
    request->initial_pose.position.z = 0.1;
    request->reference_frame = "world";

    auto result = client->async_send_request(request);
    if (rclcpp::spin_until_future_complete(node, result) == rclcpp::FutureReturnCode::SUCCESS) {
        RCLCPP_INFO(node->get_logger(), "Spawn request sent successfully");
    } else {
        RCLCPP_ERROR(node->get_logger(), "Failed to call service /spawn_entity");
    }

    rclcpp::shutdown();
    return 0;
}
