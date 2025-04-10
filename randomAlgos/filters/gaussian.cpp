#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

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

    kernel /= 1;  // Normalize to ensure the kernel sums to 1
}

#define sigmaLarge 1.0
#define sigmaSmall 1.0
#define kerDist 15
#define bigDist 14
#define smallDist 5


void processImageParallel(cv::Mat& img, cv::Mat& output) {
    // Ensure output is initialized properly
    // if (output.empty() || output.size() != img.size()) {
    //     output = cv::Mat(img.size(), CV_8U);
    // }

    // Create the Gaussian kernels outside the parallel loop
    cv::Mat gaussianKernelSmall(smallDist, smallDist, CV_32F);
    createGaussianKernel(gaussianKernelSmall, smallDist, sigmaSmall);
    cv::Mat gaussianKernelLarge(bigDist, bigDist, CV_32F);
    createGaussianKernel(gaussianKernelLarge, bigDist, sigmaLarge);
    int kernelRadiusSmall = smallDist / 2;
    int kernelRadiusLarge = bigDist / 2;

    // Parallel loop



}



int main(int, char**)
{
    cv::Mat frame, tempdst(1280+kerDist*2, 720+kerDist*2, CV_8U), tempdst2(1280+kerDist*2, 720+kerDist*2, CV_8U), dst(1280+kerDist*2, 720+kerDist*2, CV_8U), output(1280+kerDist*2,720+kerDist*2, CV_8U);

    frame.reshape(3);

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
        // wait for a new frame from camera and store it into 'frame'
        cap.read(frame);
        cv::copyMakeBorder(frame, tempdst, kerDist, kerDist, kerDist, kerDist, cv::BORDER_CONSTANT, 0);
        cv::cvtColor(tempdst, tempdst2, cv::COLOR_BGR2GRAY);
        dst = tempdst2.reshape(0, tempdst2.rows);

        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }


        cv::Mat displayMat;
        if (frame.empty()) {
            std::cerr << "ERROR! blank frame grabbed\n";
            break;
        }

        cv::Mat gaussianKernelSmall(256, 256, CV_32F);

        {
            createGaussianKernel(gaussianKernelSmall, 256, 30);
            double minVal, maxVal; cv::Point minLoc, maxLoc;
            cv::minMaxLoc(gaussianKernelSmall, &minVal, &maxVal, &minLoc, &maxLoc);
            std::cout << maxVal << std::endl;
            gaussianKernelSmall.convertTo(gaussianKernelSmall, CV_8U, (1/maxVal)*std::pow(10,2), 1);
        }



        cv::imshow("Live", gaussianKernelSmall);


        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')  // Check if 'w' or 'W' is pressed
            break;
    }
    exit(0);
}
