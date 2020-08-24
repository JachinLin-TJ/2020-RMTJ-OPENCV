#include <iostream>
#include<vector>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace std;
using namespace cv;

Mat srcImage, _grayImage1, _grayImage2, binImage1, binImage2;

//绘制旋转矩形
void rotatedrectangle(const Mat& src, const RotatedRect& rec, const Scalar& c, int width)
{
   Point2f verticles[4];
   rec.points(verticles);
   for (int i = 0;i < 4;i++)
      line(src, verticles[i], verticles[(i + 1) % 4], c, width);
}

int main()
{
   //加载图片
   srcImage = imread("test.jpg");

   //判断图像是否加载成功
   if(!srcImage.data) {
      cout << "图像加载失败!" << endl;
      return 0;
   }
   else
      cout << "图像加载成功!" << endl;

   //进行BGR通道分离
   vector<Mat> channels;
   split(srcImage, channels);
   _grayImage1 = channels[2] - channels[0];// Get red - blue image
   _grayImage2 = channels[0];// Get blue image
   
   //阈值化
   threshold(_grayImage1, binImage1, 218, 255, THRESH_BINARY);
   threshold(_grayImage2, binImage2, 230, 255, THRESH_BINARY);
   Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(3, 3));
   Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(4, 4));

   //膨胀
   dilate(binImage1, binImage1, element1);
   dilate(binImage2, binImage2, element2);

   //找红色轮廓
   vector<vector<Point>> lightContours1;
   findContours(binImage1.clone(), lightContours1, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
   for(const auto& contour : lightContours1)
   {
      RotatedRect lightRect = minAreaRect(contour);
      Size2f size = lightRect.size;     //矩形的边长
      float max = size.width > size.height ? size.width : size.height;//长
      float min = size.width < size.height ? size.width : size.height;//宽
      if (max / min <= 3.6)
         continue;
      if (max <= 80 || max >= 140)
         continue;
      cout << size.width << " " << size.height << endl;
      rotatedrectangle(srcImage, lightRect, Scalar(0, 255, 255), 8);
   }

   //找蓝色轮廓
   vector<vector<Point>> lightContours2;
   findContours(binImage2.clone(), lightContours2, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);
   for(const auto& contour : lightContours2)
   {
      RotatedRect lightRect = minAreaRect(contour);
      Size2f size = lightRect.size;     //矩形的边长
      float max = size.width > size.height ? size.width : size.height;//长
      float min = size.width < size.height ? size.width : size.height;//宽
      if (max / min <= 3.2)
         continue;
      if (max <= 80 || max >= 140)
         continue;
      cout << size.width << " " << size.height << endl;
      rotatedrectangle(srcImage, lightRect, Scalar(0, 255, 255), 8);
   }
   //添加文字
   putText(srcImage, "Red light contours : 6", Point(40, 120), FONT_HERSHEY_TRIPLEX,
           4, Scalar(0, 0, 255), 2);
   putText(srcImage, "Blue light contours : 9", Point(40, 240), FONT_HERSHEY_TRIPLEX,
           4, Scalar(0, 0, 255), 2);        
   //输出并保存结果
   namedWindow("light", 0);
   imshow("light", srcImage);
   imwrite("light.jpg", srcImage);   
   waitKey(0);
   return 0;
}