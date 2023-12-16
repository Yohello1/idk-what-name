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
        MVPMatrixes(float aspect, uint16_t width, uint16_t height, uint16_t farCloseDistance)
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
            _halfHeight = (height / 2.f) / (aspect);
            _halfWidth = (width / 2.f) / aspect;
            _aspect = aspect;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-_farCloseDistance, (float)_farCloseDistance);
        }

        void scaleView(float newAspect)
        {
            _halfHeight /= newAspect;
            _halfWidth /= newAspect;
            _aspect = newAspect;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-_farCloseDistance, (float)_farCloseDistance);
        }

        // I both wish I did, and didnt get into that mess
        // I hope the person is smart enough to relise dating
        // is a bad idea, and being friends is good enough


        // basically creates a new projection matrix LMAO
        void makeProjection(float aspect, uint16_t width, uint16_t height, uint16_t farCloseDistance)
        {
            _halfHeight = (height / 2.f) / (aspect); // ok go search it up, I aint explaning it here
            _halfWidth = (width / 2.f) / aspect;
            _aspect = aspect;
            _farCloseDistance = farCloseDistance;
            _ProjectionMatrix = glm::ortho(-_halfWidth, _halfWidth, -_halfHeight, _halfHeight, (float)-farCloseDistance, (float)farCloseDistance);
        }

        // having a model matrix would make sense
        // like me not talking would make sense
        // yet here we are, so we dont talk abt it
        // we just try and reconcile
        // and dont talk abt it
        // I feel bad for them, cause now
        // its like people who were their friend
        // are no longer their friend :( because of that
        // now they dont rlly have people to fall back on oh well
        // Sadge
        private:
    };
}
