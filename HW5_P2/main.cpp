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
    vector<Rect> roi_rect;
    vector<Mat> roi_templ;
    Size roi_size = Size(11,11);
    Mat roi_big;
    Rect roi_big_rect;
    Size search_size = Size(51,51);
    vector<Rect> roi_search_rect;
    vector<Mat> roi_search;
    Mat result;
    double minVal; double maxVal; Point minLoc; Point maxLoc;

//    VideoWriter Vout;
//    Vout.open("VideoOut.avi",CV_FOURCC('M', 'P', 'E', 'G') , 1, Size(640,480),1);

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

        corners_prev = corners;
        // Define a large roi that keeps the feature corners not near the edges of the image.
        roi_big_rect = Rect(search_size.width/2,search_size.height/2,image.cols-search_size.width,image.rows-search_size.height);
        roi_big = image(roi_big_rect);
        // Get initial good features to track.
        if(corners.empty())
        {
            goodFeaturesToTrack(roi_big,corners,1,.01,10,noArray(),3,false,.04);

            //Shifts points back in global image frame.
            for(int i = 0; i<corners.size(); i++)
            {
                corners[i] = Point(corners[i].x+search_size.width/2,corners[i].y+search_size.height/2);
            }
        }

        //        rectangle(image_color,roi_big_rect,Scalar(255,0,0),2,LINE_8,0);

        roi_search_rect.clear();
        roi_rect.clear();
        roi_templ.clear();
        roi_search.clear();

        if(!corners_prev.empty())
        {
            // Now draw "Templates" around each feature.
            for(int i = 0; i<corners_prev.size(); i++)
            {

                Point rect_center = Point(corners_prev[i].x-roi_size.width/2,corners_prev[i].y-roi_size.height/2);
                Point rect_search_center = Point(corners_prev[i].x-search_size.width/2,corners_prev[i].y-search_size.height/2);
                roi_search_rect.push_back(Rect(rect_search_center,search_size));
                roi_rect.push_back(Rect(rect_center,roi_size));
                rectangle(image_color,roi_rect[i],Scalar(0,0,255),1,LINE_8,0);
                roi_templ.push_back(image(roi_rect[i]));
                roi_search.push_back(image(roi_search_rect[i]));
                rectangle(image_color,roi_search_rect[i],Scalar(0,255,0),1,LINE_8,0);
//                circle(image_color,Point(corners_prev[i].x-search_size.width/2,corners_prev[i].y-search_size.height/2),2,Scalar(255,0,0),1,LINE_8,0);
            }

            for(int i = 0; i<corners.size(); i++)
            {

                matchTemplate(roi_search[i],roi_templ[i],result,CV_TM_SQDIFF,noArray());
                normalize( result, result, 0, 1, NORM_MINMAX, -1, Mat() );
//                imshow("result",result);
                minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat() );
                corners[i] = Point(minLoc.x+corners[i].x-search_size.width/2, minLoc.y+corners[i].y-search_size.height/2);
//                waitKey(1);
            }


                for(int i=0; i<corners.size(); i++)
                {
                    circle(image_color,corners[i],2,Scalar(0,255,0),2,LINE_8,0);
                }
        }
        //        for(int i=0; i<corners_prev.size(); i++)
        //        {
        //            line(image_color,corners[i],corners_prev[i],Scalar(0,0,255),2,LINE_8,0);
        //        }

        //        imshow("Optical Flow", image_color);
        //        imwrite("flow1_" + to_string(i) + ".bmp",image_color);


        imshow("Template",image_color);
        if(!roi_search.empty())
        {
        imshow("Search",roi_search[0]);
        imshow("Temp",roi_templ[0]);
        }
        waitKey(1);


    }

    //    for(int i=1; i<18; i+=1)
    //    {
    //        VideoCapture cap("flow1_" + to_string(i) + ".bmp");
    ////        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
    //        cap >> image_video;
    //        imshow("Video", image_video);
    //        Vout << image_video;
    //        waitKey(33);
    //    }
    //    for(int i=1; i<18; i+=2)
    //    {
    //        VideoCapture cap("flow2_" + to_string(i) + ".bmp");
    ////        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
    //        cap >> image_video;
    //        imshow("Video", image_video);
    //        Vout << image_video;
    //        waitKey(33);
    //    }
    //    for(int i=1; i<18; i+=3)
    //    {
    //        VideoCapture cap("flow3_" + to_string(i) + ".bmp");
    ////        image_video = imread("flow1_" + to_string(i) + ".bmp", CV_LOAD_IMAGE_COLOR);
    //        cap >> image_video;
    //        imshow("Video", image_video);
    //        Vout << image_video;
    //        waitKey(33);
    //    }



    waitKey(0);

    return 0;
}
