//
//  Skybox.hpp
//  CSE167PA4
//
//  Created by ztybigcat on 11/10/19.
//  Copyright © 2019 Tianyi Zhang. All rights reserved.
//

#define GL_SILENCE_DEPRECATION
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#ifndef Skybox_hpp
#define Skybox_hpp
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <list>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "Node.h"
#include "shader.h"


using namespace glm;
using namespace std;

class Skybox{
protected:
    GLuint vao, vbo;
    GLuint modelLoc; // Location of model in shader.
    GLuint viewLoc;
    GLuint projectionLoc;
    GLuint program;
    GLuint cubemapTexture;

public:
    Skybox();
    ~Skybox();
    void init();
    void updateProjection(mat4 proj);
    void updateView(mat4 view);
    void draw();
    GLuint getTexture(){return cubemapTexture;}

};

#endif /* Skybox_hpp */
