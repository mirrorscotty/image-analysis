#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include "merge-rows.h"

double sampleArea(cv::Mat src)
{
    cv::Mat dst, color_dst;
    std::vector< std::vector<cv::Point> > contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::Rect bounding_rect; // Bounding box for each contour
    int sample_contour_index = 0; // Index of the contour for the sample
    int sample_area = 0; // Area of the sample
    // Make a new Mat to hold the sample contour.
    cv::Mat sample(src.rows, src.cols, CV_8UC1, cv::Scalar::all(0));
    // Used to ensure that the contour is closed. This fills all holes in the
    // sample that are smaller than 10 px.
    cv::Mat structuringElement
        = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
    // Point at the (approximate) center of the image. This should be inside
    // the boundary for the sample.
    cv::Point center;
    center.x = src.cols/2-src.cols/10;
    center.y = src.rows/2-src.rows/10;

    // Find the edges
    cv::Canny( src, dst, 30, 150, 3, true );

    //namedWindow("Edges", 1);
    //imshow("Edges", dst);
    //waitKey(0);

    // Close any holes
    cv::morphologyEx( dst, dst, cv::MORPH_CLOSE, structuringElement );
    // ?
    cv::cvtColor( dst, color_dst, CV_GRAY2BGR );

    // Find all the contours in the image and save them to a vector.
    findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    // Look at each contour to see if it's bounding box contains the center
    // point. If it does, then it's likely the sample's contour. Keep track of
    // it.
    for(int i = 0; i<contours.size(); i++) {
        double a=contourArea(contours[i],false);
        if( pointPolygonTest(contours[i], center, false) > 0 ) {
        //if( center.inside(boundingRect(contours[i])) ) {
        //if(a>largest_area) {
            sample_area = a;
            sample_contour_index = i;
            bounding_rect = boundingRect(contours[i]);
        }
    }
    cv::Scalar color(255, 255, 255);
    cv::drawContours(sample, contours, sample_contour_index, color, CV_FILLED, 8, hierarchy);

    cv::ellipse(sample, center, cv::Size(20, 20), 0, 0, 360, cv::Scalar(255,255,255), 5, 8);
   
    //namedWindow("Sample", 1);
    //imshow("Sample", sample);
    //waitKey(0);
    
    return sample_area;
}

