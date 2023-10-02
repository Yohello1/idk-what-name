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
            Game(const char* gameName, const char* sceneName, unsigned int map_x, unsigned int map_y) // wtf else do I put LMAO
            {
                std::memcpy(&name, sceneName, sizeof(sceneName));

                background = new float[map_x*map_y*4];
                foreground = new float[map_x*map_y*4];
                effects = new float[map_x*map_y*4];
                userInterface = new float[map_x*map_y*4];
            }
            
        private:
            const char* name[64];

            uint16_t* background;
            uint16_t* foreground;
            uint16_t* effects;
            uint16_t* userInterface;
    };
}
