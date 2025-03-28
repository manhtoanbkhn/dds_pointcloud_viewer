#pragma once

#include "dds/DDSSubscriber.hpp"
#include "dds/DDSListener.hpp"
#include <PointcloudIDL.hpp>
#include <Slam3DIDL.hpp>
#include <sensor_msgs/msg/point_cloud2.hpp>

#include <mutex>
#include <rclcpp/rclcpp.hpp>
#include <thread>


class LidarMapSubscriber {

public:
  LidarMapSubscriber(rclcpp::Node::SharedPtr node);
  ~LidarMapSubscriber();

  void create_client();
  void create_submap_list_subscriber();
  void create_submap_data_subscriber();
  void create_keyframe_subscriber();
  void create_lidar_pose_subscriber();

  void receiving_loop();

private:
  std::unique_ptr<DDSSubscriber<PointCloudData::PointCloud2>> pointcloud_subscriber_;
  std::unique_ptr<DDSSubscriber<Slam3D::SubmapList>> submap_list_subscriber_;
  std::unique_ptr<DDSSubscriber<Slam3D::SubmapData>> submap_data_subscriber_;
  std::unique_ptr<DDSSubscriber<Slam3D::Keyframe>> keyframe_subscriber_;
  std::unique_ptr<DDSSubscriber<Common::Pose3DTimestamped>> lidar_pose_subscriber_;

  rclcpp::Node::SharedPtr node_;

  rclcpp::Publisher<sensor_msgs::msg::PointCloud2>::SharedPtr pointcloud2_pub_;

  std::shared_ptr<std::thread> receiving_thread_;
  bool running_{false};

  // worker thread
  void invoke(const std::function<void()> &task);
  std::mutex invoke_queue_mutex;
  std::vector<std::function<void()>> invoke_queue;
  uint32_t domain_id_;
  dds::topic::qos::TopicQos tqos_;
  dds::sub::qos::SubscriberQos pqos_;
  dds::sub::qos::DataReaderQos wqos_;
};
