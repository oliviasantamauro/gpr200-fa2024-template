#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform float shine;
uniform float ambientK;
uniform float diffuseK;
uniform float specularK;


uniform sampler2D texture1;

void main()
{
    // Ambient
    vec3 ambient = ambientK * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuseK * lightColor;
    
    // Specular
    vec3 viewDir    = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), shine);
    vec3 specular = specularK * spec * lightColor;
            
    vec3 result = ambient + diffuse + specular;
    
    vec4 texColor = texture(texture1, TexCoord);
    
    FragColor = vec4(result * texColor.rgb, texColor.a);
}