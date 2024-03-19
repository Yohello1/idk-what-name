// Oh dam 

namespace input
{
    namespace mouse 
    {
        int x_pos, y_pos;

        void update()
        {
            glfwGetMousePos(&x_pos, &y_pos);
        }

        void update_and_set()
        {
            glfwGetMousePos(&x_pos, &y_pos);
            glfwSetMousePos(ACTUAL_WINDOW_WIDTH/2, ACTUAL_WINDOW_WIDTH/2);
        }
    }

    namespace kby 
    {

    }
}