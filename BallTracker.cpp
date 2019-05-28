// Projekt NAI.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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

	//video capture object to acquire webcam feed
	VideoCapture capture;

	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1) {
		//store image to matrix
		capture.read(cameraFeed);
		//convert frame from BGR to HSV colorspace
		//show frames 

		imshow("Kamera", cameraFeed);



		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey();
	}
}