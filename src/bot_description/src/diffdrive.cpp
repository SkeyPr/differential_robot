#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>

class DiffDriveController : public rclcpp::Node {
public:
    DiffDriveController() : Node("diff_drive_controller") {
        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&DiffDriveController::controlLoop, this));
            
        RCLCPP_INFO(this->get_logger(), "Differential Drive Controller started");
    }

private:
    void controlLoop() {
        // Replace this with your control logic
        auto msg = geometry_msgs::msg::Twist();
        msg.linear.x = 0.2;  // Move forward at 0.2 m/s
        msg.angular.z = 0.1;  // Rotate at 0.1 rad/s
        
        cmd_vel_pub_->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<DiffDriveController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}