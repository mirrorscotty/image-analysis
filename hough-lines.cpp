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

