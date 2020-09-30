/**
 * Task 2: Led Light & Armor Detection
 * @author frezcirno
 */

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define LENGTH(rotrect) (max(((rotrect).size.height), ((rotrect).size.width)))
#define WIDTH(rotrect) (min(((rotrect).size.height), ((rotrect).size.width)))
#define LWRATIO(rotrect) (LENGTH(rotrect) / WIDTH(rotrect))

bool isLedLight(RotatedRect rect)
{
    if (rect.size.width > rect.size.height && -rect.angle < 45 || rect.size.height > rect.size.width && -rect.angle > 45)
    {
        return false;
    }

    if (rect.size.area() < 900)
    {
        return false;
    }

    double ratio = LWRATIO(rect);
    if (ratio < 3)
    {
        return false;
    }
    return true;
}

void drawRotatedRect(Mat img, RotatedRect rect, const Scalar &color, int thickness = 1)
{
    Point2f vertices[4];
    rect.points(vertices);
    for (int i = 0; i < 4; i++)
    {
        line(img, vertices[i], vertices[(i + 1) % 4], color, thickness);
    }
    // putText(img, to_string(LWRATIO(rect)), vertices[0], FONT_HERSHEY_SCRIPT_SIMPLEX, 1, Scalar(66, 0xcc, 0xff)); // for debug
}

vector<RotatedRect> findLedLight(Mat hsvImg, cv::InputArray lowerb, cv::InputArray upperb)
{
    Mat mask;
    inRange(hsvImg, lowerb, upperb, mask);

    vector<vector<Point>> contours;
    findContours(mask, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    vector<RotatedRect> lights;
    for (auto &&contour : contours)
    {
        auto rect = minAreaRect(contour);
        if (isLedLight(rect))
        {
            lights.push_back(rect);
        }
    }
    return lights;
}

Scalar hsvToOpencvHsv(double h, double s, double v)
{
    return Scalar(h / 2, s * 255, v * 255);
}

float euclideanDist(const Point &p, const Point &q)
{
    Point diff = p - q;
    return cv::sqrt(diff.x * diff.x + diff.y * diff.y);
}

vector<RotatedRect> findArmor(const vector<RotatedRect> &lights)
{
    vector<RotatedRect> armors;
    for (int i = 0; i < lights.size(); i++)
    {
        for (int j = i + 1; j < lights.size(); j++)
        {
            auto light1 = lights[i], light2 = lights[j];

            float angleDiff = abs(light1.angle - light2.angle);
            if (angleDiff > 5)
            {
                continue;
            }

            float LwRatioDiff = abs(LWRATIO(light1) - LWRATIO(light2));
            if (LwRatioDiff > 5)
            {
                continue;
            }

            float centerDist = euclideanDist(light1.center, light2.center);
            float avgLength = (LENGTH(light1) + LENGTH(light2)) / 2;
            float ratio = centerDist / avgLength;
            if (ratio > 4)
            {
                continue;
            }

            armors.emplace_back(RotatedRect((light1.center + light2.center) / 2, Size(centerDist, avgLength * 2), light1.angle));
        }
    }
    return armors;
}



int main()
{
    Mat img = imread("test.png");

    Mat hsvImg;
    cvtColor(img, hsvImg, COLOR_BGR2HSV);

    auto lights1 = findLedLight(hsvImg, hsvToOpencvHsv(25, 0.1, 0.9), hsvToOpencvHsv(50, 1, 1));
    for (auto &&light : lights1)
    {
        drawRotatedRect(img, light, Scalar(0, 255, 0), 3);
    }

    auto armors1 = findArmor(lights1);
    for (auto &&armor : armors1)
    {
        drawRotatedRect(img, armor, Scalar(255, 255, 0), 3);
    }

    auto lights2 = findLedLight(hsvImg, hsvToOpencvHsv(170, 0, 0.7), hsvToOpencvHsv(220, 0.6, 1));
    for (auto &&light : lights2)
    {
        drawRotatedRect(img, light, Scalar(0, 0, 255), 3);
    }

    auto armors2 = findArmor(lights2);
    for (auto &&armor : armors2)
    {
        drawRotatedRect(img, armor, Scalar(255, 0, 255), 3);
    }

    imwrite("light.jpg", img);

    return 0;
}
