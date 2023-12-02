#version 460 core

out vec4 FragColor;
uniform sampler2D screen;
in vec2 UVs;

void main()
{
    FragColor = vec4(0.0,0.0,0.0,0.0);
	vec4 tempColor = texture(screen, UVs);
    float tempVal = tempColor.a;
    if(tempVal < 0.5)
    {
        FragColor = vec4(0.0,0.0,0.0,1.0);
        discard;
    }
    FragColor = tempColor;
}
