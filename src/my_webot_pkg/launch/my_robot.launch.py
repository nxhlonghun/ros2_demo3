from launch import LaunchDescription
from webots_ros2_driver.webots_launcher import WebotsLauncher
from launch_ros.actions import Node
from webots_ros2_driver.webots_controller import WebotsController
from ament_index_python.packages import get_package_share_directory
from webots_ros2_driver.urdf_spawner import URDFSpawner
import os


def generate_launch_description():

    package_dir1 = get_package_share_directory('my_webot_pkg')
    world_path = os.path.join(package_dir1, 'worlds', 'demo.wbt')
    urdf_path = os.path.join(package_dir1, 'urdf', 'text.urdf')
    ros2_controller_path = os.path.join(package_dir1, 'resource', 'ros2_control_config.yaml')

    webots = WebotsLauncher(
        world=world_path,
        ros2_supervisor=True
    )
    spawn_robot = URDFSpawner(
        name='Text',  # 必须与 Webots 中的 Robot name 一致
        urdf_path=urdf_path,
        translation='0 0 0.01', # 初始生成位置
        rotation='0 0 1 0',    # 初始旋转
    )

    robots_controller = WebotsController(
        robot_name = 'Text',
        parameters = [
            ros2_controller_path,
            {
            'robot_description':urdf_path,
            'relative_path_prefix': package_dir1,
        }],
    )
    joint_state_broadcaster_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['joint_state_broadcaster'],
        output='screen',
    )

    # 5. 自动激活 position_controller (用于控制关节移动)
    position_controller_spawner = Node(
        package='controller_manager',
        executable='spawner',
        arguments=['arm_trajectory_controller'],
        output='screen',
    )

    #controller_manager_timeout = ['--controller-manager-timeout', '50']

    return LaunchDescription([
        webots,
        webots._supervisor, # 启动 supervisor 节点
        spawn_robot,        # 执行生成动作
        robots_controller,
        
        joint_state_broadcaster_spawner,
        position_controller_spawner
    ])