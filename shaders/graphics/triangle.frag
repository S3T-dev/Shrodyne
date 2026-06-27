#version 450
layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 fragNormal;
layout(location = 0) out vec4 outColor;

void main() {
    vec3 normal = normalize(fragNormal);
    vec3 lightDir = normalize(vec3(0.4, 0.8, 0.6));
    float diffuse = max(dot(normal, lightDir), 0.0);
    vec3 ambient = vec3(0.12, 0.12, 0.16);
    vec3 litColor = fragColor * (ambient + vec3(diffuse * 0.85));
    outColor = vec4(litColor, 1.0);
}
