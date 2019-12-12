//
//  Skybox.cpp
//  CSE167PA4
//
//  Created by ztybigcat on 11/10/19.
//  Copyright © 2019 Tianyi Zhang. All rights reserved.
//

#include "Skybox.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
Skybox::Skybox(){
    // The 8 vertices of a cu5be.
    int l = 300;
    std::vector<glm::vec3> vertices
    {
        glm::vec3(-l, l, l),
        glm::vec3(-l, -l, l),
        glm::vec3(l, -l, l),
        glm::vec3(l, l, l),
        glm::vec3(-l, l, -l),
        glm::vec3(-l, -l, -l),
        glm::vec3(l, -l, -l),
        glm::vec3(l, l, -l)
    };

    // Each ivec3(v1, v2, v3) define a triangle consists of vertices v1, v2
    // and v3 in counter-clockwise order.
    vector<vec3> indices = vector<vec3>() ;
    
    // Right face.
    indices.push_back(vertices[6]);
    indices.push_back(vertices[2]);
    indices.push_back(vertices[3]);
    indices.push_back(vertices[3]);
    indices.push_back(vertices[7]);
    indices.push_back(vertices[6]);
    
    // Left face.
    indices.push_back(vertices[1]);
    indices.push_back(vertices[5]);
    indices.push_back(vertices[4]);
    indices.push_back(vertices[4]);
    indices.push_back(vertices[0]);
    indices.push_back(vertices[1]);
    // Top face.
    indices.push_back(vertices[3]);
    indices.push_back(vertices[0]);
    indices.push_back(vertices[4]);
    indices.push_back(vertices[4]);
    indices.push_back(vertices[7]);
    indices.push_back(vertices[3]);
    // Bottom face.
    indices.push_back(vertices[6]);
    indices.push_back(vertices[5]);
    indices.push_back(vertices[1]);
    indices.push_back(vertices[1]);
    indices.push_back(vertices[2]);
    indices.push_back(vertices[6]);
    
    // Back face.
    indices.push_back(vertices[5]);
    indices.push_back(vertices[6]);
    indices.push_back(vertices[7]);
    indices.push_back(vertices[7]);
    indices.push_back(vertices[4]);
    indices.push_back(vertices[5]);
    // Front face.
    indices.push_back(vertices[2]);
    indices.push_back(vertices[1]);
    indices.push_back(vertices[0]);
    indices.push_back(vertices[0]);
    indices.push_back(vertices[3]);
    indices.push_back(vertices[2]);
    

    
    program = LoadShaders("shaders/skybox.vert", "shaders/skybox.frag");
    glUseProgram(program);
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
   // Bind to the VAO.
    glBindVertexArray(vao);

   // Bind to the first VBO. We will use it to store the vertices.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
   // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * indices.size(),
       indices.data(), GL_STATIC_DRAW);
   // Enable vertex attribute 0.
   // We will be able to access vertices through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

   // Unbind from the VBOs.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
   // Unbind from the VAO.
    glBindVertexArray(0);
    viewLoc = glGetUniformLocation(program, "view");
    projectionLoc = glGetUniformLocation(program, "projection");
    glEnable(GL_CULL_FACE);
}
Skybox::~Skybox(){
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteProgram(program);
}
void Skybox::draw(){
    glDepthMask(GL_FALSE);
    glUseProgram(program);
    glBindVertexArray(vao);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glDepthMask(GL_TRUE);
    glBindVertexArray(0);
}

void Skybox::updateProjection(mat4 proj){
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, value_ptr(proj));
}
void Skybox::updateView(mat4 view){
    view = glm::mat4(glm::mat3(view));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
}

void Skybox::init(){
    int width, height, nrChannels;
    
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
    // Make sure no bytes are padded:
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Use bilinear interpolation:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Use clamp to edge to hide skybox edges:
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    unsigned char * image;
    char * path = (char*)"skybox/right.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    path = (char*)"skybox/left.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    path = (char*)"skybox/top.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    path = (char*)"skybox/bottom.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    path = (char*)"skybox/front.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    path = (char*)"skybox/back.jpg";
    image = stbi_load(path, &width, &height,&nrChannels,0);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    stbi_image_free(image);
    glEnable(GL_TEXTURE_CUBE_MAP);
}
