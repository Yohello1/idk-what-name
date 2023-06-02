#version 460 core

out vec4 FragColor;
uniform sampler2D tex;
in vec2 UVs;

void main()
{
	FragColor = vec4(1.0f);
	FragColor = texture(tex, UVs);
}
