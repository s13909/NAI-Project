
#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

//default capture width and height
const int FRAME_WIDTH = 800;
const int FRAME_HEIGHT = 600;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;




int main(int argc, char* argv[])
{


	

	VideoCapture capture; 	//video capture object to acquire webcam feed

	const int MIN_OBJECT_AREA = 20 * 20;
	const int MAX_OBJECT_AREA = 800 * 600 / 1.5;





	//open capture
	capture.open(0);

	//defining width and height of capture
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);

	//loop for displaying webcam feeds
	while (waitKey(30) != 'q') {

		Mat cameraFeed;	//Matrix to store each frame of the webcam feed
		Mat HSV;		//Matrix to store HSV version of the image
		Mat filtered; 	//Matrix to show filtered green ball

		//saving frame from cam to mat
		capture.read(cameraFeed);

		//converting frames from rgb(BGR) to hsv, then saving them to HSV Mat object
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		//experimentaly filtering HSV feed to capture only green
		inRange(HSV, Scalar(47, 114, 0), Scalar(72, 256, 256), filtered);

		//adding dilate to make object more visible
		Mat dilateElement = getStructuringElement(MORPH_RECT, Size(10, 10));
		dilate(filtered, filtered, dilateElement);

		//fliping camera feeds
		flip(cameraFeed, cameraFeed, 1);
		flip(HSV, HSV, 1);
		flip(filtered, filtered, 1);

		//vector for storing contours
		vector< vector<Point> > contours;

		//finding contours
		findContours(filtered, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		//drawing countours
		for (unsigned i = 0; i < contours.size(); i++) {
			drawContours(cameraFeed, contours, i, Scalar(200, 10, 30), 3);
		}



		//showing camera feeds
		imshow("Camera", cameraFeed);
		imshow("Camera HSV", HSV);
		imshow("Filtered Camera", filtered);

	
	}
}