#version 420

uniform vec4 inputColour;
out vec4 fragColour;

void main() {
  fragColour = inputColour;
}