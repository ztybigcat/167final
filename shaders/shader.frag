#version 330 core
// This is a sample fragment shader.

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 color;
    vec3 pos;
};
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 fragPos;
in vec3 normal;
in vec4 fragPosLightSpace;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool flag = false;
uniform vec3 color;
uniform sampler2D shadowMap;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;
float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(normal);
    vec3 lightDir = normalize(light.pos - fragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}
void main()
{
    vec3 iNormal;
	iNormal = normalize(normal);
    if(flag){
        
        iNormal.x = (iNormal.x + 1)/2;
        iNormal.y = (iNormal.y + 1)/2;
        iNormal.z = (iNormal.z + 1)/2;
        fragColor = vec4(iNormal, sampleExtraOutput);
    }else{
		vec3 lightDir = normalize(-light.pos);
		vec3 ambient = material.ambient;
		float diff = max(dot(iNormal,lightDir),0.0);
		vec3 diffuse = (diff* material.diffuse);

		vec3 viewDir = normalize(viewPos-fragPos);
		vec3 reflectDir = reflect(-lightDir, iNormal);
		float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
		vec3 specular = (spec * material.specular);
		float shadow = ShadowCalculation(fragPosLightSpace); 
		vec3 result = light.color * (ambient + (1.0-shadow)*(diffuse + specular));
		fragColor = vec4(result*color ,0.0);

    }
}
