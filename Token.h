
#ifndef Token_h
#define Token_h

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

class Token : public Node
{
private:
	glm::mat4 M;
	glm::mat4 init;
	std::list<Node*> childs;
public:
	Token(glm::mat4 transM);
	~Token();

	void addChild(Node* child);
	void draw(glm::mat4 C, GLuint program);
	void update(bool flag);
	void rotation(int i);
	void detectCollision(glm::vec3 eye, glm::mat4 C);
};

#endif /* Token_h */
