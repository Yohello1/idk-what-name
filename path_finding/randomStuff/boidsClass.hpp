
class bird
{
    public:
    std::pair<float, float> cord;
    std::pair<float, float> velocity;
    std::vector<int> birdsToEval;


    void birdsInRange(std::array<bird, BIRD_AMT> boids, int range)
    {
        while(!birdsToEval.empty())
        {
            birdsToEval.clear();
        }

        for(int i = 0; i < BIRD_AMT; i++)
        {
            double distance = ((std::pow(std::abs(cord.first - boids[i].cord.first), 2)) +  (std::pow(std::abs(cord.second - boids[i].cord.second), 2)));
            if(distance < range)
            {
                // std::cout << "Bordy in range uwu" << " " << i << std::endl;
                birdsToEval.push_back(i);
            }
        }
    }

    void cohesionBirds(std::array<bird, BIRD_AMT> boids)
    {
        float xAvgPos = 0, xDist;
        float yAvgPos = 0, yDist;

        bool calcVal = false;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xAvgPos += boids[birdsToEval.at(i)].cord.first;
            yAvgPos += boids[birdsToEval.at(i)].cord.second;
        }

        if(xAvgPos > 0)
        {
            xAvgPos = xAvgPos / birdsToEval.size() - 1;
            xDist = xAvgPos - cord.first;
            velocity.first  += xDist*centeringFactor;
        }

        if(yAvgPos > 0)
        {
            yAvgPos = yAvgPos / birdsToEval.size() - 1;
            yDist = yAvgPos - cord.second;;
            velocity.second += yDist*centeringFactor;
        }

    }

    void seperationBirds(std::array<bird, BIRD_AMT> boids)
    {
        // first find birds within X radius
        birdsInRange(boids, 100);

        float xAvgPos = 0, xDist = 0;
        float yAvgPos = 0, yDist = 0;

        bool calcVal = false;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xAvgPos += boids[birdsToEval.at(i)].cord.first;
            yAvgPos += boids[birdsToEval.at(i)].cord.second;
        }

        if(xAvgPos > 0)
        {
            xAvgPos = xAvgPos / birdsToEval.size();
            xDist = xAvgPos - cord.first;
            velocity.first  += xDist*avoidanceFactor*-1;
        }

        if(yAvgPos > 0)
        {
            yAvgPos = yAvgPos / birdsToEval.size();
            yDist = yAvgPos - cord.second;
            velocity.second += yDist*avoidanceFactor*-1;
        }
    }

    void alignBirds(std::array<bird, BIRD_AMT> boids)
    {
        // get avg velocity
        float xVelAvg = 0;
        float yVelAvg = 0;

        for(int i = 0; (uint) i < birdsToEval.size(); i++)
        {
            xVelAvg += boids[birdsToEval.at(i)].velocity.first;
            yVelAvg += boids[birdsToEval.at(i)].velocity.second;
        }

        if(birdsToEval.size() > 0)
        {
            xVelAvg = xVelAvg / birdsToEval.size();
            yVelAvg = yVelAvg / birdsToEval.size();
            velocity.first += (xVelAvg)*matchingFactor;
            velocity.second += (yVelAvg)*matchingFactor;
        }


    }

    void screenEdges()
    {
        if(cord.first < 5)
            velocity.first += turnFactor;
        if(cord.first > (MAP_SIZE - 5))
            velocity.first -= turnFactor;
        if(cord.second < 5)
            velocity.second += turnFactor;
        if(cord.second > (MAP_SIZE - 5))
            velocity.second -= turnFactor;
    }

    void seekLeader(std::pair<int, int> pointD)
    {
        std::pair<int, int> desiredVelocity = {pointD.first-cord.first, pointD.second-cord.second};
        std::pair<int, int> steering = {desiredVelocity.first - velocity.first, desiredVelocity.second - velocity.second};
        velocity.first += steering.first/steeringFactor;
        velocity.second += steering.second/steeringFactor;

    }

    void update(std::array<bird, BIRD_AMT> boids)
    {
        float yChange = velocity.first;
        float xChange = velocity.second;
        if((0 < (xChange + cord.first))  && ( xChange + cord.first < (MAP_SIZE-1)))
        {
            cord.first += velocity.first;
        }
        if((0 < (yChange + cord.second))  && ( yChange + cord.second < (MAP_SIZE-1)))
        {
            cord.second += velocity.second;
        }

        // velocity.first = 0;
        // velocity.second = 0;

        // birdsInRange(boids, 500);

        // cohesionBirds(boids);
        // alignBirds(boids);
        seekLeader(cursor);
        //collisionAvoidance();
        screenEdges();

        // NOTE: SEPERATION MUST BE RUN AT THE END!!!
        // seperationBirds(boids);

        float speed = std::sqrt(std::pow(velocity.first,2) + std::pow(velocity.second,2));
        if(speed > maxSpeed)
        {
            velocity.first  = velocity.first/speed*maxSpeed;
            velocity.second = velocity.second/speed*maxSpeed;
        }

        if(speed < maxSpeed)
        {
            velocity.first = velocity.first/speed*minSpeed;
            velocity.second = velocity.second/speed*minSpeed;
        }


    }
};
