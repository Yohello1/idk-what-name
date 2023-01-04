#version 430 

layout (location = 0) in vec2 pos;


// attribute vec4 VPosition;

uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
// uniform mat4 umodelMatrix;

void main()
{
   vec4 final_pos = uProjectionMatrix * uViewMatrix * vec4(pos, 0.0, 1.0);
   gl_Position = final_pos;
};