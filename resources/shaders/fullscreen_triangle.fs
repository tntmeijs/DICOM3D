#version 460 core

in vec2 texCoord;
out vec4 glColor;

uniform sampler2D volumetricResult;

void main()
{
    glColor = texture(volumetricResult, texCoord);
}
