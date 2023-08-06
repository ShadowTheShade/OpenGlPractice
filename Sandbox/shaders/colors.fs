#version 330 core

out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{

//Ambient lighting
float ambientStrength = 0.125;
vec3 ambient = ambientStrength * lightColor;

//Diffuse lighting
float diffuseStrength = 0.8;
vec3 norm = normalize(Normal);
vec3 lightDir = normalize(lightPos - FragPos);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = diffuseStrength * diff * lightColor;


//Specular lighting
float specularStrength = 0.5;
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm); //Point towards trhe light source

int intensity = 32;
float spec = pow(max(dot(viewDir, reflectDir), 0.0), intensity);
vec3 specular = specularStrength * spec * lightColor;

vec3 result = (ambient + diffuse + specular) * objectColor;
FragColor = vec4(result, 1.0);
}