#include <memory>
#include <vector>
#include "rclcpp/rclcpp.hpp"
#include "trajectory_msgs/msg/joint_trajectory.hpp"
#include "trajectory_msgs/msg/joint_trajectory_point.hpp"
#include <string>
using namespace std;

using namespace std::chrono_literals;

class TrajectoryTestNode : public rclcpp::Node
{
public:
    TrajectoryTestNode(const string &node_name);

private:
    rclcpp::Publisher<trajectory_msgs::msg::JointTrajectory>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    void send_trajectory();
    
};

