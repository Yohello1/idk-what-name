// I lost something :P

namespace Game
{
    class Game
    {

        /*
        lmao i kinda wanna talk abt jace here
        But I know he'll probs read this and get mad
        over what I write, even tho it's from my
        perspective
        anyways, it kinda feels like he's
        using me, which i kinda expected
        my stomach was right :P
        if future me comes back, it's cause
         he only comes when he needs help with something
        and doesnt care abt anything else
        */

        public:
        // Yes these are public for simplicites sake
        std::vector<float*> _layers; // i have no idea how to use unique pointers correctly here :P
        MVPMatrix::MVPMatrixes* _transforms;
        GLfloat* _verticies;
        GLint* _indicies;
        GLuint _VAO, _VBO, _EBO;

        // turn this into ints of specific sizes
        Game(std::string sceneName, unsigned int map_x, unsigned int map_y, uint8_t layersAmt, int aspect, int width, int height, int farCloseDistance) // wtf else do I put LMAO
        {
            _name = sceneName;
            _mapX = map_x;
            _mapY = map_y;

            for(uint8_t i = 0; i < layersAmt; i++)
            {
                _layers.push_back(new float[_mapY*_mapY*4*layersAmt]);
            }

            _transforms = new MVPMatrix::MVPMatrixes(aspect, width, height, farCloseDistance);
            generatePlanes(layersAmt);

            // VBO and EBO stuff
            {
                glCreateVertexArrays(1, &_VAO);
                glCreateBuffers(1, &_VBO);
                glCreateBuffers(1, &_EBO);

                glNamedBufferData(_VBO, sizeof(GLfloat)*layersAmt*20, _verticies, GL_STATIC_DRAW);
                glNamedBufferData(_EBO, sizeof(GLint)*getIndiciesSize(), _indicies, GL_STATIC_DRAW);

                glEnableVertexArrayAttrib(_VAO, 0);
                glVertexArrayAttribBinding(_VAO, 0, 0);
                glVertexArrayAttribFormat(_VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

                glEnableVertexArrayAttrib(_VAO, 1);
                glVertexArrayAttribBinding(_VAO, 1, 0);
                glVertexArrayAttribFormat(_VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

                glVertexArrayVertexBuffer(_VAO, 0, _VBO, 0, 5 * sizeof(GLfloat));
                glVertexArrayElementBuffer(_VAO, _EBO);
            }
        }

        // layerNum is the layer number
        // Offset is the offset of the actual data, assuming in the same layer
        // There are no validity checks :P
        // value is the value it will be updated to
        // btw use the funny y = mx or wtv, you can figure it out x
        void updateValue(int layerNum, int index, float value)
        {
            _layers.at(layerNum)[index] = value;
        }

        void updateImageBulk(int layerNum, float* data)
        {
            // here memcpy
            std::copy(data, data + _mapX*_mapY*4, _layers.at(layerNum));
        }

        void rotateView(float x, float y, float z)
        {
            _transforms->rotateView(x,y,z);
        }

        void translateView(float x, float y, float z)
        {
            _transforms->translateView(x,y,z);
        }

        // I cannot be bothered to get this working
        // what was past me complaning about
        std::string getName()
        {
            // if(name == NULL)
            //     return "";
            return _name;
        }

        void changeTitleOfWindow(GLFWwindow* window)
        {
            glfwSetWindowTitle(window, _name.c_str());
        }

        int getIndiciesSize()
        {
            return _layers.size()*6;
        }

        ~ Game()
        {
            // tiddy muncher 5000 is sleeping AGAIn
            for(int i = _layers.size(); i > 0; i--)
            {
                std::cout << "SCENE FREED: " << _name << " LAYER FREED: " << (i-1) << std::endl;
                free(_layers.at(i-1));
            }
        }

        // loading file
        // dam this song rlly like
        // right one for this mood

        bool loadSceneFromFile(std::string filePath) // no you cannot load from memory, at least not yet
        {

            std::cout << "data should be getting outputted" << std::endl;
            // ok for ur ass rn
            // File Format:
            // first line: 0-255 headerSize
            // and then in order!
            // Scene name (String)
            // X,Y (size of map 65536)
            // X,Y (size of viewport)
            // X,Y (viewport limits)
            // farClose distance
            // Layer num (0-255)
            // Then the layer info should be stored in other files
            // The file's relative locations will be stored here

            // ok this is gonna be some C code
            // magic number: YG A0 SX TV
            FILE *fp = fopen("testData/testScene.pcsf", "r");
            // i made the name rn LMAO
            // Poisioned Chlorine will be the name
            // of the engine

            if(fp == NULL)
                {
                    return -1;
                }
            char magicNumber[12];
            char validNumber[] = "YG A0 SX TV";
            fgets(magicNumber, 12, fp);
            std::cout << "magic fetch: " << magicNumber << std::endl;
            if(strcmp(magicNumber, validNumber) != 0)
            {
                std::cout << "INVALID " << std::endl;
                return -1;
            }
            else
            {
                std::cout << "VALID FILE" << std::endl;
            }


            return 0;
        }

        private:
        std::string _name;
        unsigned int _mapX;
        unsigned int _mapY;

        // i feel like the person kinda made this a lot worse than it shouldve been
        // :(


        /* Let this monstrosicty be a warning to all others who dare try and shorten it


         float temp[20] = {-1*halfWidth, -1*halfHeight, -0.5, 0,0,-1*halfWidth, 1*halfHeight, -0.5,0,-1, halfWidth, halfHeight, -0.5, 1, 1, halfWidth, -1*halfHeight, -0.5, 1, 0};

            for(int i = 0; i < 20*layersNum; i++)
            {
                _verticies[i] = temp[i%20];
                if(temp[i%20] == -0.5)
                    _verticies[i] = -1*floor(i/20);
        */

        void generatePlanes(uint8_t layersNum)
        {
            // create the plane which is the size of the map, and at diff depths
            _verticies = new GLfloat[20*layersNum]; // 20 = 4*5 = (vert per plane) * (parts per plane)

            for(int i = 0; i < layersNum; i++)
            {
                // x, y, z, u, v
                // 2 . 3
                // . . .
                // 1 . 4
                //
                // therfore:
                // 1,2,4
                // 2,4,3
                int halfWidth = _mapX/2;
                int halfHeight = _mapY/2;

                // 1
                _verticies[i*20+0] = -1*halfWidth;
                _verticies[i*20+1] = -1*halfHeight;
                _verticies[i*20+2] = -1*i*50;
                _verticies[i*20+3] = -1*0; // why are uv vertexs mapped differently than cords?
                _verticies[i*20+4] = -1*0;

                // 2
                _verticies[i*20+5] = -1*halfWidth;
                _verticies[i*20+6] =  1*halfHeight;
                _verticies[i*20+7] = -1*i*50;
                _verticies[i*20+8] = 0;
                _verticies[i*20+9] = 1;

                // 3
                _verticies[i*20+10] =  1*halfWidth;
                _verticies[i*20+11] =  1*halfHeight;
                _verticies[i*20+12] = -1*i*50;
                _verticies[i*20+13] = 1;
                _verticies[i*20+14] = 1;

                // 4
                _verticies[i*20+15] =  1*halfWidth;
                _verticies[i*20+16] = -1*halfHeight;
                _verticies[i*20+17] = -1*i*50;
                _verticies[i*20+18] = 1;
                _verticies[i*20+19] = 0;
            }
            // then make the indices

            _indicies = new GLint[6*layersNum];
            for(int i = 0; i < layersNum; i++)
            {
                int InOffset = i*6;
                int VeOffset = i*4;
                _indicies[InOffset+0] = 0 + VeOffset;
                _indicies[InOffset+1] = 1 + VeOffset;
                _indicies[InOffset+2] = 3 + VeOffset;
                _indicies[InOffset+3] = 1 + VeOffset;
                _indicies[InOffset+4] = 3 + VeOffset;
                _indicies[InOffset+5] = 2 + VeOffset;
            }
        }


    };
}
