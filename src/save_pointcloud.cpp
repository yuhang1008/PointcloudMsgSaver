#include <ros/ros.h>
#include <sensor_msgs/PointCloud.h>
#include <pcl/point_cloud.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>

void pointCloudCallback(const sensor_msgs::PointCloud::ConstPtr& msg, const std::string& output_path)
{
    pcl::PointCloud<pcl::PointXYZ> cloud;
    for (const auto& point : msg->points) {
        cloud.push_back(pcl::PointXYZ(point.x, point.y, point.z));
    }

    pcl::io::savePCDFileASCII(output_path, cloud);
    ROS_INFO("Point cloud saved as %s", output_path.c_str());
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "save_pointcloud_node");
    ros::NodeHandle nh;
    ros::NodeHandle private_nh("~");
    
    std::string topic, output_path;
    private_nh.param<std::string>("topic", topic, "/pointcloud_topic");
    private_nh.param<std::string>("output_path", output_path, "output.pcd");

    ros::Subscriber sub = nh.subscribe<sensor_msgs::PointCloud>(topic, 1, boost::bind(pointCloudCallback, _1, output_path));

    ros::spin();
    return 0;
}
