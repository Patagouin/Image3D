#version 330

uniform mat4 mvpMatrix;

in vec4 vertex;
in float inGrey;

out float grey;

void main(void)
{
    grey = inGrey;
    gl_Position = mvpMatrix * vertex;
}
