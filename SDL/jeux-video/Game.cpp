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
                    -64.0f, -64.0f , 0.0f, 0.3f, 0.0f,
                    -64.0f,  64.0f , 0.0f, 0.3f, 1.0f,
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
        std::string _name;
        unsigned int _mapX;
        unsigned int _mapY;

    };
}
