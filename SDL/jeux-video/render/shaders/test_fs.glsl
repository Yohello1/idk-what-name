#version 430

out vec4 FragColor;

in vec3 ourColor;
in vec2 texCoord;

uniform sampler2D tex0;

void main()
{
    float r = gl_FragCoord.x / 1024.0;
    float g = gl_FragCoord.y / 1024.0;
    float b = 1.0;
    float a = 1.0;
    vec2 temp = vec2(45,45);
    FragColor = texture(tex0, temp);
}

/*
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
    // if I hadnt hit the bottom the amount of times I did, I dont think I would be the person i would be today

    float r = gl_FragCoord.x / 1024.0;
    float g = gl_FragCoord.y / 1024.0;
    float b = uColor_temp.x;
    float a = uColor_temp.y;
    fragColor = vec4(r, g, b, a);
        // FragColor = texture(ourTexture, TexCoord);

    // fragColor = vec4(uColor_temp, 1.0);
}
*/