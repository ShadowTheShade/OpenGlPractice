#version 330 core

//Material of the object
struct Material
{
sampler2D diffuse;
sampler2D specular;
float shininess;
};

uniform Material material;

//Directional Light on the object
struct DirectionalLight
{
vec3 direction;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

#define NUM_DIR_LIGHTS 1
uniform DirectionalLight directionalLights[NUM_DIR_LIGHTS];

struct PointLight
{
vec3 position;

float constant;
float linear;
float quadratic;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

#define NUM_POINT_LIGHTS 1
uniform PointLight pointLights[NUM_POINT_LIGHTS];

struct SpotLight
{
vec3 position;
vec3 direction;

float cutOff;
float outerCutOff;

float constant;
float linear;
float quadratic;

vec3 ambient;
vec3 diffuse;
vec3 specular;
};

#define NUM_SPOT_LIGHTS 0
//uniform SpotLight spotLights[NUM_SPOT_LIGHTS];


out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

uniform vec3 viewPos;

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 FragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir);

void main()
{

vec3 result = vec3(0.0);

vec3 norm = normalize(Normal);
vec3 viewDir = normalize(viewPos - FragPos);

for(int i = 0; i < NUM_DIR_LIGHTS; i++)
result += CalcDirLight(directionalLights[i], norm, viewDir);

for(int i = 0; i < NUM_POINT_LIGHTS; i++)
result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);

//for(int	i =	0; i < NUM_SPOT_LIGHTS;	i++)
//result += CalcSpotLight(spotLights[i], norm, FragPos, viewDir);

FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
vec3 lightDir = normalize(-light.direction);
//diffuse shading
float diff = max(dot(normal, lightDir), 0.0);
//specular shading
vec3 reflectDir = reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//combine results
vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
vec3 specular =	light.specular * spec * vec3(texture(material.specular, TexCoords));
return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
vec3 lightDir =	normalize(light.position - fragPos);
//diffuse shading
float diff = max(dot(normal, lightDir),	0.0);
//specular	shading
vec3 reflectDir	= reflect(-lightDir, normal);
float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//attenuation
float distance	  =	length(light.position -	fragPos);
float attenuation =	1.0	/ (light.constant +	light.linear * distance	+
light.quadratic	* (distance	* distance));
//combine results
vec3 ambient  =	light.ambient  * vec3(texture(material.diffuse,	TexCoords));
vec3 diffuse  =	light.diffuse  * diff *	vec3(texture(material.diffuse, TexCoords));
vec3 specular =	light.specular * spec *	vec3(texture(material.specular,	TexCoords));
ambient	 *=	attenuation;
diffuse	 *=	attenuation;
specular *=	attenuation;
return (ambient	+ diffuse +	specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 norm, vec3 fragPos, vec3 viewDir)
{
//vec3 lightDir = normalize(-light.direction); //Directional light | Negated to be made pointing towards the light source
vec3 lightDir = normalize(light.position - fragPos); //Point light | 

float theta = dot(lightDir, normalize(-light.direction));
float epsilon = light.cutOff - light.outerCutOff;
float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0); //Clamp sets bounds

if(theta > light.cutOff)
{

//Ambient lighting
vec3 ambient = light.ambient * texture(material.diffuse, TexCoords).rgb;

//Diffuse lighting
vec3 norm = normalize(Normal);

float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoords).rgb;

//Specular lighting
vec3 viewDir = normalize(viewPos - FragPos);
vec3 reflectDir = reflect(-lightDir, norm); //Point towards trhe light source

float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
vec3 specular = light.specular * spec * texture(material.specular, TexCoords).rgb;

diffuse *= intensity;
specular *= intensity;


//Point light based on distance
float distance = length(light.position - FragPos);
float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
ambient *= attenuation;
diffuse *= attenuation;
specular *= attenuation;

return (ambient + diffuse + specular);
}
else
return vec3(light.ambient * vec3(texture(material.diffuse, TexCoords)));

}
