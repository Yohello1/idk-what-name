#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include <iostream>
#include <stdio.h>


cv::Mat createGaussianKernel(int size, double sigma) {
    cv::Mat kernel(size, size, CV_16F);
    int center = size / 2;
    double sum = 0.0;

    for (int y = -center; y <= center; ++y) {
        for (int x = -center; x <= center; ++x) {
            kernel.at<double>(y + center, x + center) = exp(-(x * x + y * y) / (2.0 * sigma * sigma));
            sum += kernel.at<double>(y + center, x + center);
        }
    }

    kernel /= sum;

    return kernel;
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
    cv::Mat gaussianKernelSmall = createGaussianKernel(smallDist, sigmaSmall);
    cv::Mat gaussianKernelLarge = createGaussianKernel(bigDist, sigmaLarge);
    int kernelRadiusSmall = gaussianKernelSmall.rows / 2;
    int kernelRadiusLarge = gaussianKernelLarge.rows / 2;

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
    cv::Mat frame(1280, 720, CV_8U), tempdst(1280+kerDist*2, 720+kerDist*2, CV_8U), tempdst2(1280+kerDist*2, 720+kerDist*2, CV_8U), dst(1280+kerDist*2, 720+kerDist*2, CV_8U), output(1280+kerDist*2,720+kerDist*2, CV_8U);


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

        //processImageParallel(dst, output);
        createGaussianKernel(64, 8.0);

        cv::imshow("Live", dst);
        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')  // Check if 'w' or 'W' is pressed
            break;
    }
    return 0;
}
