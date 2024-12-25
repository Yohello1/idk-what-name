
#include <SFML/Network.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#undef None

#include <sycl/sycl.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdint>
#include <functional>
#include <typeindex>

#include <chlorine/logs/logs.hpp>
#include <chlorine/utils/strings.hpp>
#include <chlorine/scene/scene.hpp>
#include <chlorine/io/io.hpp>
#include <chlorine/scene/component.hpp>
#include <chlorine/scene/componentManager.hpp>


long long _lcg_seed = 78947268766;
uint64_t birdies_amt = 25000;




int main()
{
    std::cout << "amt of birdies " << std::endl;
    // std::cin >> birdies_amt;

    
    sf::RenderWindow window(sf::VideoMode(1024, 1024), "draw2");
    srand(time(NULL));
    
    sycl::queue queue;

    // inital positions
    sycl::buffer<float, 1> x_pos(sycl::range<1>{birdies_amt});
    sycl::buffer<float, 1> y_pos(sycl::range<1>{birdies_amt});

    // final positions
    sycl::buffer<float, 1> x_vel(sycl::range<1>{birdies_amt});
    sycl::buffer<float, 1> y_vel(sycl::range<1>{birdies_amt});

    // mouse position
    float mouse_x_p = 0, mouse_y_p = 0;
    // sycl::buffer<float, 1> mouse_x_s(&mouse_x_p, sycl::range<1>(1));
    // sycl::buffer<float, 1> mouse_y_s(&mouse_y_p, sycl::range<1>(1));

    auto mouse_x_s = malloc_shared<float>(1, queue);
    auto mouse_y_s = malloc_shared<float>(1, queue);
    auto ctr = malloc_shared<uint64_t>(1, queue);


    // init the data
    {
        sycl::host_accessor<float, 1, sycl::access::mode::write> x_acc(x_pos);
        sycl::host_accessor<float, 1, sycl::access::mode::write> y_acc(y_pos);

        sycl::host_accessor<float, 1, sycl::access::mode::write> x_acl(x_vel);
        sycl::host_accessor<float, 1, sycl::access::mode::write> y_acl(y_vel);

        for(uint64_t i = 0; i < birdies_amt; i++)
        {
            y_acc[i] = rand() % 1024;
            x_acc[i] = rand() % 1024;

            y_acl[i] = 0;
            x_acl[i] = 0;
        }
    }

    // queue.submit([&](sycl::handler& h) {

    //     sycl::accessor xx_pos(x_vel, h, sycl::read_write);
    //     sycl::accessor yy_pos(y_pos, h, sycl::read_write);

    //     sycl::accessor xx_vel(x_vel, h, sycl::read_write);
    //     sycl::accessor yy_vel(y_pos, h, sycl::read_write);

    //     sycl::accessor x_m(mouse_x_s, h, sycl::read_only);
    //     sycl::accessor y_m(mouse_y_s, h, sycl::read_only);

    //     auto out = sycl::stream(1024, 768, h);

    //     h.parallel_for(sycl::range<1>(birdies_amt), [=](sycl::id<1> indx){

    //         // Constants
    //         float steer_factor = 0.00005;
    //         float min_speed = 0.1, max_speed = 0.5;

    //         // Calculating the speed
    //         float old_speed, speed;
    //         old_speed = speed = sycl::hypot(xx_vel[indx], yy_vel[indx]);

    //         float xx_speed = (xx_pos[indx] - x_m[0])/2*steer_factor +  xx_vel[indx];
    //         float yy_speed = (yy_pos[indx] - y_m[0])/2*steer_factor +  yy_vel[indx];

    //         speed = sycl::fmin(speed, max_speed);
    //         speed = sycl::fmax(speed, min_speed);

    //         // Scalling the speed
    //         float x_v_scaled = xx_speed*(old_speed/speed);
    //         float y_v_scaled = yy_speed*(old_speed/speed);

    //         // Updating the position
    //         xx_pos[indx] += xx_vel[indx];
    //         yy_pos[indx] += yy_vel[indx];

    //         // Updating vel
    //         xx_vel[indx] = x_v_scaled;
    //         yy_vel[indx] = y_v_scaled;

    //         if(x_m[0] == 0.0f)
    //             out << "nO";
    //     });
    // });

    queue.wait();

    // render_and_update_points(queue, x_pos, y_pos, x_vel, y_vel, birdies_amt);

    while(window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            switch(event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;

                // SCATTER
            case sf::Event::KeyPressed:
                queue.submit([&](sycl::handler& h) {
                    sycl::accessor xx_pos(x_pos, h, sycl::read_write);
                    sycl::accessor yy_pos(y_pos, h, sycl::read_write);

                    sycl::accessor xx_vel(x_vel, h, sycl::read_write);
                    sycl::accessor yy_vel(y_vel, h, sycl::read_write);

                    h.parallel_for(sycl::range<1>(birdies_amt), [=](sycl::id<1> indx)
                    {
                        float steer_factor = 0.05;
                        float min_speed = 8, max_speed = 15;

                        float vel_x = xx_vel[indx];
                        float vel_y = yy_vel[indx];

                        // update position
                        if(( 0 < (vel_x + xx_pos[indx])) && ((vel_x + xx_pos[indx]) < 1024))
                            xx_pos[indx] += vel_x;

                        if(( 0 < (vel_y + yy_pos[indx])) && ((vel_y + yy_pos[indx]) < 1024))
                            yy_pos[indx] += vel_y;

                        // determine new speed
                        vel_x = ((mouse_x_s[0]-xx_pos[indx])/2);
                        vel_y = ((mouse_y_s[0]-yy_pos[indx])/2);


                        // update speed
                        float speed = sycl::sqrt(sycl::pow(vel_x, 2) + sycl::pow(vel_y, 2));
                        if(speed > max_speed)
                        {
                            vel_x = vel_x/speed*max_speed;
                            vel_y = vel_y/speed*max_speed;
                        }
                        else if(speed < min_speed)
                        {
                            vel_x = vel_x/speed*min_speed;
                            vel_y = vel_y/speed*min_speed;
                        }

                        xx_vel[indx] = vel_x;
                        yy_vel[indx] = vel_y;
                    });
                });
                queue.wait();
                std::cout << "ASADASD";

                break;
            default:
                break;
            }


            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            *(mouse_x_s) = localPosition.x;
            *(mouse_y_s) = localPosition.y;
        }

        window.clear(sf::Color::Black);

        // std::cout << "Enter the next val: ";
        // int temp1, temp2;
        // std::cin >> temp1 >> temp2;

        // mouse_x_p = temp1;
        // mouse_y_p = temp2;

        auto t1 = std::chrono::steady_clock::now();
        queue.submit([&](sycl::handler& h) {

            sycl::accessor xx_pos(x_pos, h, sycl::read_write);
            sycl::accessor yy_pos(y_pos, h, sycl::read_write);

            sycl::accessor xx_vel(x_vel, h, sycl::read_write);
            sycl::accessor yy_vel(y_vel, h, sycl::read_write);

            sycl::stream out(1024, 256, h);

            h.parallel_for(sycl::range<1>(birdies_amt), [=](sycl::id<1> indx){

                // Constants
                float steer_factor = 0.01 ;
                float min_speed = 15, max_speed = 16;

                float vel_x = xx_vel[indx];
                float vel_y = yy_vel[indx];

                // update position
                if(( 0 < (vel_x + xx_pos[indx])) && ((vel_x + xx_pos[indx]) < 1024))
                    xx_pos[indx] += vel_x;

                if(( 0 < (vel_y + yy_pos[indx])) && ((vel_y + yy_pos[indx]) < 1024))
                    yy_pos[indx] += vel_y;


                // determine new speed
                vel_x += ((mouse_x_s[0]-xx_pos[indx])/2)*steer_factor;
                vel_y += ((mouse_y_s[0]-yy_pos[indx])/2)*steer_factor;


                // update speed
                float speed = sycl::sqrt(sycl::pow(vel_x, 2) + sycl::pow(vel_y, 2));
                if(speed > max_speed)
                {
                    vel_x = (vel_x/speed)*max_speed;
                    vel_y = (vel_y/speed)*max_speed;
                }
                else if(speed < min_speed)
                {
                    vel_x = (vel_x/speed)*min_speed;
                    vel_y = (vel_y/speed)*min_speed;
                }

                xx_vel[indx] = vel_x;
                yy_vel[indx] = vel_y;
            });
        });

        ctr++;
        // std::cout << ctr <<  '\n';

        queue.wait();

        auto t2 = std::chrono::steady_clock::now();

        std::cout << std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count() << '\n';

        std::vector<float> x_positions(birdies_amt);
        std::vector<float> y_positions(birdies_amt);
        {
            sycl::host_accessor x_acc(x_pos, sycl::read_only);
            sycl::host_accessor y_acc(y_pos, sycl::read_only);
            for (size_t i = 0; i < birdies_amt; i++) {
                x_positions[i] = x_acc[i];
                y_positions[i] = y_acc[i];
            }
        }

        for (size_t i = 0; i < birdies_amt; i++) {
            sf::CircleShape point(1.0f); // Radius of the point
            point.setPosition(x_positions[i], y_positions[i]); // Scale to canvas size
            point.setFillColor(sf::Color::White);
            window.draw(point);
        }

        // std::cout << x_positions[10] << "," << y_positions[10] << '\n';

        window.display();

    }

    std::cout << "a";
    free(mouse_x_s, queue);
    free(mouse_y_s, queue);
}
