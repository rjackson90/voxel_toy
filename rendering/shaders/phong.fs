#version 330

in vec2 uv;
in vec3 direction;

out vec4 vFragColor;

layout(std140) uniform PointLight
{
    vec3 position;
    vec4 intensity;
}light;

layout(std140) uniform Material
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
}material;

uniform sampler2D texColor;
uniform sampler2D texNormal;

layout (std140) uniform TransformBlock
{
    mat4 mvp;
    mat4 mv;
    mat3 normal;
}transform;

void main()
{
    // Transform the normal vector for this fragment
    vec3 vNormal = normalize(transform.normal * texture(texNormal, uv).rgb);

    // Dot product give diffuse intensity
    float diffuse_intensity = max(0.0, dot(vNormal, direction));
    
    // Multiply intensity by diffuse color from texture
    vFragColor = diffuse_intensity * texture(texColor, uv);

    // Add in ambient light (maybe don't do this?????)
    vFragColor += ambientColor;
}
