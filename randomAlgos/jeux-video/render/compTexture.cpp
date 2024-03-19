class computeTexture
{
public:
    int ImgWidth, ImgHeigh, numColCh;
    GLuint texture2;

    computeTexture()
    {
        std::cout << "MAKING COMPUTE SHADER TEXTURES" << '\n';

        glCreateTextures(GL_TEXTURE_2D, 1, &texture2);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        // 5 parameters
        glTextureStorage2D(texture2, 1, GL_RGBA32F, ImgWidth, ImgHeigh);
        // 9 parameters
        glTextureSubImage2D(texture2, 0, 0, 0, ImgWidth, ImgHeigh, GL_RGBA, GL_FLOAT, NULL);
        // there's no way i need to generate mipmaps-
        // RIGHT????
        // glGenerateTextureMipmap(texture2);
        // Btw threre should in theory
        // be no need to bind the texture

    }
};
