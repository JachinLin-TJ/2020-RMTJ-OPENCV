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

void paint_armors(const Mat& src, const RotatedRect& leftrec, const RotatedRect& rightrec, const Scalar& c, int width)
{
   Point2f verticles1[4];
   Point2f verticles2[4];

   leftrec.points(verticles1);
   rightrec.points(verticles2);
   
   line(src, verticles1[3], verticles1[2], c, width);
   line(src, verticles1[2], verticles2[1], c, width);
   line(src, verticles2[1], verticles2[0], c, width);
   line(src, verticles2[0], verticles1[3], c, width);
}

double getDistance(const Point2f& pointO, const Point2f& point1)   
{   
    double distance;  
    distance = powf((pointO.x - point1.x), 2) + powf((pointO.y - point1.y), 2);  
    distance = sqrtf(distance);
    return distance;
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
   
   //灯条集合
   vector<RotatedRect> lightInfos1;
   vector<RotatedRect> lightInfos2;

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
      rotatedrectangle(srcImage, lightRect, Scalar(0, 255, 255), 8);
      lightInfos1.emplace_back(lightRect);
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
      rotatedrectangle(srcImage, lightRect, Scalar(0, 255, 255), 8);
      lightInfos2.emplace_back(lightRect);
   }

   //添加文字
   //putText(srcImage, "Red light contours : 6", Point(40, 120), FONT_HERSHEY_TRIPLEX,
   //       4, Scalar(0, 0, 255), 2);
   //putText(srcImage, "Blue light contours : 9", Point(40, 240), FONT_HERSHEY_TRIPLEX,
   //       4, Scalar(0, 0, 255), 2);    
   
   //按灯条中心x从小到大排序
   sort(lightInfos1.begin(), lightInfos1.end(), 
   [](const RotatedRect& ld1, const RotatedRect& ld2) { return ld1.center.x < ld2.center.x; });
 
   vector<RotatedRect> _armors[2];
   _armors[0].clear();
   _armors[1].clear();
   for(size_t i = 0; i < lightInfos1.size(); i++)//遍历所有灯条进行匹配
      for(size_t j = i + 1; j < lightInfos1.size(); j++)
      {
         const RotatedRect& leftLight  = lightInfos1[i];
         const RotatedRect& rightLight = lightInfos1[j];
         //角差
         float angleDiff_ = abs(leftLight.angle - rightLight.angle);
         //长度差比率
         float LenDiff_ratio = abs(leftLight.size.height - rightLight.size.height) / max(leftLight.size.height, rightLight.size.height);
         //筛选
         if(angleDiff_ > 7 || LenDiff_ratio > 0.2)
            continue;

         //左右灯条相距距离
         float dis = getDistance(leftLight.center, rightLight.center);
         //左右灯条长度的平均值
         float meanLen = (leftLight.size.height + rightLight.size.height) / 2;
         //左右灯条中心点y的差值
         float yDiff = abs(leftLight.center.y - rightLight.center.y);
         //y差比率
         float yDiff_ratio = yDiff / meanLen;
         //左右灯条中心点x的差值
         float xDiff = abs(leftLight.center.x - rightLight.center.x);
         //x差比率
         float xDiff_ratio = xDiff / meanLen;
         //相距距离与灯条长度比值
         float ratio = dis / meanLen;
         //筛选
         if(yDiff_ratio > 2.0 ||
            xDiff_ratio < 0.5 ||
            ratio > 5.0 ||
            ratio < 1.0)
            continue;
         
         _armors[0].emplace_back(leftLight);
         _armors[1].emplace_back(rightLight);
      }
   
   for (size_t i = 0; i < _armors[0].size(); i++) 
      paint_armors(srcImage, _armors[0][i], _armors[1][i], Scalar(0, 0, 255), 8);

   _armors[0].clear();
   _armors[1].clear();
   for(size_t i = 0; i < lightInfos2.size(); i++)//遍历所有灯条进行匹配
      for(size_t j = i + 1; j < lightInfos2.size(); j++)
      {
         const RotatedRect& leftLight  = lightInfos2[i];
         const RotatedRect& rightLight = lightInfos2[j];
         //角差
         float angleDiff_ = abs(leftLight.angle - rightLight.angle);
         //长度差比率
         float LenDiff_ratio = abs(leftLight.size.height - rightLight.size.height) / max(leftLight.size.height, rightLight.size.height);
         //筛选
         if(angleDiff_ > 7 || LenDiff_ratio > 0.2)
            continue;

         //左右灯条相距距离
         float dis = getDistance(leftLight.center, rightLight.center);
         //左右灯条长度的平均值
         float meanLen = (leftLight.size.height + rightLight.size.height) / 2;
         //左右灯条中心点y的差值
         float yDiff = abs(leftLight.center.y - rightLight.center.y);
         //y差比率
         float yDiff_ratio = yDiff / meanLen;
         //左右灯条中心点x的差值
         float xDiff = abs(leftLight.center.x - rightLight.center.x);
         //x差比率
         float xDiff_ratio = xDiff / meanLen;
         //相距距离与灯条长度比值
         float ratio = dis / meanLen;
         //筛选
         if(yDiff_ratio > 2.0 ||
            xDiff_ratio < 0.5 ||
            ratio > 5.0 ||
            ratio < 1.0)
            continue;
      
         _armors[0].emplace_back(leftLight);
         _armors[1].emplace_back(rightLight);
      }
   
   for (size_t i = 0; i < _armors[0].size(); i++) 
      paint_armors(srcImage, _armors[0][i], _armors[1][i], Scalar(255, 0, 0), 8);

   //输出并保存结果
   namedWindow("light", 0);
   imshow("light", srcImage);
   imwrite("light.jpg", srcImage);   
   waitKey(0);
   return 0;
}