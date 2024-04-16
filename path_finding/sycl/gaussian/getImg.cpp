#include <SFML/Graphics.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <sycl/sycl.hpp>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <linux/ioctl.h>
#include <linux/types.h>
#include <linux/v4l2-common.h>
#include <linux/v4l2-controls.h>
#include <linux/videodev2.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <fstream>
#include <string>


int main()
{
    std::cout << "Hello world" << std::endl;

    int fd;

    fd = open("/dev/video0", O_RDWR);

    if(fd < 0)
    {
        perror("Failed to open device");
        return -1;
    }

    v4l2_capability capability;
    if(ioctl(fd, VIDIOC_QUERYCAP, &capability) < 0)
    {
        perror("Failed to get device abilities");
        return -1;
    }

    v4l2_format imageFormat;
    imageFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    imageFormat.fmt.pix.width = 1024;
    imageFormat.fmt.pix.height = 1024;
    imageFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
    imageFormat.fmt.pix.field = V4L2_FIELD_NONE;

    if(ioctl(fd, VIDIOC_S_FMT, &imageFormat) < 0)
    {
        perror("Could not set format");
        return -1;
    }

    v4l2_requestbuffers requestBuffer = {0};
    requestBuffer.count = 1;
    requestBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    requestBuffer.memory = V4L2_MEMORY_MMAP;

    if(ioctl(fd, VIDIOC_REQBUFS, &requestBuffer) < 0){
        perror("Could not request buffer from device, VIDIOC_REQBUFS");
        return 1;
    }

        v4l2_buffer queryBuffer = {0};
    queryBuffer.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    queryBuffer.memory = V4L2_MEMORY_MMAP;
    queryBuffer.index = 0;
    if(ioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0){
        perror("Device did not return the buffer information, VIDIOC_QUERYBUF");
        return 1;
    }

    if(ioctl(fd, VIDIOC_QUERYBUF, &queryBuffer) < 0)
    {
        perror("Device did not return buffer information");
        return -1;
    }

    char* buffer = (char*) mmap(NULL, queryBuffer.length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, queryBuffer.m.offset);
    std::memset(buffer, 0, queryBuffer.length);

    v4l2_buffer bufferinfo;
    std::memset(&bufferinfo, 0, sizeof(bufferinfo));
    bufferinfo.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    bufferinfo.memory = V4L2_MEMORY_MMAP;
    bufferinfo.index = 0;

    int type = bufferinfo.type;
    if(ioctl(fd, VIDIOC_STREAMON,  &type) < 0)
    {
        perror("Could not start streaming");
        return -1;
    }

    // Queue the buffer
    if(ioctl(fd, VIDIOC_QBUF, &bufferinfo) < 0){
        perror("Could not queue buffer, VIDIOC_QBUF");
        return 1;
    }

    // Dequeue the buffer
    if(ioctl(fd, VIDIOC_DQBUF, &bufferinfo) < 0){
        perror("Could not dequeue the buffer, VIDIOC_DQBUF");
        return 1;
    }
    // Frames get written after dequeuing the buffer

    std::cout << "Buffer has: " << (double)bufferinfo.bytesused / 1024
            << " KBytes of data" << std::endl;

    // Write the data out to file
    std::ofstream outFile;
    outFile.open("webcam_output.jpeg", std::ios::binary| std::ios::app);

    int bufPos = 0, outFileMemBlockSize = 0;  // the position in the buffer and the amoun to copy from
                                        // the buffer
    int remainingBufferSize = bufferinfo.bytesused; // the remaining buffer size, is decremented by
                                                    // memBlockSize amount on each loop so we do not overwrite the buffer
    char* outFileMemBlock = NULL;  // a pointer to a new memory block
    int itr = 0; // counts thenumber of iterations
    while(remainingBufferSize > 0) {
        bufPos += outFileMemBlockSize;  // increment the buffer pointer on each loop
                                        // initialise bufPos before outFileMemBlockSize so we can start
                                        // at the begining of the buffer

        outFileMemBlockSize = 1024;    // set the output block size to a preferable size. 1024 :)
        outFileMemBlock = new char[sizeof(char) * outFileMemBlockSize];

        // copy 1024 bytes of data starting from buffer+bufPos
        memcpy(outFileMemBlock, buffer+bufPos, outFileMemBlockSize);
        outFile.write(outFileMemBlock,outFileMemBlockSize);

        // calculate the amount of memory left to read
        // if the memory block size is greater than the remaining
        // amount of data we have to copy
        if(outFileMemBlockSize > remainingBufferSize)
            outFileMemBlockSize = remainingBufferSize;

        // subtract the amount of data we have to copy
        // from the remaining buffer size
        remainingBufferSize -= outFileMemBlockSize;

        // display the remaining buffer size
        std::cout << itr++ << " Remaining bytes: "<< remainingBufferSize << std::endl;

        delete outFileMemBlock;
    }

    // Close the file
    outFile.close();


/******************************** end looping here **********************/

    // end streaming
    if(ioctl(fd, VIDIOC_STREAMOFF, &type) < 0){
        perror("Could not end streaming, VIDIOC_STREAMOFF");
        return 1;
    }

    close(fd);
    return 0;
}
