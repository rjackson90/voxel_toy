#version 330

in vec2 UV;

out vec4 vFragColor;

uniform sampler2D texColor;

layout (std140) uniform TransformBlock
{
    mat4 mvp;
    mat4 mv;
    mat3 normal;
}transform;

void main()
{
    // The fragment color is the same as the color of the texture
    vFragColor = texture(texColor, UV);
}
