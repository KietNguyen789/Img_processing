#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include<iostream>
using namespace cv;
std::vector<cv::Rect> detectLetters(cv::Mat img)
{
    std::vector<cv::Rect> boundRect;
    cv::Mat img_gray, img_sobel, img_threshold, element;
    cvtColor(img, img_gray, COLOR_BGR2GRAY);
    cv::Sobel(img_gray, img_sobel, CV_8U, 1, 0, 3, 1, 0, cv::BORDER_DEFAULT);
    cv::threshold(img_sobel, img_threshold, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY);
    element = getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));
    cv::morphologyEx(img_threshold, img_threshold, cv::MORPH_CLOSE, element); //Does the trick
    std::vector< std::vector< cv::Point> > contours;
    cv::findContours(img_threshold, contours, 0, 1);
    std::vector<std::vector<cv::Point> > contours_poly(contours.size());
    for (int i = 0; i < contours.size(); i++)
        if (contours[i].size() > 100)
        {
            cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
            cv::Rect appRect(boundingRect(cv::Mat(contours_poly[i])));
            if (appRect.width > appRect.height)
                boundRect.push_back(appRect);
        }
    return boundRect;
}
int main()
{
	Mat src = imread("D:\\test10.jpg", IMREAD_COLOR);
    std::vector<cv::Rect> boxlist = detectLetters(src);
    for (const auto& rect : boxlist)
    {
        cv::rectangle(src, rect, cv::Scalar(0, 255, 0), 2);
    }
    cv::imshow("Image with Bounding Boxes", src);
    cv::waitKey(0);
	return 0;
}