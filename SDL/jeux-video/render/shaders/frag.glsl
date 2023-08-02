#version 460 core

out vec4 FragColor;
uniform sampler2D screen;
uniform mat4 uViewMatrix;
in vec2 UVs;

void main()
{
	FragColor = texture(screen, UVs);
}
