#version 330 core
// This is a sample fragment shader.

//struct Material {
//    vec3 ambient;
//    vec3 diffuse;
//    vec3 specular;
//    float shininess;
//};
//struct Light {
//    vec3 color;
//    vec3 pos;
//};
// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
//in vec3 fragPos;
in vec3 normal;

//uniform vec3 viewPos;
//uniform Material material;
//uniform Light light;
uniform bool flag = false;
uniform vec3 color;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 fragColor;

void main()
{
    vec3 iNormal;
    if(flag){
        iNormal = normalize(normal);
        iNormal.x = (iNormal.x + 1)/2;
        iNormal.y = (iNormal.y + 1)/2;
        iNormal.z = (iNormal.z + 1)/2;
        fragColor = vec4(iNormal, sampleExtraOutput);
    }else{
//        float lightAtt = 1.0f/(1.0f+0.05f*length(light.pos-fragPos));//pow(length(light.pos-fragPos),2));
//        vec3 ambient = light.color * material.ambient;
//        vec3 norm = normalize(normal);
//        vec3 lDir = normalize(light.pos - fragPos);
//        float diff = max(dot(norm, lDir), 0.0f);
//        vec3 diffuse = light.color * (diff * material.diffuse);
//
//        vec3 rDir = reflect(-lDir, norm);
//        vec3 vDir = normalize(viewPos - fragPos);
//        float spec = pow(max(dot(vDir, rDir), 0.0f), material.shininess);
//        vec3 specular = light.color * (spec * material.specular);
    // Use the color passed in. An alpha of 1.0f means it is not transparent.
//        fragColor = vec4((ambient+lightAtt*(diffuse+specular)), sampleExtraOutput);
        fragColor = vec4((color), sampleExtraOutput);

    }
}
