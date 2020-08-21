#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
    Mat img = imread("test.png");
    resize(img, img, Size(0, 0), 0.2, 0.2);

    // cvtColor(img, img, COLOR_BGR2HSV);
    // imshow("hsv", img);
    // waitKey();

    threshold(img, img, 230, 255, THRESH_BINARY);
    imshow("thresh", img);
    waitKey();

    vector<vector<Point>> contours;

    // findContours(img.clone(), contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    // imshow("hsv1", img);

    waitKey();
    return 0;
}
