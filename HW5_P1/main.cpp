#include <iostream>
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include <fstream>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    Mat image;
    Mat image_color;
    Mat image_prev;
    Mat image_video;
    int channels;
    //    Mat corners;
    Size patternsize;
    string filename;
    string header;
    string tail;
    vector<Point> pyramid;
    Size winsize = Size(21,21);
    vector<Point2f> corners, corners_prev,corners_next;
    vector<uchar> status;
    vector<float> err;

    VideoWriter Vout;
    Vout.open("VideoOut.avi",CV_FOURCC('M', 'P', 'E', 'G') , 1, Size(640,480),1);

    header = "/home/dallin/robotic_vision/HW5/Optical_Flow/O";
    tail = ".jpg";

    for (int i = 1; i < 18; i+=1)
    {
        filename = header + to_string(i) + tail;
        if(image.empty())
        {
            image = Mat::zeros(480,640,CV_32F);
        }

        image_prev = image.clone();
        image = imread(filename,CV_LOAD_IMAGE_GRAYSCALE);
        image_color = imread(filename,CV_LOAD_IMAGE_COLOR);
        //        goodFeaturesToTrack(image_prev,corners_prev,500,.01,5,noArray(),3,false,.04);

        corners_prev = corners;
        if(corners.empty())
            goodFeaturesToTrack(image,corners,500,.01,10,noArray(),3,false,.04);


        //        buildOpticalFlowPyramid(corners, pyramid, winsize,500,false,BORDER_REFLECT_101,BORDER_CONSTANT,true);
        if(corners_prev.empty())
        {
            cout<< "No corners!" << endl;
        }
        else
        {
            calcOpticalFlowPyrLK(image_prev,image,corners_prev,corners,status,err,winsize,3,
                                 TermCriteria(TermCriteria::COUNT+TermCriteria::EPS,30,.01),0,1e-4);
        }
        for(int i=0; i<corners_prev.size(); i++)
        {
            circle(image_color,corners_prev[i],2,Scalar(0,255,0),2,LINE_8,0);
        }
        for(int i=0; i<corners_prev.size(); i++)
        {
            line(image_color,corners[i],corners_prev[i],Scalar(0,0,255),2,LINE_8,0);
        }

//        imshow("Optical Flow", image_color);
//        imwrite("flow1_" + to_string(i) + ".bmp",image_color);


        waitKey(1);


    }
    for(int i=1; i<18; i+=1)
    {
        VideoCapture cap("flow1_" + to_string(i) + ".bmp");
//        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
        cap >> image_video;
        imshow("Video", image_video);
        Vout << image_video;
        waitKey(33);
    }
    for(int i=1; i<18; i+=2)
    {
        VideoCapture cap("flow2_" + to_string(i) + ".bmp");
//        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
        cap >> image_video;
        imshow("Video", image_video);
        Vout << image_video;
        waitKey(33);
    }
    for(int i=1; i<18; i+=3)
    {
        VideoCapture cap("flow3_" + to_string(i) + ".bmp");
//        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
        cap >> image_video;
        imshow("Video", image_video);
        Vout << image_video;
        waitKey(33);
    }



    waitKey(0);

    return 0;
}
