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

uniform vec3 viewPos;
uniform Material material;
uniform Light light;
uniform bool flag = false;
uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

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

		vec3 result = light.color * (ambient + diffuse + specular);
		fragColor = vec4(result*color ,0.0);

    }
}
