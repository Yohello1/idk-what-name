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
        std::vector<float *> layers;
        MVPMatrix::MVPMatrixes* transforms;

        Game(std::string sceneName, unsigned int map_x, unsigned int map_y, uint8_t layersAmt, int aspect, int width, int height, int farCloseDistance) // wtf else do I put LMAO
        {
            name = sceneName;
            mapX = map_x;
            mapY = map_y;

            for(uint8_t i = 0; i < layersAmt; i++)
            {
                layers.push_back(new float[map_x*map_y*4*layersAmt]);
            }

            transforms = new MVPMatrix::MVPMatrixes(aspect, width, height, farCloseDistance);
        }

        // layerNum is the layer number
        // Offset is the offset of the actual data, assuming in the same layer
        // There are no validity checks :P
        // value is the value it will be updated to
        void updateValue(int layerNum, int offset, float value)
        {
            // layers[offset+layerNum*mapX*mapY*4] = value;

        }

        // No validity check, part is the like individual offset(?)
        // like 0,1,2,3
        unsigned int findOffset(unsigned int row, unsigned int col, unsigned int part)
        {
            unsigned int temp = row*mapX*4 + 4*col + part;
            return temp;
        }

        void updateImageBulk(int layerNum, float* data)
        {
            // here memcpy
            std::copy(data, data + mapX*mapY*4, layers.at(layerNum));

        }

        void rotateView(float x, float y, float z)
        {
            transforms->rotateView(x,y,z);
        }

        void translateView(float x, float y, float z)
        {
            transforms->translateView(x,y,z);
        }


        // I cannot be bothered to get this working
        std::string getName()
        {
            // if(name == NULL)
            //     return "";
            return name;
        }

        void changeTitle(GLFWwindow* window)
        {
            glfwSetWindowTitle(window, name.c_str());
        }

        private:
        std::string name;
        unsigned int mapX;
        unsigned int mapY;

    };
}
