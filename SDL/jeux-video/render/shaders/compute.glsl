#version 460 core
layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
void main()
{

    vec4 pixel = vec4(0.0, 0.0, 0.0, 1.0);
	ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    pixel.x = float(pixel_coords.x)/(gl_NumWorkGroups.x);
    pixel.y = float(pixel_coords.y)/(gl_NumWorkGroups.y);

	imageStore(screen, pixel_coords, pixel);
}
