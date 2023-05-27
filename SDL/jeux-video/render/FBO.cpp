class FBO
{
  // I forgot to make this and am going insane
public:
    GLuint ID;

    FBO()
    {
        glCreateFramebuffers(1, &ID);
    }

    // it just tells u the status of the Frame buffer
    void fboWorking()
    {
        auto fboStatus = glCheckNamedFramebufferStatus(ID, GL_FRAMEBUFFER);
        if(fboStatus != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "Frame Buffer Error: " << fboStatus << '\n';
        }
    }
private:

};
