#ifndef _NEATNESS_ESTIMATOR_DATA_CHECKER_H_
#define _NEATNESS_ESTIMATOR_DATA_CHECKER_H_

#include <ros/ros.h>
#include <nodelet/nodelet.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>

#include <jsk_recognition_msgs/BoundingBoxArray.h>
#include <sensor_msgs/PointCloud2.h>
#include <sensor_msgs/Image.h>
#include <std_msgs/Header.h>
#include <std_srvs/SetBool.h>

#include <pcl_conversions/pcl_conversions.h>
#include <pcl_msgs/PointIndices.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/io/io.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv/cv.hpp>

#include <mutex>
#include <boost/filesystem.hpp>
#include <yaml-cpp/yaml.h>

namespace neatness_estimator
{
  class DataChecker : public nodelet::Nodelet
  {
  public:

    typedef message_filters::sync_policies::ExactTime<
      jsk_recognition_msgs::BoundingBoxArray,
      sensor_msgs::PointCloud2,
      sensor_msgs::Image
      > SyncPolicy;

    typedef message_filters::sync_policies::ApproximateTime<
      jsk_recognition_msgs::BoundingBoxArray,
      sensor_msgs::PointCloud2,
      sensor_msgs::Image
      > ApproximateSyncPolicy;

  protected:

    // functions

    virtual void onInit();

    virtual void callback(const jsk_recognition_msgs::BoundingBoxArray::ConstPtr& boxes_msg,
                          const sensor_msgs::PointCloud2::ConstPtr& cloud_msg,
                          const sensor_msgs::Image::ConstPtr& image_msg);

    virtual bool service_callback(std_srvs::SetBool::Request& req,
                                  std_srvs::SetBool::Response& res);

    virtual bool get_read_dir(std::string& target_dir);

    virtual bool read_pcd(std::string path);

    virtual bool read_image(std::string path);

    virtual bool read_boxes(std::string path);

    virtual void read_files(std_srvs::SetBool::Response& res);

    // variables

    ros::NodeHandle nh_;

    ros::NodeHandle pnh_;

    ros::ServiceServer server_;

    boost::shared_ptr<message_filters::Synchronizer<SyncPolicy> >sync_;

    boost::shared_ptr<message_filters::Synchronizer<ApproximateSyncPolicy> > async_;

    message_filters::Subscriber<jsk_recognition_msgs::BoundingBoxArray> sub_boxes_;

    message_filters::Subscriber<sensor_msgs::PointCloud2> sub_point_cloud_;

    message_filters::Subscriber<sensor_msgs::Image> sub_image_;


    pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud_;

    cv::Mat image_;

    jsk_recognition_msgs::BoundingBoxArray::ConstPtr boxes_;


    pcl::PointCloud<pcl::PointXYZRGB>::Ptr prev_cloud_;

    cv::Mat prev_image_;

    jsk_recognition_msgs::BoundingBoxArray::Ptr prev_boxes_;


    boost::mutex mutex_;

    std::string prefix_ = "./";

    std_msgs::Header header_;

  private:

  };

} // namespace neatness_estimator

#endif
