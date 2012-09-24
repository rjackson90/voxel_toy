#version 420


uniform mat4 mvp;

in vec3 vVertex;
in vec3 vNormal;
in vec3 vColor;

out vec4 vVaryingColor;

void main(void)
{
    vVaryingColor = vec4(vColor, 1.0f);
    gl_Position = vec4(vVertex, 1.0f);
}
