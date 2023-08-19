// there is no good or evil in the end

class game
{
    public:
        GLFWwindow* window;
        unsigned int currenttime = (unsigned int)time(NULL);

        game(int x, int y, const char* winName)
        {
            srand(current_time);

            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

            GLFWwindow* window = glfwCreateWindow(ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH, "Gament2", NULL, NULL);
            if (!window)
            {
                std::cout << "Failed to create the GLFW window\n";
                glfwTerminate();
            }
            glfwMakeContextCurrent(window);
            glfwSwapInterval(true);

            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                std::cout << "Failed to initialize OpenGL context" << std::endl;
            }
            glViewport(0, 0, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);

        }
    private:

};
