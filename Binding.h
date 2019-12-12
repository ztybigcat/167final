//
//  Binding.h
//  CSE167 P2
//
//  Created by Yichen Zhang on 10/28/19.
//  Copyright © 2019 Yichen Zhang. All rights reserved.
//

#ifndef Binding_h
#define Binding_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Node.h"

class Binding : public Node
{
private:
	glm::mat4 C;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	GLuint vao, vbo;
	GLfloat pointSize;
	glm::mat4 model;
	glm::vec3 color;
public:
	Binding(std::string objFilename, GLfloat pointSize);
	~Binding();
	bool sFlag;
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }
	void draw(glm::mat4 M, GLuint program);
	void update(bool flag);
	void setModelMatrixM(glm::mat4 M);
	void rendering();

};

#endif /* Binding_h */
