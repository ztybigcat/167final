#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include "Object.h"
#include "Cube.h"
#include "Geometry.h"
#include "Transform.h"
#include "TopTrans.h"
#include "Binding.h"
#include "shader.h"
#include "Light.h"
#include "Node.h"

class Window
{
public:
	static int width;
	static int height;
    static int lightSwitch;
	static int cullingCount;
    static int count;
	static double fov;
    static bool f;
	static bool debugMode;
    static bool normalFlag, cFlag, unCalc;
	static const char* windowTitle;
//	static Cube* cube;
//    static PointCloud * cubePoints;
//    static PointCloud * bunnyPoints;
//    static PointCloud * dragonPoints;
//    static PointCloud * bearPoints;
//    static Light * lightObj;
//	static Object * currentObj;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up, direction, right;
	static glm::vec3 fc, ftl, ftr, fbl, fbr, nc, ntl, ntr, nbl, nbr, normF, normN, normU, normB, normR, normL;
	static GLuint program, projectionLoc, viewLoc, modelLoc, colorLoc, flagLoc;
//    , viewPosLoc, lightPosLoc, lightColorLoc, materialAmbientLoc, materialDiffuseLoc, materialSpecularLoc, materialShininessLoc, flagLoc;
	static Transform* root, * base1, * terrian, * park;
	static Geometry* g_base1, * g_mid1, * g_mid2, * g_mid3, * g_top1, *g_terrian, * g_road, * g_park;
	static std::vector<Transform*> blocks, roads;
	//static Binding * binder;

	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
    static void updateModel(Geometry * geo);
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
	static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void mouseButtonCheck(GLFWwindow* window, double xpos, double ypos);
    static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void frustumCalc();
};

#endif
