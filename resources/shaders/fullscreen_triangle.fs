#version 460 core

in vec2 texCoord;

out vec4 glColor;

void main()
{
    glColor = vec4(texCoord, 1.0, 1.0);
}
