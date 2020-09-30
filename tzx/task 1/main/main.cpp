/**
 * Task 1: Hello, OpenCV
 * @author frezcirno
 */

#include <iostream>
#include <opencv2/opencv.hpp>
using namespace cv;

int main()
{
    Mat img(300, 600, CV_8UC3, Scalar::all(0));

    String text = "Hello World!";
    int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
    double fontScale = 3;
    int thickness = 3;

    Size textSize = getTextSize(text, fontFace, fontScale, thickness, NULL);
    Point textOrg((img.cols - textSize.width) / 2, (img.rows + textSize.height) / 2);

    putText(img, text, textOrg, fontFace, fontScale, Scalar(0xff, 0xcc, 0x66), thickness, 8);
    imshow(text, img);
    imwrite("hello.png", img);
    waitKey();
    return 0;
}
