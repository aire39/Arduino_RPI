#include <cv.h>
#include <highgui.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

	cvNamedWindow("video", 0);
	IplImage * im = cvCreateImage(cvSize(320, 240), 8, 3);
	printf("key touched! 00");
	while(1) {
		printf("key touched! 0");
		cvShowImage("video", im);
		printf("key touched! 1");
		cvWaitKey(0);
	}	
	
	return 0;
}
