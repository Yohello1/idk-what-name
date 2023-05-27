class Texture {
public:
    int ImgWidth, ImgHeigh, numColCh;
    GLuint texture;

    Texture(const char* filePath)
    {
        stbi_set_flip_vertically_on_load(true);
        unsigned char* rawData = stbi_load(filePath, &ImgWidth, &ImgHeigh, &numColCh, 0);

        // why is it plural
        glCreateTextures(GL_TEXTURE_2D, 1, &texture);

        // idc what other ppl say,
        // im gonna use these settings whether they
        // like it or not
        glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // TIME FOR 4D TEXTURES
        // one level btw
        // ALL THE THINGS SHE SAID
        // RUNNING THROUGH MY HEAD
        // THIS IS NOT ENOUGH
        glTextureStorage2D(texture, 1, GL_RGBA8, ImgWidth, ImgHeigh);
        // in theory i should be
        // providing a method to
        // specifiy the offsets
        // but eh
        glTextureSubImage2D(texture,0,0,0, ImgWidth, ImgHeigh, GL_RGBA, GL_UNSIGNED_BYTE, rawData);
        // In theory, this will fuck over the display method
        // but eh
        glGenerateTextureMipmap(texture);

        stbi_image_free(rawData);
    }

    //TODO: create some method to delete a texture


};
