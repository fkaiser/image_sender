#include "ros/ros.h"
#include "image_sender/SendImage.h"
#include <sstream>
#include "sensor_msgs/Image.h"
#include <ros/callback_queue.h>
// This node sends service request frequently to get image at fixed frequency

class ImgSenderClient{

public:
	ImgSenderClient(): n_("/request_client"){
		client_ = n_.serviceClient<image_sender::SendImage>("/server/image_publisher");
		img_pub_=n_.advertise<sensor_msgs::Image>("rate_image", 1,this);
	}
	void ImgRequest()
	  {
		srv_.request.im_name="test";
		  if(client_.call(srv_)){
			  ROS_INFO_STREAM("Got image");
			  img_pub_.publish(srv_.response.img);
		  }
		  else
		  {
			  ROS_INFO_STREAM("Got no image");
		  }
	  }

private:
	ros::NodeHandle n_;
	ros::ServiceClient client_;
	image_sender::SendImage srv_;
	ros::Publisher img_pub_;
};


int main(int argc, char **argv)
{
	ros::init(argc, argv, "Image_sender_server");
	ImgSenderClient img_client;
	ros::Rate R(10);
	while(ros::ok()){
		img_client.ImgRequest();
		R.sleep();
	}

	return 0;
}
