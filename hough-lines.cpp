/* This is a standalone program. Pass an image name as the first parameter of
 * the program.  Switch between standard and probabilistic Hough transform by
 * changing "#if 1" to "#if 0" and back */
/* Source:
 * http://docs.opencv.org/2.4/modules/imgproc/doc/feature_detection.html */
#include <cv.h>
#include <highgui.h>
#include <math.h>
#include <iostream>
#include "merge-rows.h"

using namespace cv;

bool searchMatrix(const vector< vector<int> > A, int value)
{
    for(const auto row : A)
        for(const auto element : row)
            if(element == value)
                return true;
    return false;
}

vector<Vec2f> averageLines(const vector<Vec2f> lines, float rt, float tt)
{
    size_t i, j, k;
    bool found = false;
    vector< vector<int> > mapping;
    vector<int> tmpvector, tmpvector2;
    vector<Vec2f> new_lines;
    Vec2f tmp_line;
    /* Identify lines where dr and dt are less than rt and tt */
    /* create a vector of lines to average */
        tmpvector.push_back(0);
        tmpvector.push_back(0);
    for(i=0; i<lines.size(); i++) {
        for(j=0; j<lines.size(); j++) {
            if(i != j)
            if( (fabs(lines[i][0]-lines[j][0]) < rt)
                && (fabs(lines[i][1]-lines[j][1]) < tt) ) {
                std::cout << "Lines " << i << " and " << j << " are similar. ";
                std::cout << "Merging (" << lines[i][0] << ", " << lines[i][1];
                std::cout << ") and (";
                std::cout << lines[j][0] << ", " << lines[j][1] << ")\n";
                std::cout << "dr = " << abs(lines[i][0] - lines[j][0]);
                std::cout << " dt = " << abs(lines[i][1] - lines[j][1]);
                std::cout << '\n';
                tmpvector[0] = i;
                tmpvector[1] = j;
                mapping.push_back(tmpvector);
            }
        }
    }

    /* merge vectors with any common elements */
    disp(mapping);
    std::cout << '\n';
    mergeRowsWithCommonValues(mapping);
    disp(mapping);

    /* Add in any lines that didn't need to be averaged */
    tmpvector2.push_back(0);
    for(i=0; i<lines.size(); i++) {
        if(!searchMatrix(mapping, i)) {
            tmpvector2[0] = i;
            mapping.push_back(tmpvector2);
        }
    }

    /* average lines */
    for(i=0; i<mapping.size(); i++) {
        tmp_line[0] = 0;
        tmp_line[1] = 0;
        for(j=0; j<mapping[i].size(); j++) {
            tmp_line[0] += lines[mapping[i][j]][0];
            tmp_line[1] += lines[mapping[i][j]][1];
        }
        tmp_line[0] = tmp_line[0]/mapping[i].size();
        tmp_line[1] = tmp_line[1]/mapping[i].size();
        new_lines.push_back(tmp_line);
    }

    /* return results */
    return new_lines;
}

int main(int argc, char** argv)
{
    float theta;
    Mat src, dst, color_dst;
    if( argc != 2 || !(src=imread(argv[1], 0)).data)
        return -1;
    // Load in the grid sample.

    //Grid finding variables
    Mat gridRef = imread("grid-sample.png", 0);
    Mat result;
    Point minLoc, maxLoc, matchLoc;
    double minVal, maxVal;

    //Contour variables
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Rect bounding_rect;
    int largest_contour_index = 0;
    int largest_area = 0;
    Mat sample(src.rows, src.cols, CV_8UC1, Scalar::all(0));
    cv::Mat structuringElement = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(10, 10));
    Point center;
    center.x = src.cols/2-src.cols/10;
    center.y = src.rows/2-src.rows/10;

    Canny( src, dst, 50, 200, 3, true );
    cv::morphologyEx( dst, dst, cv::MORPH_CLOSE, structuringElement );
    cvtColor( dst, color_dst, CV_GRAY2BGR );

    matchTemplate(src, gridRef, result, CV_TM_SQDIFF);
    cv::minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, cv::Mat() );
    matchLoc = minLoc; //Because we're using CV_TM_SQDIFF
    //Draw the rectangle on the image.
    //cv::rectangle(src, matchLoc,
            //cv::Point(matchLoc.x + gridRef.cols, matchLoc.y + gridRef.rows),
            //CV_RGB(0,255,0), 3);
            //
            //
    findContours(dst, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE);
    for(int i = 0; i<contours.size(); i++) {
        double a=contourArea(contours[i],false);
        if( center.inside(boundingRect(contours[i])) ) {
        //if(a>largest_area) {
            largest_area = a;
            largest_contour_index = i;
            bounding_rect = boundingRect(contours[i]);
        }
    }
    Scalar color(255, 255, 255);
    //largest_contour_index = -1;
    drawContours(sample, contours, largest_contour_index, color, CV_FILLED, 8, hierarchy);
    
    std::cout << "Area = " << largest_area << '\n';

    ellipse(sample, center, Size(10, 10), 0, 0, 360, Scalar(255,255,255), 2, 8);

    namedWindow("Grid", 1);
    imshow("Grid", sample);
    waitKey(0);
    return 0;

#if 1
    vector<Vec2f> lines;
    HoughLines( dst, lines, 1, CV_PI/180, 195 );

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
        Point pt1(cvRound(x0 + 10000*(-b)),
                  cvRound(y0 + 10000*(a)));
        Point pt2(cvRound(x0 - 10000*(-b)),
                  cvRound(y0 - 10000*(a)));
        //if(theta>CV_PI/180*165 || theta<CV_PI/180*25) {
            line( color_dst, pt1, pt2, Scalar(0,0,255), 3, 8 );
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
    namedWindow( "Source", 1 );
    imshow( "Source", src );

    namedWindow( "Detected Lines", 1 );
    imshow( "Detected Lines", color_dst );

    waitKey(0);
    return 0;
}

