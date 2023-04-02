#include "shader_class.h"

namespace Textures
{
    class Texture
    {
        private:
            unsigned int m_RendererID;
            std::string m_FilePath; // File path
            unsigned char* m_LocalBuffer; // Buffer of the image
            int m_Width, m_Height, m_BPP; // width, heigh, and bits per pixel
        public:
            Texture(const std::string& path);
            ~Texture();

            void Bind() const;
            void Unbind();

            inline int GetWidth() const {return m_Width; }
            inline int GetHeight() const {return m_Height; }

    };


};
