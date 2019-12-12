#ifndef _OBJECT_H_
#define _OBJECT_H_

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

class Object
{
protected:
	glm::mat4 model;
	glm::vec3 color;
public:
	glm::mat4 getModel() { return model; }
	glm::vec3 getColor() { return color; }

	virtual void draw() = 0;
	virtual void update() = 0;
    virtual void updatePointSizeL() = 0;
    virtual void updatePointSizeS() = 0;
    virtual void updateModelSize(double offset) = 0;
    virtual void spin(float angle, glm::vec3 axis) = 0;
};

#endif

