#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <ncurses.h>
#include <cstdlib>

class WASDController : public rclcpp::Node {
public:
    WASDController() : Node("wasd_controller") {
        // Initialize publisher
        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);
        
        // Initialize ncurses for keyboard input
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        nodelay(stdscr, TRUE);
        
        RCLCPP_INFO(this->get_logger(), "WASD Controller Initialized");
        RCLCPP_INFO(this->get_logger(), "Press 'Q' to quit");
        
        // Create timer for control loop
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(100),
            std::bind(&WASDController::controlLoop, this));
    }
    
    ~WASDController() {
        endwin(); // Clean up ncurses
    }

private:
    void controlLoop() {
        auto msg = geometry_msgs::msg::Twist();
        int ch = getch();
        
        // Handle keypress
        switch(ch) {
            case 'w': case 'W':
                msg.linear.x = 0.5;  // Forward
                break;
            case 's': case 'S':
                msg.linear.x = -0.5; // Backward
                break;
            case 'a': case 'A':
                msg.angular.z = 1.0; // Left turn
                break;
            case 'd': case 'D':
                msg.angular.z = -1.0; // Right turn
                break;
            case 'q': case 'Q':
                RCLCPP_INFO(this->get_logger(), "Shutting down...");
                rclcpp::shutdown();
                exit(0);
                break;
            default:
                // Stop if no key pressed
                msg.linear.x = 0.0;
                msg.angular.z = 0.0;
        }
        
        cmd_vel_pub_->publish(msg);
    }

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    auto node = std::make_shared<WASDController>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}