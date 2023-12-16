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
        GLuint VAO, VBO, EBO;

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



            // we do not talk abt this
            //
            GLfloat vertices[] =
                {
                    -64.0f, -64.0f , 0.0f, 0.0f, 0.0f,
                    -64.0f,  64.0f , 0.0f, 0.0f, 1.0f,
                    64.0f,  64.0f , 0.0f, 1.0f, 1.0f,
                    64.0f, -64.0f , 0.0f, 1.0f, 0.0f,

                    -64.0f, -64.0f , 5.0f, 0.0f, 0.0f,
                    -64.0f,  64.0f , 5.0f, 0.0f, 1.0f,
                    64.0f,  64.0f , 5.0f, 1.0f, 1.0f,
                    64.0f, -64.0f , 5.0f, 1.0f, 0.0f,
                };

            GLuint indices[] =
                {
                    0, 2, 1,
                    0, 3, 2,
                    4, 6, 5,
                    4, 7, 6
                };
            // VBO and EBO stuff
            {
                glCreateVertexArrays(1, &VAO);
                glCreateBuffers(1, &VBO);
                glCreateBuffers(1, &EBO);

                glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glNamedBufferData(EBO, sizeof(indices), indices, GL_STATIC_DRAW);

                glEnableVertexArrayAttrib(VAO, 0);
                glVertexArrayAttribBinding(VAO, 0, 0);
                glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);

                glEnableVertexArrayAttrib(VAO, 1);
                glVertexArrayAttribBinding(VAO, 1, 0);
                glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));

                glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(GLfloat));
                glVertexArrayElementBuffer(VAO, EBO);
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

        void changeTitle(GLFWwindow* window)
        {
            glfwSetWindowTitle(window, _name.c_str());
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

        private:
        GLfloat* _verticies;
        GLint* _indicies;
        std::string _name;
        unsigned int _mapX;
        unsigned int _mapY;

        // i feel like the person kinda made this a lot worse than it shouldve been
        // :(

        void generatePlanes(uint8_t layersNum, uint16_t height, uint16_t width)
        {
            // create the plane which is the size of the map, and at diff depths
            _verticies = new GLfloat[20*layersNum]; // 20 = 4*5 = (vert per plane) * (parts per plane)

            for(int i = 0; i < layers; i++)
            {
                // x, y, z, u, v
                // 2 . 3
                // . . .
                // 1 . 4
                //
                // therfore:
                // 1,2,4
                // 2,4,3
                int halfWidth = width/2;
                int halfHeight = Height/2;

                // 1
                _verticies[i*20+0] = -1*halfWidth;
                _verticies[i*20+1] = -1*halfHeight;
                _verticies[i*20+2] = -1*i;
                _verticies[i*20+3] = -1*0; // why are uv vertexs mapped differently than cords?
                _verticies[i*20+4] = -1*0;

                // 2
                _verticies[i*20+5] = -1*halfWidth;
                _verticies[i*20+6] =  1*halfHeight;
                _verticies[i*20+7] = -1*i;
                _verticies[i*20+8] = 0;
                _verticies[i*20+9] = 1;

                // 3
                _verticies[i*20+10] =  1*halfWidth;
                _verticies[i*20+11] =  1*halfHeight;
                _verticies[i*20+12] = -1*i;
                _verticies[i*20+13] = 1;
                _verticies[i*20+14] = 1;

                // 4
                _verticies[i*20+15] =  1*halfWidth;
                _verticies[i*20+16] = -1*halfHeight;
                _verticies[i*20+17] = -1*i;
                _verticies[i*20+18] = 1;
                _verticies[i*20+19] = 0;
            }
            // then make the indices

            _indicies = new GLint[6*layers];
            for(int i = 0; i < layers; i++)
            {
                int InOffset = i*6;
                int VeOffset = i*4;
                _indices[InOffset+0] = 0 + VeOffset;
                _indices[InOffset+1] = 1 + VeOffset;
                _indices[InOffset+2] = 3 + VeOffset;
                _indices[InOffset+3] = 1 + VeOffset;
                _indices[InOffset+4] = 3 + VeOffset;
                _indices[InOffset+5] = 2 + VeOffset;
            }
        }

    };
}
