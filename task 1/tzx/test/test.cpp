#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    Mat img = imread("test.png");
    cvtColor(img, img, COLOR_RGB2GRAY);
    threshold(img, img, 200, 255, 0);
    resize(img, img, Size(0, 0), 0.2, 0.2);
    imshow("Image Binarization", img);
    waitKey();
    return 0;
}
