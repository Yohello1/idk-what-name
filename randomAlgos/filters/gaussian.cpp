#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>


void createGaussianKernel(cv::Mat& kernel, int size, float sigma) {
    int center = size / 2;
    float sum = 0.0f;
    for(int i = 0; i < 64; i++)
    {
        for(int j = 0; j < 64; j++)
        {
            float x = std::pow(i-center,2);
            float y = std::pow(y-center,2);
            kernel.at<float>(i,j) = (float)std::exp(-(x+y)/(2.0f*sigma*sigma));
            sum +=  std::exp(-(x+y)/(2.0f*sigma*sigma));
        }
    }

    kernel /= 1;  // Normalize to ensure the kernel sums to 1
}

#define sigmaLarge 3.0
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
    cv::parallel_for_(cv::Range(kerDist, img.rows - kerDist), [&](const cv::Range& range) {
        for (int i = range.start; i < range.end; ++i) {
            for (int j = kerDist; j < img.cols - kerDist; ++j) {
                double weightedSumSmall = 0.0;
                double weighted2SumLarge = 0.0;

                // Apply the small kernel
                for (int ky = -kernelRadiusSmall; ky <= kernelRadiusSmall; ++ky) {
                    for (int kx = -kernelRadiusSmall; kx <= kernelRadiusSmall; ++kx) {
                        int ni = i + ky;
                        int nj = j + kx;
                        if (ni >= 0 && ni < img.rows && nj >= 0 && nj < img.cols) {
                            weightedSumSmall += img.at<uchar>(ni, nj) * gaussianKernelSmall.at<double>(ky + kernelRadiusSmall, kx + kernelRadiusSmall);
                        }
                    }
                }

                // Apply the large kernel
                for (int ky = -kernelRadiusLarge; ky <= kernelRadiusLarge; ++ky) {
                    for (int kx = -kernelRadiusLarge; kx <= kernelRadiusLarge; ++kx) {
                        int ni = i + ky;
                        int nj = j + kx;
                        if (ni >= 0 && ni < img.rows && nj >= 0 && nj < img.cols) {
                            weighted2SumLarge += img.at<uchar>(ni, nj) * gaussianKernelLarge.at<double>(ky + kernelRadiusLarge, kx + kernelRadiusLarge);
                        }
                    }
                }

                // Compute the difference of Gaussians
                double dogValue = weightedSumSmall - weighted2SumLarge;

                // Clamp the result to [0, 255]
                output.at<uchar>(i, j) = static_cast<uchar>(std::min(255.0, std::max(0.0, dogValue)));
            }
        }
    });
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

        #define tempSizeKernel 64

        processImageParallel(dst, output);
        cv::Mat bigKernelMat(tempSizeKernel, tempSizeKernel, CV_32F);
        createGaussianKernel(bigKernelMat, tempSizeKernel, 8.0);

        cv::Mat displayMat;
        cv::normalize(bigKernelMat, displayMat, 0, 255, cv::NORM_MINMAX);
        displayMat.convertTo(displayMat, CV_8U);  // Now it's safe for imshow

        std::cout << "Type: " << bigKernelMat.type() << ", Size: " << bigKernelMat.size() << std::endl;



        cv::imshow("Live", frame);


        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')  // Check if 'w' or 'W' is pressed
            break;
    }
    exit(0);
}
