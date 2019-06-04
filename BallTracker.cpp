

#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <list>
#include <string>
#include <sstream>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2\opencv.hpp>

using namespace std;
using namespace cv;

int main(int argc, char* argv[])
{

	//video capture object to acquire webcam feed
	VideoCapture capture; 

	//open capture
	capture.open(0);

	//defining width and height of capture
	capture.set(CV_CAP_PROP_FRAME_WIDTH, 600);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 800);

	//lists
	list < Point2f > path; //List for saving path

	//mat objects
	Mat cameraFeed;	//Matrix to store each frame of the webcam feed
	Mat HSV;		//Matrix to store HSV version of the image
	Mat filtered; 	//Matrix to show filtered green ball

	//loop for displaying webcam feeds
	while (waitKey(30) != 'q') {

		//saving frame from cam to mat
		capture.read(cameraFeed);

		//converting frames from rgb(BGR) to hsv, then saving them to HSV Mat object
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		//experimentaly filtering HSV feed to capture only green
		inRange(HSV, Scalar(47, 114, 0), Scalar(72, 256, 256), filtered);

		//adding dilate to make object more visible
		Mat dilateElement = getStructuringElement(MORPH_ELLIPSE, Size(10, 10));
		dilate(filtered, filtered, dilateElement);

		//fliping camera feeds
		flip(cameraFeed, cameraFeed, 1);
		flip(HSV, HSV, 1);
		flip(filtered, filtered, 1);

		//vectors
		vector< vector<Point> > contours;								//vector for storing contours
		vector < pair < double, vector < Point > > > sortedContours;	//vector for storing sorted contours



		//finding contours
		findContours(filtered, contours, CV_RETR_LIST, CV_CHAIN_APPROX_NONE);

		for (unsigned i = 0; i < contours.size(); i++) {
			drawContours(cameraFeed, contours, i, Scalar(200, 10, 30), 3); 				//drawing contours
			sortedContours.push_back({ contourArea(contours[i], false),contours[i] });	//saving contours area and contours for sorting
		}

		if (sortedContours.size() > 0) {

			Point2f pos;// circle center
			float r;	// circle radius

			//sorting conturs
			sort(sortedContours.begin(), sortedContours.end(), [](auto a, auto b) {
				return a.first > b.first;
			});

			minEnclosingCircle(sortedContours[0].second, pos, r); //searching for smallest possible circle to enclose our contured area
			
			if (r > 8) { //if radius of the circle is more than 8

				putText(cameraFeed, "Tracking...", Point(0, 50), 2, 1, Scalar(232, 130, 0), 2);

				if (path.size() < 30) {
					path.push_back(pos);	//adding center point of the enclosing circle to back of our path list if the list.size is smaller then 30
				}
				else {
					path.pop_front();		//deleting point from the front when path.size is larger then 30 
					path.push_back(pos);	//then adding center point of the enclosing circle to back of our path list
				}

				vector < Point > pathV; //vector for storying path
				vector < Point2f > approx;//vector for storying approximated contours

				approxPolyDP(vector<Point2f>(path.begin(), path.end()), approx, 60, false); //approximation, simplification of contours

				//adding x,y parameters from circle center to pathV vector
				for (auto &p : approx) pathV.push_back({ (int)p.x,(int)p.y });

				//drawing line
				polylines(cameraFeed, { pathV }, false, Scalar(232, 30, 0),4 );

				if (pathV.size() >= 5) {
					vector < Point > itr(pathV.end() - 5, pathV.end());
					int conditions = 0;

					//drawing from the left side
					if ((itr[0].x < itr[1].x) && (itr[0].y > itr[1].y)) {
						conditions++;
					
						if ((itr[1].x < itr[2].x) && (itr[1].y < itr[2].y)) {
							conditions++;
						}
						if ((itr[2].x < itr[3].x) && (itr[2].y > itr[3].y)) {
							conditions++;
						}
						if ((itr[3].x < itr[4].x) && (itr[3].y < itr[4].y)) {
							conditions++;
						}
					}

					//drawing from the right side
					if ((itr[0].x > itr[1].x) && (itr[0].y > itr[1].y)) {
						conditions++;

						if ((itr[1].x > itr[2].x) && (itr[1].y < itr[2].y)) {
							conditions++;
						}
						if ((itr[2].x > itr[3].x) && (itr[2].y > itr[3].y)) {
							conditions++;
						}
						if ((itr[3].x > itr[4].x) && (itr[3].y < itr[4].y)) {
							conditions++;
						}
					}

					//additional conditions to determin vertices are on similiar height. Point[0] is our measure
					if (  (itr[4].y > (itr[0].y * 0.8)) && (itr[4].y < (itr[0].y * 1.2))  ) {
						conditions++;
					}
					
					//if all conditions are met we have succesfully detected letter M
					if (conditions == 5) {
						
						Mat splited[3];
						//split(cameraFeed, splited);

						putText(cameraFeed, "Gesture M detected...", Point(0, 450), 2, 1, Scalar(232, 130, 0), 2);
						imshow("Zdjêcie M", cameraFeed);

						path.clear();
					}


				}

				
			}
		}
		//showing camera feeds
		imshow("Camera", cameraFeed);
		imshow("Filtered Camera", filtered);

	
	}
}