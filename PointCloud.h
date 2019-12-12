#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
	GLuint vao, vbo;
	GLfloat pointSize;
public:
	PointCloud(std::string objFilename, GLfloat pointSize);
	~PointCloud();

	void draw();
	void update();

	void updatePointSizeL();
    void updatePointSizeS();
    void updateModelSize(double offset);
	void spin(float angle, glm::vec3 axis);
};

#endif

