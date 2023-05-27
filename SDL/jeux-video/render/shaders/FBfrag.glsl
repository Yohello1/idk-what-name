#version 460 ore

out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;

void main()
{
    FragColor = vec4(1.0) - texture(screen, UVs);

}
