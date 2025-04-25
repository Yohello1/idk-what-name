#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <omp.h>

#include <iostream>
#include <stdio.h>


void createGaussianKernel(cv::Mat& kernel, int size, float sigma) {
    int center = size / 2;
    float sum = 0.0f;
    for(int i = 0; i < size; i++)
    {
        for(int j = 0; j < size; j++)
        {
            float x = std::pow(i-center,2);
            float y = std::pow(j-center,2);
            float x_val = std::sqrt(x+y);
            float factor = 1/(sigma*std::sqrt(2*3.14));
            float exp = std::exp((-1.0/2.0)*(std::pow(x_val,2)/(std::pow(sigma,2))));
            float val = factor*exp;


            kernel.at<float>(i,j) = val;
            sum +=  kernel.at<float>(i,j);
            // std::cout << (std::exp(-(std::pow(z,2))/(2.0f*sigma*sigma)))*(1/(sigma*std::sqrt(3.14*2))) << '\n';
        }
    }

    kernel /= sum;  // Normalize to ensure the kernel sums to 1
}
#define kerDist 16

#define sigmaLarge 2.828
#define bigDist 19

#define sigmaSmall 1.0
#define smallDist 13

void processImageParallel(const cv::Mat& img, cv::Mat& output)
{

    CV_Assert(img.type() == CV_32FC1);

    const int bigRadius = bigDist;
    const int BigKernelSize = bigRadius * 2 + 1;
    cv::Mat gaussianKernelLarge(BigKernelSize, BigKernelSize, CV_32F);
    createGaussianKernel(gaussianKernelLarge, BigKernelSize, sigmaLarge);

    const int smallRadius = smallDist;
    const int smallKernelSize = smallRadius * 2 + 1;
    cv::Mat gaussianKernelSmall(smallKernelSize, smallKernelSize, CV_32F);
    createGaussianKernel(gaussianKernelSmall, smallKernelSize, sigmaSmall);


    output.create(img.size(), CV_32F);

    const int rows = img.rows;
    const int cols = img.cols;

    #pragma omp parallel for collapse(2)
    for (int j = kerDist; j < rows - kerDist; ++j) {
        for (int i = kerDist; i < cols - kerDist; ++i) {
            float bigSum = 0.0f;

            for (int m = 0; m < BigKernelSize; ++m) {
                int y = j + (m - bigRadius);
                for (int n = 0; n < BigKernelSize; ++n) {
                    int x = i + (n - bigRadius);
                    float a = img.at<float>(y, x);
                    float b = gaussianKernelLarge.at<float>(m, n);
                    float c = a*b;
                    bigSum += std::abs(c);
                }
            }

            float smallSum = 0.0f;
            for (int m = 0; m < smallKernelSize; ++m) {
                int y = j + (m - smallRadius);
                for (int n = 0; n < smallKernelSize; ++n) {
                    int x = i + (n - smallRadius);
                    float a = img.at<float>(y, x);
                    float b = gaussianKernelSmall.at<float>(m, n);
                    smallSum += std::abs(a * b);
                }
            }


            output.at<float>(j, i) = smallSum - bigSum;
        }
    }
}



int main(int, char**)
{
    cv::Mat frame, tempdst(720+kerDist*2, 1280+kerDist*2, CV_32F), tempdst2(720+kerDist*2, 1280+kerDist*2, CV_32F), dst(720+kerDist*2, 1280+kerDist*2, CV_32F), output(720+kerDist*2,1280+kerDist*2, CV_32F);

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
              << "Press 'w' key to terminate" << std::endl;
    for (;;)
    {

        cv::Mat frame, gray, img, output;

        cap.read(frame);
        cv::copyMakeBorder(frame, frame,
                           kerDist,kerDist, kerDist,kerDist,
                           cv::BORDER_CONSTANT, cv::Scalar::all(0));
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        gray.convertTo(img, CV_32F);
        output.create(img.size(), CV_32F);
        processImageParallel(img, output);
        cv::Mat disp;
        output.convertTo(disp, CV_8U);
        cv::imshow("draw2", disp);


        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')
            break;
    }
    exit(0);
}
