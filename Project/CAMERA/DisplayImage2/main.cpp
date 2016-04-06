#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
using namespace std;
using namespace cv;

int main(int argc, char *argv[]) {

	cvNamedWindow("video", 0);
	IplImage * im = cvCreateImage(cvSize(320, 240), 8, 3);
	while(1) {
		//cvShowImage("video", im);
		waitKey(100);
	}	
	
	return 0;
}
