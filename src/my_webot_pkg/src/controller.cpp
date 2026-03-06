#include "my_webot_pkg/controller.hpp"

TrajectoryTestNode::TrajectoryTestNode(const string &node_name) : Node(node_name)
{
    publisher_ = this->create_publisher<trajectory_msgs::msg::JointTrajectory>(
        "/arm_trajectory_controller/joint_trajectory", 10);

    // 延迟 1 秒后发送指令，确保控制器已就绪
    timer_ = this->create_wall_timer(6s, std::bind(&TrajectoryTestNode::send_trajectory, this));
}

void TrajectoryTestNode::send_trajectory()
{
    // timer_->cancel(); // 只发送一次

    auto message = trajectory_msgs::msg::JointTrajectory();

    // 1. 指定关节名称（必须与 YAML 一致）
    message.joint_names = {"middle_joint", "arm_joint", "hand_joint"};

    // 2. 创建第一个轨迹点：2秒后到达位置 A
    trajectory_msgs::msg::JointTrajectoryPoint p1;
    p1.positions = {0.5, -0.5, 0.2};
    p1.time_from_start = rclcpp::Duration(2s);
    message.points.push_back(p1);

    // 3. 创建第二个轨迹点：4秒后到达位置 B
    trajectory_msgs::msg::JointTrajectoryPoint p2;
    p2.positions = {-0.5, 0.5, -0.2};
    p2.time_from_start = rclcpp::Duration(4s);
    message.points.push_back(p2);

    trajectory_msgs::msg::JointTrajectoryPoint p3;
    p3.positions = {0.0, 0.0, 0.0};
    p3.time_from_start = rclcpp::Duration(6s);
    message.points.push_back(p3);

    RCLCPP_INFO(this->get_logger(), "正在发布平滑轨迹...");
    publisher_->publish(message);
}

int main(int argc, char *argv[])
{
    rclcpp::init(argc, argv);
    auto node = make_shared<TrajectoryTestNode>("trajectory_test_node");
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}