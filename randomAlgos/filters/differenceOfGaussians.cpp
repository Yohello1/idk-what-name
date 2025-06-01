#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/videoio.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <omp.h>
#include <sycl/sycl.hpp>

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

#define sigmaLarge 1.8
#define bigDist 13

#define sigmaSmall 0.9
#define smallDist 6

#define kerDist MAX(smallDist, bigDist)

sycl::queue q(sycl::gpu_selector_v);


void processImageParallel(const cv::Mat& img, cv::Mat& output) {
    CV_Assert(img.type() == CV_32FC1);

    const int bigRadius      = bigDist;
    const int bigKernelSize  = bigRadius * 2 + 1;
    const int smallRadius    = smallDist;
    const int smallKernelSize= smallRadius * 2 + 1;

    cv::Mat gaussianKernelLarge(bigKernelSize,  bigKernelSize,  CV_32F);
    createGaussianKernel(gaussianKernelLarge, bigKernelSize,  sigmaLarge);

    cv::Mat gaussianKernelSmall(smallKernelSize,smallKernelSize,CV_32F);
    createGaussianKernel(gaussianKernelSmall, smallKernelSize, sigmaSmall);

    output.create(img.size(), CV_32F);

    const int rows = img.rows;
    const int cols = img.cols;
    size_t   imgCount   = size_t(rows) * cols;
    size_t   bigKCount  = size_t(bigKernelSize)  * bigKernelSize;
    size_t   smallKCount= size_t(smallKernelSize)* smallKernelSize;

    float *d_img, *d_out, *d_kL, *d_kS;

    d_img = sycl::malloc_shared<float>(imgCount,    q);
    d_out = sycl::malloc_shared<float>(imgCount,    q);
    d_kL  = sycl::malloc_shared<float>(bigKCount,   q);
    d_kS  = sycl::malloc_shared<float>(smallKCount, q);

    std::memcpy(d_img, img.ptr<float>(),              imgCount   * sizeof(float));
    std::memcpy(d_kL,  gaussianKernelLarge.ptr<float>(),  bigKCount  * sizeof(float));
    std::memcpy(d_kS,  gaussianKernelSmall.ptr<float>(),  smallKCount* sizeof(float));
    q.memset(d_out, 0, imgCount * sizeof(float)).wait();

    q.parallel_for(
        sycl::range<2>( rows - 2*kerDist,
                        cols - 2*kerDist ),
        [=](sycl::id<2> idx) {
            int j = int(idx[0]) + kerDist;
            int i = int(idx[1]) + kerDist;

            float bigSum = 0.0f;
            for(int m = 0; m < bigKernelSize; ++m) {
                int y = j + (m - bigRadius);
                for(int n = 0; n < bigKernelSize; ++n) {
                    int x = i + (n - bigRadius);
                    float a = d_img[y*cols + x];
                    float b = d_kL[m*bigKernelSize + n];
                    bigSum += sycl::fabs(a * b);
                }
            }

            float smallSum = 0.0f;
            for(int m = 0; m < smallKernelSize; ++m) {
                int y = j + (m - smallRadius);
                for(int n = 0; n < smallKernelSize; ++n) {
                    int x = i + (n - smallRadius);
                    float a = d_img[y*cols + x];
                    float b = d_kS[m*smallKernelSize + n];
                    smallSum += sycl::fabs(a * b);
                }
            }

            d_out[j*cols + i] = smallSum - bigSum;
        }
    ).wait();

    std::memcpy(output.ptr<float>(), d_out, imgCount * sizeof(float));

    sycl::free(d_img, q);
    sycl::free(d_out, q);
    sycl::free(d_kL,  q);
    sycl::free(d_kS,  q);

}

void customRound(cv::Mat& mat, double cutoff = 0.8) {
    CV_Assert(mat.type() == CV_32F);

    float max_v = -1.0;

    for (int y = 0; y < mat.rows; ++y) {
        for (int x = 0; x < mat.cols; ++x) {
            float frac = mat.at<float>(y,x);

            max_v = std::max(frac, max_v);
        }
    }

    for (int y = 0; y < mat.rows; ++y) {
        for (int x = 0; x < mat.cols; ++x) {
            float frac = (mat.at<float>(y,x))/max_v;

            // if(frac > cutoff)
            //     mat.at<float>(y,x) = 255.0;
            // else
            //  mat.at<float>(y,x) = 0.0;
        }
    }

}

int main(int, char**)
{
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


    cv::Mat frame, gray, img, output;

    bool alloced = false;

    for (;;)
    {


        cap.read(frame);
        cv::copyMakeBorder(frame, frame,
                           kerDist,kerDist, kerDist,kerDist,
                           cv::BORDER_CONSTANT, cv::Scalar::all(0));
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        gray.convertTo(img, CV_32F);
        output.create(img.size(), CV_32F);

        // if(!alloced)
        // {
        //     alloced = true;
        //     preAlloc(img);
        //     std::cout << " RUN" << std::endl;
        // }

        // if (!d_img || !d_out || !d_kL || !d_kS) {
        //     std::cerr << "Memory not allocated properly\n";
        //     std::exit(1);
        // }


        processImageParallel(img, output);
        cv::Mat disp;

        customRound(output, 0.05);

        output.convertTo(disp, CV_8U, 20.0f, 0.0f);
        cv::imshow("draw2", disp);


        char key = cv::waitKey(5);
        if (key == 'w' || key == 'W')
            break;
    }


    exit(0);
}
