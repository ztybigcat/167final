
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

class Light : public Object
{
private:
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> vertices;
    GLuint lvao, lvbo;
    glm::vec3 loc;
    
public:
    Light(glm::vec3 Location);
    ~Light();

    void draw();
    void update();
    glm::vec3 returnLocation();
    void updatePointSizeL();
    void updatePointSizeS();
    void updateModelSize(double offset);
    void spin(float angle, glm::vec3 axis);
};

