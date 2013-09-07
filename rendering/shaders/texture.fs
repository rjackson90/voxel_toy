#version 330

in vec2 UV;

out vec3 color;

uniform sampler2D texColor;

void main()
{
    color = texture( texColor, UV ).rgb;
}
