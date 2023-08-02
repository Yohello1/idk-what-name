#version 460 core
layout (location = 0) in vec3 pos; // vert 3d position
layout (location = 1) in vec2 uvs; // vert texture position

// uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;

out vec2 UVs;
void main()
{
	gl_Position = uViewMatrix * vec4(pos, 1.0); // btw it has to be projection*view*vec4(1)
	// gl_Position = vec4(pos.x, pos.y, pos.z, 1.000);
	UVs = uvs;
}
