#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/laser_scan.hpp"

class LaserProcessor : public rclcpp::Node {
public:
    LaserProcessor() : Node("laser_processor") {
        // Parameters for FOV filtering
        this->declare_parameter("min_angle", 0.0);      // 0 degrees
        this->declare_parameter("max_angle", 2.0944);   // 120 degrees (2.0944 radians)

        // Subscription to original scan
        subscription_ = this->create_subscription<sensor_msgs::msg::LaserScan>(
            "/scan", 10,
            [this](const sensor_msgs::msg::LaserScan::SharedPtr msg) {
                this->laser_callback(msg);
            });

        // Publisher for filtered scan
        publisher_ = this->create_publisher<sensor_msgs::msg::LaserScan>(
            "/filtered_scan", 10);

        RCLCPP_INFO(this->get_logger(), 
            "Laser processor node started. Filtering to 0-120° FOV.");
    }

private:
    void laser_callback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
        // Get parameters
        double min_angle = this->get_parameter("min_angle").as_double();
        double max_angle = this->get_parameter("max_angle").as_double();

        // Create new message for filtered scan
        auto filtered_msg = std::make_shared<sensor_msgs::msg::LaserScan>(*msg);
        
        // Calculate indices for the angle range
        size_t min_index = static_cast<size_t>(
            (min_angle - msg->angle_min) / msg->angle_increment);
        size_t max_index = static_cast<size_t>(
            (max_angle - msg->angle_min) / msg->angle_increment);

        // Validate indices
        min_index = std::max(min_index, size_t(0));
        max_index = std::min(max_index, msg->ranges.size() - 1);

        // Filter the ranges and intensities
        filtered_msg->ranges = std::vector<float>(
            msg->ranges.begin() + min_index,
            msg->ranges.begin() + max_index + 1);

        if (!msg->intensities.empty()) {
            filtered_msg->intensities = std::vector<float>(
                msg->intensities.begin() + min_index,
                msg->intensities.begin() + max_index + 1);
        }

        // Update message metadata
        filtered_msg->angle_min = min_angle;
        filtered_msg->angle_max = max_angle;
        filtered_msg->header.stamp = this->now();

        // Publish filtered scan
        publisher_->publish(*filtered_msg);

        // Log front distance (middle of filtered range)
        size_t front_index = filtered_msg->ranges.size() / 2;
        RCLCPP_INFO_THROTTLE(
            this->get_logger(),
            *this->get_clock(),
            1000,
            "Filtered scan: %zu ranges (%.1f-%.1f°). Front distance: %.2f m",
            filtered_msg->ranges.size(),
            min_angle * 180.0 / M_PI,
            max_angle * 180.0 / M_PI,
            filtered_msg->ranges[front_index]
        );
    }

    rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr subscription_;
    rclcpp::Publisher<sensor_msgs::msg::LaserScan>::SharedPtr publisher_;
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<LaserProcessor>());
    rclcpp::shutdown();
    return 0;
}