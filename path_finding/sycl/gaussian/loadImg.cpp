#include <SFML/Graphics.hpp>
#include <opencv4/opencv2/opencv.hpp>
#include <sycl/sycl.hpp>

//sf::RenderWindow window(sf::VideoMode(1024, 1024), "draw2");

int main(int argc, char** argv)
{
    std::cout << "Hello world" << std::endl;

    // opencv
    if(argc != 2)
    {
        std::cout << "Missing img path" << std::endl;
        return -1;
    }


    cv::Mat image;

    image = cv::imread(argv[1], 1);

    if(!image.data)
    {
        std::cout << "No Image Data" << std::endl;
        return -1;
    }

    cv::namedWindow("DisplayImage", cv::WINDOW_AUTOSIZE);
    cv::imshow("Displaay imaage" , image);

    cv::waitKey(0);

    // while(window.isOpen())
    // {
    //     sf::Event event;

    //     while(window.pollEvent(event))
    //     {
    //         if(event.type == sf::Event::Closed)
    //             window.close();
    //     }

    //     window.clear(sf::Color::White);

    //     window.display();
    // }
}
