#version 450 core

layout (location = 0) in vec4 position;
layout (location = 1) in vec2 textureCoord;
layout (location = 2) in vec3 normal;

struct LightData
{
    vec4 LightPos;

    vec3 DiffuseColor;
    float DiffuseIntensity;

    vec3 AmbientColor;
    float AmbientIntensity;

    vec3 SpecularColor;
    float SpecularIntensity;
};
#define MAX_LIGHTS 10

struct Material
{
    vec3 ambientColor;
    vec3 diffuseColor;
    vec3 specularColor;
    float shininess;
};
uniform Material u_Material;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Transform;

uniform vec3 u_CameraPos;
uniform LightData[MAX_LIGHTS] u_Lights;
uniform int u_NumLights;

uniform float u_Time;
uniform float u_Freq;
uniform float u_Amplitude;
uniform vec3 u_AxisInput;
uniform vec3 u_AxisEffect;

out vec2 vs_textureCoord;
out vec3 vs_DiffuseLight;
out vec3 vs_AmbientLight;
out vec3 vs_SpecularLight;

void main()
{
   vec4 pos = position;
   vec4 axisInput = vec4(u_AxisInput, 1.0f);

   pos += vec4(u_AxisEffect * sin(length(axisInput * pos) * u_Freq * u_Time) * u_Amplitude, 0.0f);

   gl_Position = u_Proj * u_View * u_Transform * pos;
   vs_textureCoord = textureCoord;

   vs_DiffuseLight  = vec3(0.0f, 0.0f, 0.0f);
    vs_AmbientLight  = vec3(0.0f, 0.0f, 0.0f);
    vs_SpecularLight = vec3(0.0f, 0.0f, 0.0f);
    
    for(int i = 0; i < u_NumLights; i++)
    {
        vec3 LightPos = u_Lights[i].LightPos.xyz;

        vec3 vertexInModelSpace = (u_Transform * position).xyz;
        vec3 norm = normalize((transpose(inverse(u_Transform)) * vec4(normal, 0.0f)).xyz);
        vec3 lightDir = normalize(LightPos - vertexInModelSpace);

        //if w = 0, distanceScale = 1, w = 1, scale = 1/dist
        //done this way to avoid using an if statement
        float distanceFromLight = distance(LightPos, vertexInModelSpace);
        float distanceScale = (1 / distanceFromLight) * mix(distanceFromLight, 1.0f, u_Lights[i].LightPos.w);
        
	    vs_DiffuseLight += max(dot(norm, lightDir), 0.0) * u_Lights[i].DiffuseColor * u_Lights[i].DiffuseIntensity * distanceScale * u_Material.diffuseColor;
        vs_AmbientLight += u_Lights[i].AmbientIntensity * u_Lights[i].AmbientColor * distanceScale * u_Material.ambientColor;
	    
	    vec3 viewDir = normalize(u_CameraPos - vertexInModelSpace);
	    vec3 reflectDir = reflect(-lightDir, norm);
	    vs_SpecularLight += u_Lights[i].SpecularIntensity * u_Lights[i].SpecularColor * pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess) * distanceScale * u_Material.specularColor;
    }
}