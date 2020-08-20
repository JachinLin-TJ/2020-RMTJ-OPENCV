#include<iostream>
#include<opencv2/opencv.hpp>
using namespace std;
using namespace cv;
int main(int argc,char* argv[]){
    Mat src,imgGray,imgFinal;
    src=imread("test.jpg");
    
    cvtColor(src,imgGray,CV_BGR2GRAY);
    threshold(imgGray,imgFinal,100,200,CV_THRESH_BINARY);
    namedWindow("Final image",WINDOW_NORMAL);
    imshow("Final image",imgFinal);
    waitKey();

}