namespace MVPMatrix
{
    class MVPMatrixes
    {
        public:
            glm::mat4 ModelMatrix;
            glm::mat4 ViewMatrix;
            glm::mat4 ProjectionMatrix;

        // ok quick run down before i write some
        // terrible code
        // View matrix only needs to be initalized, it will
        // be edited later
        // Same thing with model matrix
        // Projection matrix needs to be italized w 3 things
        // Viewport size
        // and how far back/forwards to look
        MVPMatrixes(uint8_t aspect, uint16_t width, uint16_t height, uint16_t farCloseDistance)
        {
            float half_heigh = height / 2.f; // ok go search it up, I aint explaning it here
            float half_width = width / 2.f;
            half_width = half_width / (aspect);
            half_heigh = half_heigh / (aspect);
            ProjectionMatrix = glm::ortho(-half_width, half_width, -half_heigh, half_heigh, (float)-farCloseDistance, (float)farCloseDistance);


            ViewMatrix = glm::mat4(1.0f);
            ModelMatrix = glm::mat4(1.0f);
        }

        // Make a transform func for the ViewMatrix
        void rotateView(float x, float y, float z)
        {
            ViewMatrix = glm::rotate(ViewMatrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
            ViewMatrix = glm::rotate(ViewMatrix, glm::radians(x), glm::vec3(0.0f, 1.0f, 0.0f));
            ViewMatrix = glm::rotate(ViewMatrix, glm::radians(x), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void translateView(float x, float y, float z)
        {
            ViewMatrix = glm::translate(ViewMatrix, glm::vec3(x, y, z));
        }

        // I have no a single clue how this works lmao
        void scaleView(float factor)
        {
            // ViewMatrix = glm::scale(factor, factor, factor);
        }

        private:
    };
}
