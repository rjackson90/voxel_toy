#version 330

in vec2 UV;
smooth in vec3 vSmoothLightDir;

out vec4 vFragColor;

uniform mat3 normalMatrix;
uniform vec4 ambientColor;
uniform sampler2D texColor;
uniform sampler2D texNormal;

void main()
{
    // Transform the normal vector for this fragment
    vec3 vNormal = normalMatrix * texture(texNormal, UV).rgb;

    // Dot product give diffuse intensity
    float diffuse_intensity = max(0.0, dot(normalize(vNormal),
                                            normalize(vSmoothLightDir)));
    
    // Multiply intensity by diffuse color from texture
    vFragColor = diffuse_intensity * texture(texColor, UV);

    // Add in ambient light (maybe don't do this?????)
    vFragColor += ambientColor;
}
