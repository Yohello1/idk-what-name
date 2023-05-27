#version 460 core
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 uvs;

out vec2 UVs;

void main()
{
    gl_Position = vec4(2.0 * pos.x, 2.0 * pos.y, 2.0 * pos.z, 1.000);
    UVs = uvs;
}
