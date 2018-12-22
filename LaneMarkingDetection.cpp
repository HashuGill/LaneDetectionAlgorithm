//
//  main.cpp
//  opencvtest
//
//  Created by Hashu Gill on 2018-11-13.
//  Copyright © 2018 Hashu Gill. All rights reserved.
//

#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <stdint.h>

using namespace std;
using namespace cv;

Mat cannyfnc(Mat image){

    Mat greyImage = image;
    Mat smoothed;
    int sigma = 3;
    GaussianBlur(greyImage, smoothed, Size(5,5), sigma,sigma);
    Mat canny;
    Canny(smoothed, canny, 50, 150);

    return canny;
}

int main(){

    VideoCapture vid("/users/hashu/Desktop/Computer_Vision/test2.mp4");
    int once = 1;

    while(1){
        //mat object to contain each frame
        Mat frame;
        vid >>frame;

        if(once == 1){
            cout<<"The number of Cols are "<< frame.cols<<endl;
            cout<<"The number of Rows are "<< frame.rows<<endl;
        } once =0;

        if(frame.empty()) break;

        Mat cannyframe = cannyfnc(frame);
        Mat mask = Mat::zeros(frame.rows,frame.cols, CV_8UC1 );
        Point pts[5] = {
            Point (200,720),
            Point (200,500),
            Point (550,300),
            Point (600,300),
            Point (1200,720)
        };

        fillConvexPoly(mask, pts, 5, Scalar(255,0,0));
        //creating Region Of Interest
        Mat ROI;
        //bitwissing frame with the mark to create and fill ROI
        bitwise_and(cannyframe, cannyframe, ROI,mask);
        //finding lane markings using the Hough Transfom
        vector<Vec4i>lines;
        HoughLinesP(ROI, lines, 2, CV_PI/180, 20,10,10);

        for(size_t i =0; i<lines.size() ; i++){ //i <lines.size()
            line(frame,
                 Point(lines[i][0],lines[i][1]),
                 Point(lines[i][2],lines[i][3]),
                 Scalar(0,0,255),3,8);
        }
        imshow("frame", cannyframe);
        char c = (char)waitKey(1);
        if(c==27) break;
    }

    vid.release();
    destroyAllWindows();
    return 0;
}
