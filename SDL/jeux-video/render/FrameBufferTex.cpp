// TODO: switch this to FB instead of FrameBuffer, cause
// namming schemes :P
class FrameBufferTex
{
public:
    GLuint ID;

    // So much stuff is being assumed
    // Also you have to give the FBO id addy
    FrameBufferTex(FBO& fboIn)
    {
        glCreateTextures(GL_TEXTURE_2D, 1, &ID);
        // this should be in the texture class.... WAIT
        glTextureParameteri(ID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(ID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(ID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureStorage2D(ID, 1, GL_RGB8, ACTUAL_WINDOW_WIDTH, ACTUAL_WINDOW_HEIGH);
        glNamedFramebufferTexture(fboIn.ID, GL_COLOR_ATTACHMENT0, ID, 0);
    }
private:

};
