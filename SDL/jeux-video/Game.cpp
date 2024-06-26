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
        std::vector<::Shaders::computeImageOut> _computeOut;
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
            // magic number: "YG A0 SX TV"
            // and then in order!
            // Scene name (String)
            // X Y (size of map 65536)
            // X Y (size of viewport)
            // X Y (viewport limits)
            // farClose distance
            // Layer num (0-255)
            // Then the layer info should be stored in other files
            // The file's relative locations will be stored here

            // ok this is gonna be some C code
            // magic number: YG A0 SX TV
            // i made the name rn LMAO
            // Poisioned Chlorine will be the name
            // of the engine

            /*
            FILE *fp = fopen("testData/testScene.pcsf", "r");
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

            // Scene name, yes the 64 is arbitary limit
            char sceneName[64];
            char discardVal[8]; // yes, its a workaround, yes its cancer
            fgets(discardVal, 8, fp);
            fgets(sceneName, 64, fp);
            std::cout << "name" << sceneName << std::endl;

            // size of map
            int mapX, mapY;
            fscanf()


            // size of viewport (window)

            // viewport limits (how far they can see)

            */

            std::ifstream sceneDataFile;
            sceneDataFile.open("testData/testScene.pcsf");

            if (!sceneDataFile) //checks tom see if file opens properly
            {
                std::cout << "Error: Could not find the requested file." << std::endl;
                return 1;
            }

            std::string magicNumber;

            std::getline(sceneDataFile, magicNumber);
            std::cout << "magic " << magicNumber << std::endl;

            if(magicNumber.compare("YG A0 SX TV") != 0)
            {
                std::cout << "Error: incorrect file type" << std::endl;
                return 1;
            }

            // Scene name (String)
            // X Y (size of map 65536)
            // X Y (size of viewport)
            // X Y (viewport limits)
            // farClose distance
            // Layer num (0-255)

            std::getline(sceneDataFile, _name);

            sceneDataFile >> _mapX;
            sceneDataFile >> _mapY;

            // this is for proj matrixes
            int winSizeX, winSizeY;
            int viewSizeX, viewSizeY;
            int farClose;
            int layerNum;
            sceneDataFile >> winSizeX;
            sceneDataFile >> winSizeY;
            sceneDataFile >> viewSizeX;
            sceneDataFile >> viewSizeY;
            sceneDataFile >> farClose;
            sceneDataFile >> layerNum;


            std::cout << "( " << _mapX << " , " << _mapY << " )" << std::endl;

            return 0;
        }

        bool saveSceneToFile(std::string filePath)
        {
            // Magic number
            // Scene name (String)
            // X Y (size of map 65536)
            // X Y (size of viewport)
            // X Y (viewport limits)
            // farClose distance
            // Layer num (0-255)

            // schnoigingers bug, i got an illegal instruction from here once
            // write a print statment to start debugging...
            // they'll fear for the worst
            // and will fall back in like
            // fear will keep them in line

            std::ofstream saveFile;
            saveFile.open(filePath);
            saveFile << "YG A0 SX TV\n";
            saveFile << _name << '\n';
            saveFile << _mapX << ' ' << _mapY << '\n'; // could be def be computed
            saveFile << _viewportSizeX << ' ' << _viewportSizeY << '\n';
            saveFile << _viewportLimitX << ' ' << _viewportLimitY << '\n';
            saveFile << _transforms->_farCloseDistance << '\n';
            saveFile << _layers.size() << '\n';

            // no fucking idea what Im doing here
            filePath.erase(filePath.length()-5);
            std::filesystem::create_directories(filePath);
            std::cout << filePath << " NAME" << std::endl;

            for(int i = 0; i < _layers.size(); i++)
            {
                std::string tempPath = filePath;
                tempPath += "/";
                tempPath += std::to_string(i);

                std::ofstream layerFile(tempPath);
                layerFile << "i can try\n";

                // why do you hate me, hate me, still trying to get rid of me
                // aaaaaaaaaaaaaaaaaaaaaaa
                // oh well, that was a mistake I will not be making again
                // never EVER get into love again
                // im starting to think she was lying
                // from the very start abt actually liking me
                // like,, to leave without even saying anything is kinda not nice
                // idk what to do in the end tho, cause like, I cannot force them to
                // like me, so aaaaaaaa
                for(long long j = 0; j < _mapY; j++)
                {
                    for(int k = 0; k < _mapX; k++)
                        {
                            for(int q = 0; q < 4; q++)
                                {
                                    float temp = getFloatLayerDirect(j*1024*4+k*4+q, i);
                                    // temp = 0.5;
                                    layerFile << temp;
                                    if(temp != 0)
                                        std::cout << temp;

                                }
                        }
                }

                layerFile.close();

                std::cout << tempPath << std::endl;
            }

            saveFile.close();


            return true;
        }

        // TODO: dispatch compute should be dependent upon Fullimg size
        void computeDispatch(::Shaders::computeShader* shaderInput )
        {
           shaderInput->useProgram();
           glDispatchCompute(1024,1024,1);
           glMemoryBarrier(GL_ALL_BARRIER_BITS);
        }


        // COMPUTEIN IS TEMPORARY FOR TESTING/TRANSITIONAL PURPOSES
        void render(::Shaders::computeShader* computeShaderIn, ::Shaders::Shader* renderShader, ::Shaders::computeImageOut* computeTemp )
        {
            // why did u lie to me
            // why didnt u tell me
            // that you didnt like me
            // why couldnt you be more clear
            // it really hurt me
            // idk why it hurt me
            // that was vv mean of you

            computeDispatch(computeShaderIn);

            renderShader->useProgram();
            // hypnodancer
            // ok now we dance with the devil trying to render these
            // ok I still dont have loading setupppppp
            // CRIIII
            // TODO: for loop later for each layer
            glBindTextureUnit(computeTemp->_unit, computeTemp->getID());
            glUniform1i(glGetUniformLocation(renderShader->shaderProgram, "screen"), 0);
            glBindVertexArray(_VAO);
            glDrawElements(GL_TRIANGLES, getIndiciesSize(), GL_UNSIGNED_INT, 0);
        }


        private:
        std::string _name;
        unsigned int _mapX, _mapY;
        unsigned int _viewportSizeX, _viewportSizeY;
        unsigned int _viewportLimitX, _viewportLimitY;
        // i feel like the person kinda made this a lot worse than it shouldve been
        // :(

        /*
        ** x,y is the coordinate
        ** layer is the layer the data you want is on
        ** pos is which part of the rgba you want, 0-4
         */
        float getFloatLayerCoordinate(unsigned int x, unsigned int y, uint8_t layer, uint8_t pos)
        {
            float returnVal = _layers.at(layer)[y*_mapX*4+x*4+pos];
            return returnVal;
        }

        /*
        ** just pull the float directly from the layer
         */
        float getFloatLayerDirect(long long position, uint8_t layer)
        {
            return _layers.at(layer)[position];
        }

        void generatePlanes(uint8_t layersNum)
        {
            // this is going to get expoentially worse ToT

            // create the plane which is the size of the map, and at diff depths
            _verticies = new GLfloat[20*layersNum]; // 20 = 4*5 = (vert per plane) * (parts per plane)
            // x,y,z,u(x),v(y)
            // gonna just cycle these in & out-
            // pairs of 2 coordinates
            // 0 1 2
            // 3 4 5
            // 6 7 8
            // float[18] uvCoords = {0,1,0.5,0.5,1,1,1,0,0.5,0.5,0.5,1,0.5,0,0,0.5,0,1,0};


            int currComputeImage = 0;

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
                // 2
                _verticies[i*20+5] = -1*halfWidth;
                _verticies[i*20+6] =  1*halfHeight;
                _verticies[i*20+7] = -1*i*50;
                // 3
                _verticies[i*20+10] =  1*halfWidth;
                _verticies[i*20+11] =  1*halfHeight;
                _verticies[i*20+12] = -1*i*50;
                // 4
                _verticies[i*20+15] =  1*halfWidth;
                _verticies[i*20+16] = -1*halfHeight;
                _verticies[i*20+17] = -1*i*50;

                // uvCoordsPosition
                // 2 3
                // 0 1
                int layoutPosition = i % 4;
                if(layoutPosition == 0)
                {
                    _verticies[i*20+3] = 0.0; // why are uv vertexes mapped differently than cords?
                    _verticies[i*20+4] = 0.0;

                    _verticies[i*20+8] = 0.0;
                    _verticies[i*20+9] = 0.5;

                    _verticies[i*20+13] = 0.5;
                    _verticies[i*20+14] = 0.5;

                    _verticies[i*20+18] = 0.5;
                    _verticies[i*20+19] = 0.0;
                }
                else if(layoutPosition == 1)
                {
                    _verticies[i*20+3] = 0.5;
                    _verticies[i*20+4] = 0.0;

                    _verticies[i*20+8] = 0.5;
                    _verticies[i*20+9] = 0.5;

                    _verticies[i*20+13] = 1.0;
                    _verticies[i*20+14] = 0.5;

                    _verticies[i*20+18] = 1.0;
                    _verticies[i*20+19] = 0.0;
                }
                else if(layoutPosition == 2)
                {
                    _verticies[i*20+3] = 0.0;
                    _verticies[i*20+4] = 0.5;

                    _verticies[i*20+8] = 0.0;
                    _verticies[i*20+9] = 1.0;

                    _verticies[i*20+13] = 0.5;
                    _verticies[i*20+14] = 1.0;

                    _verticies[i*20+18] = 0.5;
                    _verticies[i*20+19] = 0.5;
                }
                else if(layoutPosition == 3)
                {
                    _verticies[i*20+3] = 0.5;
                    _verticies[i*20+4] = 0.5;

                    _verticies[i*20+8] = 0.5;
                    _verticies[i*20+9] = 1.0;

                    _verticies[i*20+13] = 1.0;
                    _verticies[i*20+14] = 1.0;

                    _verticies[i*20+18] = 1.0;
                    _verticies[i*20+19] = 0.5;
                }
                /*
                _verticies[i*20+3] = -1*0; // why are uv vertexes mapped differently than cords?
                _verticies[i*20+4] = -1*0;

                _verticies[i*20+8] = 0;
                _verticies[i*20+9] = 1;

                _verticies[i*20+13] = 1;
                _verticies[i*20+14] = 1;

                _verticies[i*20+18] = 1;
                _verticies[i*20+19] = 0;
                */

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
