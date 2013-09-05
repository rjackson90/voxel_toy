#version 330

in vec3 vVertex;
in vec3 vNormal;
in vec3 vColor;
in vec2 vUV;

out vec2 UV;

uniform mat4 mvp;

void main()
{
    gl_Position = mvp * vec4(vVertex, 1.0f);
    UV = vUV;
}
