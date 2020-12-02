#shader vertex
#version 330 core

in vec3 position;
in vec2 texcord;
in vec3 normals;

out vec3 outnormals;
out vec2 TexCoord;
out vec3 vposition;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    vposition = vec3(model * vec4(position, 1.0));
    outnormals = mat3(transpose(inverse(model))) * normals;

    gl_Position = projection * view * model * vec4(vposition, 1.0f);
    TexCoord = vec2(texcord.x, 1.0 - texcord.y);
}

#shader fragment
#version 330 core
in vec3 outnormals;
in vec3 vposition;
in vec2 TexCoord;

out vec4 outColor;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // ambient
    float ambientStrength = 0.05;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse 
    vec3 norm = normalize(outnormals);
    vec3 lightDir = normalize(lightPos - vposition);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular (Blinn-Phong)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - vposition);
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 64.0);
    
    vec3 specular = specularStrength * spec * lightColor;

    // simple attenuation
    float max_distance = 1.5;
    float distance = length(lightPos - vposition);
    float attenuation = 1.0 / (distance * distance);
    
    ambient = ambient * attenuation;
    diffuse = diffuse * attenuation;
    specular = specular * attenuation;

    vec3 result = (ambient + diffuse + specular) * objectColor * mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.9);
    result = pow(result, vec3(1.0/2.2));    
    outColor = vec4(result, 1.0);
}

