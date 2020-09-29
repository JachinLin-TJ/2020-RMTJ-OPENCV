#include<stddef.h>
#include<opencv2/opencv.hpp>
using namespace cv;
using namespace std;

int main()
{
  Mat src = imread("test.jpg",1);

  //Mat src = imread("test.jpg",0);


  Mat dst;
  namedWindow("RGB",0);
  imshow("RGB",src);
  //waitKey(0);


  cvtColor(src,dst,COLOR_BGR2GRAY);
  namedWindow("GRAY",0);
  resizeWindow("enhanced", 640, 480);
  imshow("GRAY",dst);
  imwrite("test_gray.jpg",dst);
  cout<<dst.channels()<<endl;



  waitKey(0);

  src.release();
  dst.release();
  destroyWindow("RGB");
  destroyWindow("GRAY");
  return 0;
}