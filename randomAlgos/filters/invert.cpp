#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>


void processImageParallel(cv::Mat& img) {
    cv::parallel_for_(cv::Range(10, img.rows-10), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            for (int j = 10; j < img.cols-10; ++j) {
                img.at<uchar>(i, j) = 255 - img.at<uchar>(i, j);
            }
        }
    });
}

int main(int, char**)
{
    cv::Mat frame(1280, 720, CV_8U), dst(1280, 720, CV_8U);


    cv::VideoCapture cap;



    int deviceID = 0;             // 0 = open default camera
    int apiID = cv::CAP_ANY;      // 0 = autodetect default API
    cap.open(deviceID, apiID);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1280);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 720);

    if (!cap.isOpened()) {
        std::cerr << "ERROR! Unable to open camera\n";
        return -1;
    }
    std::cout << "Start grabbing" << std::endl
              << "Press any key to terminate" << std::endl;
    for (;;)
    {
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        cv::copyMakeBorder(frame, dst, 10, 10, 10, 10, cv::BORDER_CONSTANT, 0);
        cv::cvtColor(dst, dst, cv::COLOR_BGR2GRAY);

        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        processImageParallel(dst);

        cv::imshow("Live", dst);
        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')  // Check if 'w' or 'W' is pressed
            break;
    }
    return 0;
}
