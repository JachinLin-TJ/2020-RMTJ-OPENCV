#include<stddef.h>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
//定义变量
Mat img = imread("gray.jpg",0);
Mat result;

// 转为二值图  
threshold(img, result, 200, 255, THRESH_BINARY);
resizeWindow("enhanced", 640, 480);
//显示原图  
namedWindow("gray",0);
imshow("gray", img);
// 显示二值图 
namedWindow("二值化后的图像1",0);
imshow("二值化后的图像1",result);
imwrite("gray_after.jpg",result);
waitKey(0);
return 0;
}