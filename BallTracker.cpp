
#include <sstream>
#include <string>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{


	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;

	Mat HSV;
	//Matrix to store HSV version of the image

	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture
	capture.open(0);
	//defining width and height of capture
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);

	//infinite loop for displaying webcam
	while (1) {
		//saving frame from cam to mat
		capture.read(cameraFeed);

		//converting frames from rgb(BGR) to hsv, then saving them to HSV Mat object
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);



		//showing frame
		imshow("Kamera", cameraFeed);
		imshow("Kamera HSV", HSV);

		
		waitKey(30);
	}
}