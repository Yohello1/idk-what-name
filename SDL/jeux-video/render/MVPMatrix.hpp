namespace MVPMatrix
{
    class MVPMatrixes
    {
        public:
            glm::mat4 _ModelMatrix;
            glm::mat4 _ViewMatrix;
            glm::mat4 _ProjectionMatrix;
            float _halfHeight,_halfWidth, _aspect, _farCloseDistance;
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
            _halfHeight = (height / 2.f) / (aspect); // ok go search it up, I aint explaning it here
            _halfWidth = (width / 2.f) / aspect;
            _aspect = aspect;
            _farCloseDistance = farCloseDistance;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-farCloseDistance, (float)farCloseDistance);

            _ViewMatrix = glm::mat4(1.0f);
            _ModelMatrix = glm::mat4(1.0f);
        }

        // Past me: Make a transform func for the ViewMatrix
        // Future me: Did I not do that?
        void rotateView(float x, float y, float z)
        {
            _ViewMatrix = glm::rotate(_ViewMatrix, glm::radians(x), glm::vec3(1.0f, 0.0f, 0.0f));
            _ViewMatrix = glm::rotate(_ViewMatrix, glm::radians(y), glm::vec3(0.0f, 1.0f, 0.0f));
            _ViewMatrix = glm::rotate(_ViewMatrix, glm::radians(z), glm::vec3(0.0f, 0.0f, 1.0f));
        }

        void translateView(float x, float y, float z)
        {
            _ViewMatrix = glm::translate(_ViewMatrix, glm::vec3(x,y,z));
        }

        // Its not actually scaling in the tradtional sense
        // its changing the projection matrix, to make it smaller or wtv
        // btw we assume that farCloseDistance remains constant
        void scaleView(float aspect, uint16_t width, uint16_t height)
        {
            _halfHeight = (height / 2.f) / (aspect); // ok go search it up, I aint explaning it here
            _halfWidth = (width / 2.f) / aspect;
            _aspect = aspect;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-_farCloseDistance, (float)_farCloseDistance);
        }

        void scaleView(float newAspect)
        {
            _halfHeight /= newAspect; // ok go search it up, I aint explaning it here
            _halfWidth /= newAspect;
            _aspect = newAspect;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-_farCloseDistance, (float)_farCloseDistance);
        }


        private:
    };
}
