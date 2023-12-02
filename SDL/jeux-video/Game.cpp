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
        }

        // layerNum is the layer number
        // Offset is the offset of the actual data, assuming in the same layer
        // There are no validity checks :P
        // value is the value it will be updated to
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
            // tiddy muncher 5000
            for(int i = _layers.size(); i > 0; i--)
            {
                free(_layers.at(0));
            }
        }

        private:
        std::string _name;
        unsigned int _mapX;
        unsigned int _mapY;

    };
}
