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
    int type;
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform int numberOfLights;
uniform Light lights[5];

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform vec3 viewPos;

uniform bool onlySpecular;

uniform bool enableBlinnPhong;
uniform int renderMode;
uniform int lightSource;
uniform float hdrExposure;

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

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 ambient = vec3(0.f, 0.f, 0.f);
    vec3 diffuse = vec3(0.f, 0.f, 0.f);
    vec3 specular = vec3(0.f, 0.f, 0.f);
    for(int i = 0; i < numberOfLights; i++) {
        if(lightSource != 0 && (lightSource - 1) != i) continue;

        ambient += ka * lights[i].ambient;

        vec3 lightDir;
        if(lights[i].type == 0)
            lightDir = normalize(lights[i].position - FragPos);
        else if(lights[i].type == 1)
            lightDir = normalize(-lights[i].direction);
        else
            lightDir = vec3(1.f, 1.f, 1.f);
        diffuse += max(dot(norm, lightDir), 0.0) * kd * lights[i].diffuse;

        if(enableBlinnPhong) {
            vec3 halfwayDir = normalize(lightDir + viewDir);
            float delta = max(dot(norm, halfwayDir), 0.0);
            if(delta <= 0) delta = 0;
            else if(delta <= 0.5f) delta = 0.25f;
            else if(delta <= 0.9f) delta = 0.75f;
            else delta = 1.f;
            specular += pow(delta, material.shininess) * ks * lights[i].specular;
        } else {
            vec3 reflectDir = reflect(-lightDir, norm);
            specular += pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * ks * lights[i].specular;
        }
    }

    vec3 result;

    if(renderMode == 0) {
        if(onlySpecular)
            result = specular;
        else
            result = (ambient + diffuse + specular);
    } else if(renderMode == 1) {
        result = ambient;
    } else if(renderMode == 2) {
        result = diffuse;
    } else if(renderMode == 3) {
        result = specular;
    } else if(renderMode == 4) {
        result = norm;
    }


    if(hdrExposure > 0.05f) {
        const float gamma = 2.2f;
        result = pow(vec3(1.0) - exp(-result * hdrExposure), vec3(1.0 / gamma));
    }

    FragColor = vec4(result, 1.f);
}
