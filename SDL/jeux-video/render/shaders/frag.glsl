#version 460 core

out vec4 FragColor;
uniform sampler2D tex;
in vec2 UVs;

void main()
{
	FragColor = vec4(0.965, 0.318, 0.000, 1.000);
	// FragColor = texture(tex, UVs);
}