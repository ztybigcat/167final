#ifndef Node_h
#define Node_h

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>

class Node
{
protected:
public:
    virtual void draw(glm::mat4 C, GLuint program) = 0;
    virtual void update(bool flag) = 0;
};

#endif /* Node_h */
