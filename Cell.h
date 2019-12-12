
#ifndef Cell_h
#define Cell_h

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
#include <GLFW/glfw3.h>

#include "Node.h"

class Cell : public Node
{
private:
	glm::mat4 M;
	glm::mat4 init;
	std::list<Node*> childs;
	bool cResult;
public:
	Cell(glm::mat4 transM);
	~Cell();

	void addChild(Node* child);
	void draw(glm::mat4 C, GLuint program);
	void update(bool flag);
	void rotation(int i);
	void detectCollision(glm::vec3 eye, glm::mat4 C);
};

#endif /* Cell_h */
