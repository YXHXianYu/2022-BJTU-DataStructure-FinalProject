#version 330 core

struct Material {
    bool have_texture;
    vec3 ka;
    vec3 kd;
    vec3 ks;
    vec3 ke;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};
uniform Material material;

struct Light {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

void main() {
    vec3 ka, kd, ks;
    if(material.have_texture) {
        ka = vec3(texture(material.texture_diffuse1,TexCoords));
        kd = ka;
        ks = vec3(texture(material.texture_specular1,TexCoords));
    } else {
        ka = material.ka;
        kd = material.kd;
        ks = material.ks;
    }

    // ambient
    vec3 ambient = ka * light.ambient;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * kd * light.diffuse;
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular =  spec * ks * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}
