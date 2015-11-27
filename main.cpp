
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include "merge-rows.h"

void waitESC()
{
    char key;
    do 
        key = cv::waitKey(0);
    while(key != 'q');
    return;
}

int main(int argc, char** argv)
{
    float theta;
    cv::Mat src, dst, color_dst;
    if( argc != 2 || !(src=cv::imread(argv[1], 0)).data)
        return -1;
    // Load in the grid sample.

    //Grid finding variables
    cv::Mat gridRef = cv::imread("grid-sample1.png", 0);
    cv::Mat result;
    cv::Point minLoc, maxLoc, matchLoc;
    double minVal, maxVal;

    cv::Canny( src, dst, 50, 200, 3, true );
    cv::cvtColor( dst, color_dst, CV_GRAY2BGR );

    cv::matchTemplate(src, gridRef, result, CV_TM_SQDIFF);
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    matchLoc = minLoc; //Because we're using CV_TM_SQDIFF
    //Draw the rectangle on the image.
    //cv::rectangle(src, matchLoc,
            //cv::Point(matchLoc.x + gridRef.cols, matchLoc.y + gridRef.rows),
            //CV_RGB(0,255,0), 3);

    std::cout << "Area (px) = " << sampleArea(src) << '\n';

    cv::namedWindow("Grid", 1);
    cv::imshow("Grid", dst);
    waitESC();
    return 0;

#if 1
    std::vector<cv::Vec2f> lines;
    cv::HoughLines( dst, lines, 1, CV_PI/180, 195 );

    disp(lines);
    std::cout << '\n';
    lines = averageLines(lines, 50, .2);
    disp(lines);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        float rho = lines[i][0];
        float theta = lines[i][1];
        double a = cos(theta), b = sin(theta);
        double x0 = a*rho, y0 = b*rho;
        cv::Point pt1(cvRound(x0 + 10000*(-b)),
                  cvRound(y0 + 10000*(a)));
        cv::Point pt2(cvRound(x0 - 10000*(-b)),
                  cvRound(y0 - 10000*(a)));
        //if(theta>CV_PI/180*165 || theta<CV_PI/180*25) {
            line( color_dst, pt1, pt2, cv::Scalar(0,0,255), 3, 8 );
        //}
    }
#else
    vector<Vec4i> lines;
    HoughLinesP( dst, lines, 1, CV_PI/180, 80, 30, 10 );
    for( size_t i = 0; i < lines.size(); i++ )
    {
        //theta = lines[i][1];
        //if(theta>CV_PI/180*170 || theta<CV_PI/180*10) {
            line( color_dst, Point(lines[i][0], lines[i][1]),
                Point(lines[i][2], lines[i][3]), Scalar(0,0,255), 3, 8 );
        //}
    }
#endif
    cv::namedWindow( "Source", 1 );
    cv::imshow( "Source", src );

    cv::namedWindow( "Detected Lines", 1 );
    cv::imshow( "Detected Lines", color_dst );

    cv::waitKey(0);
    return 0;
}

