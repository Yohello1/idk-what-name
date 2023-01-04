#version 430
out vec4 fragColor;
uniform vec3 uColor_temp;
uniform int logical_width;
uniform int logical_heigh;

void main()
{
    // gl_FragCoord contains the window relative coordinate for the fragment.
    // we use gl_FragCoord.x position to control the red color value.
    // we use gl_FragCoord.y position to control the green color value.
    // please note that all r, g, b, a values are between 0 and 1.

    float r = gl_FragCoord.x / 1024.0;
    float g = gl_FragCoord.y / 1024.0;
    float b = uColor_temp.x;
    float a = uColor_temp.y;
    fragColor = vec4(r, g, b, a);
    // fragColor = vec4(uColor_temp, 1.0);
}