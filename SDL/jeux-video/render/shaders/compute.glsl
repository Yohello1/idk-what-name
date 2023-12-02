#version 460 core

layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D screen;
layout(rgba32f, binding = 1) uniform image2D one;
layout(rgba32f, binding = 2) uniform image2D two;
layout(rgba32f, binding = 3) uniform image2D three;

void main()
{

    vec4 pixel = vec4(0.0, 0.0, 1.0, 0.05);
    ivec2 pixel_coords = ivec2(gl_GlobalInvocationID.xy);

    vec4 pixel3 = imageLoad(one,   pixel_coords);
    vec4 pixel4 = imageLoad(two,   pixel_coords);
    vec4 pixel5 = imageLoad(three, pixel_coords);

    pixel.x = pixel3.x;
    pixel.y = pixel4.y;
    pixel.z = pixel5.z;
    pixel.a = 1.0;

    imageStore(screen, pixel_coords, pixel);
}
