namespace render
{
    class Scene
    {
        // god help me write whatever the fuck this
        // war crime is
        public:

            std::atomic<bool> kys;
            GLFWwindow* window;
            GLuint VAO, VBO, EBO;
            shaders::Shader* inMyMind;
            MVPMatrix::MVPMatrixes* favoriteConvosInTheAM;
            Shaders::computeShader* dejaVu;
            Shaders::computeImageOut* halfwayThroughNovember;
            Shaders::computeImageIn* heyWorld;

            Scene(const char* windowName,uint16_t x_logical, uint16_t y_logical, uint16_t x, uint16_t y, GLfloat* vertices, GLuint* indices)
            {
                logicalWidth  = x_logical;
                logicalHeight = y_logical;
                width  = x;
                height = y;

                window = glfwCreateWindow(x, y, windowName, NULL, NULL); // well it'll only run on
                                                                                     // one monitor so eh
                if (!window)
                {
                    std::cout << "Failed to create the GLFW window\n";
                    glfwTerminate();
                }

                glfwMakeContextCurrent(window);
                glfwSwapInterval(true); // vsync lmao

                if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
                {
                    std::cout << "Failed to initialize OpenGL context" << std::endl;
                }
                glViewport(0, 0, x, y);


                // I rlly need to make this an external file but I cannot be bothered
                // to make this an external file
                // kms
                // Basically gonna ask the usr for two files which
                // include the vertices and indices
                // and the map and whatnot
                // this stuff is like never again touched so eh
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

                std::cout << "vert and frag shaders being made" << std::endl;
                inMyMind = new shaders::Shader("render/shaders/vert.glsl", "render/shaders/frag.glsl");
                std::cout << "vert and frag shaders have been made" << std::endl;
                glUseProgram(inMyMind->shaderProgram);

                // aspect, width, height, farclose distance
                favoriteConvosInTheAM = new MVPMatrix::MVPMatrixes(1,1024,1024,1000);
                inMyMind->setMat4("uProjectionMatrix", favoriteConvosInTheAM->ProjectionMatrix);
                inMyMind->setMat4("uViewMatrix", favoriteConvosInTheAM->ViewMatrix);
                inMyMind->setMat4("uModelMatrix", favoriteConvosInTheAM->ModelMatrix);
                dejaVu = new Shaders::computeShader("render/shaders/compute.glsl");
                halfwayThroughNovember = new Shaders::computeImageOut(1024,1024,0);
                heyWorld = new Shaders::computeImageIn(1024,1024,1);
                std::cout << "Somehow been completed" << '\n';
            }


            void update(GLuint* indices)
            {
                float* fakeImg = new float[4194304];
                functions::whiteSquares(fakeImg);
                heyWorld->copyDataFloat(fakeImg);
                glClearColor(0.0f, 0.5f, 0.0f, 1.0f );
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                favoriteConvosInTheAM->rotateView(0.0,0.0,0.0);
                favoriteConvosInTheAM->translateView(0.0,0.0,0.0);
                inMyMind->setMat4("uViewMatrix", favoriteConvosInTheAM->ViewMatrix);

                dejaVu->useProgram();
                glDispatchCompute(1024, 1024, 1);
                glMemoryBarrier(GL_ALL_BARRIER_BITS);

                glUseProgram(inMyMind->shaderProgram); // FIXME: AT SOME POINT
                glBindTextureUnit(0, halfwayThroughNovember->getID());
                glUniform1i(glGetUniformLocation(inMyMind->shaderProgram, "screen"), 0);
                glBindVertexArray(VAO);
                glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);

                glfwSwapBuffers(window);
                glfwPollEvents();

                if(glfwWindowShouldClose(window))
                {
                    kys = true;
                }
            }

            int getHeight()
            {
                return height;
            }
            int getWidth()
            {
                return width;
            }
            int getLogicalHeight()
            {
                return logicalHeight;
            }
            int getlogicalWidth()
            {
                return height;
            }

            bool getEnd()
            {
                return !kys;
            }

            void endScene()
            {
                glfwDestroyWindow(window);
            }

        private:

            // yeah wont be as performant but eh
            int logicalWidth, logicalHeight, width, height;

    };
}
