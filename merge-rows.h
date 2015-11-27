#ifndef MERGE_ROWS_H
#define MERGE_ROWS_H

#include<cv.h>
double sampleArea(cv::Mat);
std::vector<cv::Vec2f> averageLines(const std::vector<cv::Vec2f>, float, float);

//Source: http://stackoverflow.com/questions/30897072/vector-merge-algorithm-c

void mergeRowsWithCommonValues( std::vector< std::vector< int > >& );
void disp( const std::vector< int >& );
void disp( const std::vector< std::vector< int > >& );
void disp( const cv::Vec< float,2 >& );
void disp( const std::vector< cv::Vec< float,2 > >& );

#endif

