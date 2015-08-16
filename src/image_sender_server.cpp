#include "ros/ros.h"
#include "image_sender/SendImage.h"
#include <sstream>
#include "sensor_msgs/Image.h"
#include <ros/callback_queue.h>
// This node subscribes to images and sends image to client if service is called

class ImgSender{

public:
	ImgSender():ch_(n_,"server"),
	ih_(n_,"image_saver"){
		ch_.setCallbackQueue(&server_queue_);
		ih_.setCallbackQueue(&image_queue_);
		advertiseimgservice();
		subscribetoimage();
	}

	void subscribetoimage(){
		img_sub_ = ih_.subscribe("/mv_25000060/image_raw",1,&ImgSender::imgsaver,this);

	}

	void advertiseimgservice()
	{
		server_=ch_.advertiseService("image_publisher", &ImgSender::sendimg,this);
	}

	void imgsaver(const sensor_msgs::ImageConstPtr& msg){
	img_=*msg;
		ROS_INFO_STREAM("See something");
	}

	void EmtpyImgQueue()
	  {
		  image_queue_.callOne();
	  }

	void EmtpyServerQueue()
		  {
			  server_queue_.callOne();
		  }

	bool sendimg(image_sender::SendImage::Request &req,
			image_sender::SendImage::Response &res) {
		// Get latest image from queue
		EmtpyImgQueue();
		std::string topic_name;
		return true;
	}

private:
	ros::NodeHandle n_;
	ros::NodeHandle ch_;
	ros::NodeHandle ih_;
	ros::Subscriber img_sub_;
	ros::ServiceServer server_;
	std::string img_topic_;
	sensor_msgs::Image img_;
	ros::CallbackQueue server_queue_;
	ros::CallbackQueue image_queue_;

};


int main(int argc, char **argv)
{
	ros::init(argc, argv, "Image_sender_server");
	ImgSender sender;
	ros::Rate R(30);
	while(ros::ok()){
		sender.EmtpyServerQueue();
		R.sleep();
	}

	return 0;
}
