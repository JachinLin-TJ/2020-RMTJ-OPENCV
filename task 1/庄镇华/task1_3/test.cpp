#include<iostream>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace cv;
using namespace std;
int main() {
    Mat img, imgGray, imgFinal;
    img = imread("test.jpg");
    cvtColor(img, imgGray, CV_BGR2GRAY);
    threshold(imgGray, imgFinal, 200, 255, CV_THRESH_BINARY);
    namedWindow("二值化图", 0);
    imshow("二值化图", imgFinal);
    waitKey(); 
    return 0;
}
