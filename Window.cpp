#include "Window.h"
#include "pathtools.h"
int Window::width;
int Window::height;
bool Window::normalFlag = false;
int Window::lightSwitch = 0;
bool flag = false;
bool Window::cFlag = false;
bool Window::unCalc = true;
double Window::fov = 60.0;
bool buttonDown = false;
glm::vec3 lastPoint;
bool Window::f = true;
bool Window::debugMode = false;
int Window::count = 0;
int Window::cullingCount = 0;

const char* Window::windowTitle = "GLFW Starter Project";

Skybox* Window::skybox;

// Objects to display.
//
//Light * Window::lightObj;
int blockStatus[100] = {}; //0 for building, 1 for park
int tokenStatus[360] = {}; //0 for gold, 1 for bomb

std::vector<Transform*> Window::blocks = std::vector<Transform*>(100);
std::vector<Transform*> Window::roads = std::vector<Transform*>(18); //0-8 vertical left to right, 9-17 herizontal bottom to top
std::vector<Transform*> Window::tokens = std::vector<Transform*>(360);//20 per road
Transform * Window::root;
Transform * Window::base1;
Transform * Window::terrian;
Transform * Window::park;
Transform* Window::win;
Transform* Window::allTokens;
//Transform * Window::bindingS;

//Geometry * Window::cube;
Geometry * Window::g_base1;
Geometry * Window::g_mid1;
Geometry * Window::g_mid2;
Geometry * Window::g_mid3;
Geometry * Window::g_top1;
Geometry* Window::g_terrian;
Geometry* Window::g_road;
Geometry* Window::g_park;
Geometry* Window::g_win;
Geometry* Window::g_gold;
Geometry* Window::g_bomb;

//Binding * Window::binder;

glm::vec3 Window::fc;
glm::vec3 Window::ftl;
glm::vec3 Window::ftr;
glm::vec3 Window::fbl;
glm::vec3 Window::fbr;
glm::vec3 Window::nc;
glm::vec3 Window::ntl;
glm::vec3 Window::ntr;
glm::vec3 Window::nbl;
glm::vec3 Window::nbr;
glm::vec3 Window::normF;
glm::vec3 Window::normN;
glm::vec3 Window::normU;
glm::vec3 Window::normB;
glm::vec3 Window::normL;
glm::vec3 Window::normR;

// The object currently displaying.
//Object * Window::currentObj;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(-0.5f, 18.0f, -10.0f); // Camera position.
//glm::vec3 Window::eye(28.0f, 500.0f, 148.0f); // Camera position.
glm::vec3 Window::center(0.0f, 18.0f, 148.0f); // The point we are looking at.
//glm::vec3 Window::center(28.0f, 2.0f, 148.0f); // The point we are looking at.
glm::vec3 Window::up(0.0f, 1.0f, 0.0f); // The up direction of the camera.
//glm::vec3 Window::up(1.0f, 0.0f, 0.0f); // The up direction of the camera.

//glm::vec3 Window::eye(-0.5f, 18.0f, -10.0f);

glm::vec3 Window::direction = glm::normalize(Window::center - Window::eye);
glm::vec3 Window::right = glm::cross(Window::up, - Window::direction);

//glm::vec3 materialAmbient;
//glm::vec3 materialDiffuse;
//glm::vec3 materialSpecular;

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);


GLuint Window::program; // The shader program id.

GLuint Window::projectionLoc; // Location of projection in shader.
GLuint Window::viewLoc; // Location of view in shader.
GLuint Window::modelLoc; // Location of model in shader.
GLuint Window::colorLoc; // Location of color in shader.
GLuint Window::viewPosLoc; //Location of viewLoc in shader.
GLuint Window::lightPosLoc; //Location of light position in shader.
GLuint Window::lightColorLoc; //Location of light ambient in shader.
GLuint Window::materialAmbientLoc; //Location of material ambient in shader.
GLuint Window::materialDiffuseLoc; //Location of material diffuse in shader.
GLuint Window::materialSpecularLoc; //Location of material specular in shader.
GLuint Window::materialShininessLoc; //Location of material shininess in shader.
GLuint Window::flagLoc;

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
	program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");

	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	modelLoc = glGetUniformLocation(program, "model");
	colorLoc = glGetUniformLocation(program, "color");
    viewPosLoc = glGetUniformLocation(program, "veiwPos");
    lightPosLoc = glGetUniformLocation(program, "light.pos");
    lightColorLoc = glGetUniformLocation(program, "light.color");

    materialAmbientLoc = glGetUniformLocation(program, "material.ambient");
    materialDiffuseLoc = glGetUniformLocation(program, "material.diffuse");
    materialSpecularLoc = glGetUniformLocation(program, "material.specular");
    materialShininessLoc = glGetUniformLocation(program, "material.shininess");
    flagLoc = glGetUniformLocation(program, "flag");

	Window::projection = glm::perspective(glm::radians(Window::fov),
		double(Window::width) / (double)Window::height, 1.0, 1000.0);
	Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
	if (!debugMode) {
		Window::frustumCalc();
	}
	return true;
}

bool Window::initializeObjects()
{
	// Create a cube of size 5.
//	cube = new Cube(5.0f);
	// Create a point cloud consisting of cube vertices.
//	cubePoints = new PointCloud("foo", 100);
/*    bunnyPoints = new PointCloud("bunny.obj", 1);
    
    dragonPoints = new PointCloud("dragon.obj", 1);
    
    bearPoints = new PointCloud("bear.obj", 1);
    
    lightObj = new Light(glm::vec3(-10.0f,-0.1f,0.1f));

	// Set cube to be the first to display
	currentObj = bunnyPoints;
    materialAmbient = bunnyMaterialAmbient;
    materialDiffuse = bunnyMaterialDiffuse;
    materialSpecular = bunnyMaterialSpecular;*/
	srand(time(0));

	std::string path = Path_StripFilename(Path_GetExecutablePath());
	Material mat{ glm::vec3(0.80,0.36,0.0) ,glm::vec3(0.3,0.3,0.3) ,glm::vec3(0.5,0.5,0.5) ,glm::vec3(0.1,0.1,0.1) ,0.25};
	g_base1 = new Geometry(path + "\\objs\\base1.obj", 1, mat);
    g_mid1 = new Geometry(path+"\\objs\\mid1.obj",1, mat);
	g_mid2 = new Geometry(path + "\\objs\\mid2.obj", 1, mat);
	g_mid3 = new Geometry(path + "\\objs\\mid3.obj", 1, mat);
	g_top1 = new Geometry(path + "\\objs\\top1.obj", 1, mat);
	mat = { glm::vec3(0.25,0.5,0.25) ,glm::vec3(0.4,0.4,0.4) ,glm::vec3(0.2,0.2,0.2) ,glm::vec3(0.5,0.5,0.5) ,0.7 };
	g_terrian = new Geometry(path + "\\objs\\terrian.obj", 1, mat);
	mat = { glm::vec3(0.3,0.3,0.3) ,glm::vec3(0.4,0.4,0.4) ,glm::vec3(0.2,0.2,0.2) ,glm::vec3(0.5,0.5,0.5) ,0.7 };
	g_road = new Geometry(path + "\\objs\\road.obj", 1, mat);
	mat = { glm::vec3(0.0,0.4,0.75) ,glm::vec3(0.4,0.4,0.4) ,glm::vec3(0.5,0.5,0.5) ,glm::vec3(0.7,0.7,0.7) ,0.7 };
	g_park = new Geometry(path + "\\objs\\park.obj", 1, mat);
	mat = { glm::vec3(0.75,0.75,0.0) ,glm::vec3(0.25,0.20725,0.20725) ,glm::vec3(1.0,0.829,0.829) ,glm::vec3(0.892157,0.841176,0.707843) ,0.21794872 };
	g_gold = new Geometry(path + "\\objs\\token.obj", 1, mat);
	mat = { glm::vec3(0.1,0.1,0.1) ,glm::vec3(0.4,0.4,0.4) ,glm::vec3(0.5,0.5,0.5) ,glm::vec3(0.7,0.7,0.7) ,0.7 };
	g_bomb = new Geometry(path + "\\objs\\bomb.obj", 1, mat);
	//binder = new Binding("C:\\Users\\ztybigcat\\Desktop\\Final_Proj\\cube.obj", 1);


    glm::mat4 identity = glm::mat4(1.0f);
    root = new Transform(identity);
	allTokens = new Transform(identity);
	terrian = new Transform(glm::translate(glm::vec3(28.0f,0.0f,148.0f)));
	for (int i = 0; i < 18; i++) {
		if (i < 9) {
			roads[i] = new Transform(glm::translate(glm::vec3(-92.0f + i * 30, 0.1f, 148.0f)));
		}
		else {
			roads[i] = new Transform(glm::translate(glm::vec3(28.0f, 0.1f, 28.0f + (i - 9) * 30)) * glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
		}
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			blocks[i+10*j] = new Transform(glm::translate(glm::vec3(-120.0f+30*i, 0.0f, j * 30)));
		}
	}
	int arr[74];
	for (int i = 0; i < 74; i++) {
		arr[i] = i;
	}
	for (int i = 0; i < 18; i++) {
		shuffle(arr, 74);
		if(i<9){
			for (int j = 0; j < 20; j++) {
				tokens[j + 20 * i] = new Transform(glm::translate(glm::vec3(-92 + i*30, 18, 2 + 4 * arr[j]))* glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			}
		}
		else {
			for (int j = 0; j < 20; j++) {
				tokens[j + 20 * i] = new Transform(glm::translate(glm::vec3(-118 + 4 * arr[j], 18, 28 + 30 * (i - 9))));
			}
		}
	}
	park = new Transform(glm::translate(glm::vec3(13.0f, 0.15f, 13.0f)));
	blockStatus[11] = 1;
	blockStatus[12] = 1;
	for (int i = 0; i < 100; i++) {
		if (i != 4) {
			if (std::rand() % 8 > 6) {
				blockStatus[i] = 1;
			}
		}
	}
	for (int i = 0; i < 360; i++) {
		if (std::rand() % 5 > 3) {
			tokenStatus[i] = 1;
		}
	}

	root->addChild(terrian);
	terrian->addChild(g_terrian);
	root->addChild(allTokens);
	park->addChild(g_park);
	for (int i = 0; i < 18; i++) {
		root->addChild(roads[i]);
		roads[i]->addChild(g_road);
	}
	for (int i = 0; i < 100; i++) {
		root->addChild(blocks[i]);
	}
	for (int i = 0; i < 100; i++) {
		if (blockStatus[i] == 1) {
			blocks[i]->addChild(park);
		}
		else {
			std::vector<Transform*> buildings = std::vector<Transform*>(4);
			buildings[0] = new Transform(glm::translate(glm::vec3(7.0f, 0.0f, 7.0f)));
			buildings[1] = new Transform(glm::translate(glm::vec3(19.0f, 0.0f, 7.0f)));
			buildings[2] = new Transform(glm::translate(glm::vec3(7.0f, 0.0f, 19.0f)));
			buildings[3] = new Transform(glm::translate(glm::vec3(19.0f, 0.0f, 19.0f)));
			for (int j = 0; j < 4; j++) {
				blocks[i]->addChild(buildings[j]);
				TopTrans* base1 = new TopTrans(glm::translate(glm::vec3(0.0f, 2.0f, 0.0f)));
				base1->addChild(g_base1);
				buildings[j]->addChild(base1);
				int midnum = int(std::rand() % 6 + 3);
				for (int k = 0; k < midnum; k++) {
					int type = int(rand() % 3);
					TopTrans* mid = new TopTrans(glm::translate(glm::vec3(0.0f, 2.0f + 4.0f * (k + 1), 0.0f)));
					if (type == 0) {
						mid->addChild(g_mid1);
					}
					else if (type == 1) {
						mid->addChild(g_mid2);
					}
					else {
						mid->addChild(g_mid3);
					}
					buildings[j]->addChild(mid);
				}
				TopTrans* top1 = new TopTrans(glm::translate(glm::vec3(0.0f, 2.0f + 4.0f * (midnum + 1), 0.0f)));
				top1->addChild(g_top1);
				buildings[j]->addChild(top1);		
			}
		}
	}


	//skybox = new Skybox();
	//skybox->init();
	//skybox->updateProjection(projection);
	//skybox->updateView(view);

	for (int i = 0; i < 360; i++) {
		if (tokenStatus[i] == 1) {
			allTokens->addChild(tokens[i]);
			tokens[i]->addChild(g_bomb);
		}
		else {
			allTokens->addChild(tokens[i]);
			tokens[i]->addChild(g_gold);
		}
	}

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	delete root;

	// Delete the shader program.
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(fov), 
		double(width) / (double)height, 1.0, 1000.0);
	Window::frustumCalc();
}

void Window::idleCallback()
{   
//    lightObj->update();
}
void Window::displayCallback(GLFWwindow* window)
{
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Window::view));
	Window::cullingCount = 0;
    // Render the object.
	glUseProgram(program);

	//printf("Number %d", Window::cullingCount);

	glUniform3fv(lightColorLoc, 1, value_ptr(glm::vec3(1.,1.,1.)));
	glUniform3fv(lightPosLoc, 1, value_ptr(glm::vec3(0.0, -100.0, 100.0)));
	root->draw(glm::mat4(1.0f), program);
	//skybox->updateProjection(projection);
	//skybox->updateView(view);
	//skybox->draw();

	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	/*
	 * TODO: Modify below to add your key callbacks.
	 */
	
	// Check for a key press.
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
//		case GLFW_KEY_1:
//			// Set currentObj to cube
//			currentObj = cube;
//			break;
//		case GLFW_KEY_2:
//			// Set currentObj to cubePoints
//			currentObj = cubePoints;
//			break;
//        case GLFW_KEY_F1:
//            currentObj = bunnyPoints;
//            materialAmbient = bunnyMaterialAmbient;
//            materialDiffuse = bunnyMaterialDiffuse;
//            materialSpecular = bunnyMaterialSpecular;
//            break;
//        case GLFW_KEY_F2:
//            currentObj = dragonPoints;
//            materialAmbient = dragonMaterialAmbient;
//            materialDiffuse = dragonMaterialDiffuse;
//            materialSpecular = dragonMaterialSpecular;
//            break;
//        case GLFW_KEY_F3:
//            currentObj = bearPoints;
//            materialAmbient = bearMaterialAmbient;
//            materialDiffuse = bearMaterialDiffuse;
//            materialSpecular = bearMaterialSpecular;
//            break;
//        case GLFW_KEY_P:
//            if (mods == GLFW_MOD_SHIFT){
//                currentObj->updatePointSizeL();
//            }else{
//                currentObj->updatePointSizeS();
//            }
//            break;
        case GLFW_KEY_N:
		{glUniform1f(flagLoc, !normalFlag);
		Window::normalFlag = !Window::normalFlag;
		break; }
		case GLFW_KEY_S:
			//binder->sFlag = !binder->sFlag;
			break;
		case GLFW_KEY_C:
			Window::cFlag = !Window::cFlag;
			break;
		case GLFW_KEY_D: {
			Window::debugMode = !Window::debugMode;
			if (debugMode) {
				Window::fov = Window::fov + 5.0f;
			}
			else {
				Window::fov = Window::fov - 2.0f;
			}
			Window::projection = glm::perspective(glm::radians(Window::fov),
				double(Window::width) / double(Window::height), 1.0, 1000.0);
			break;
		}
//        case GLFW_KEY_1:
//            lightSwitch = 0;
//            break;
//        case GLFW_KEY_2:
//            lightSwitch = 1;
//            break;
//        case GLFW_KEY_3:
//            lightSwitch = 2;
//            break;
        case GLFW_KEY_UP:
		{Window::direction = glm::rotate(glm::radians(1.0f), glm::vec3(1.0f, 0.0f, 1.0f)) * glm::vec4(Window::direction, 0.0f);
		Window::center = Window::direction + Window::eye;
		Window::up = glm::normalize(glm::cross(-Window::direction, Window::right));//glm::rotate(glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Window::up, 0);
		Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
		if (!Window::debugMode) {
			Window::frustumCalc();
		}
		break; }
        case GLFW_KEY_DOWN:
		{Window::direction = glm::rotate(glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 1.0f)) * glm::vec4(Window::direction, 0.0f);
		Window::center = Window::direction + Window::eye;
		Window::up = glm::normalize(glm::cross(-Window::direction, Window::right));//glm::rotate(glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Window::up, 0);
		Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
		if (!Window::debugMode) {
			Window::frustumCalc();
		}
		break; }
        case GLFW_KEY_LEFT:
		{Window::direction = glm::rotate(glm::radians(1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Window::direction, 0.0f);
		Window::center = Window::direction + Window::eye;
		Window::right = glm::normalize(glm::cross(Window::up, -Window::direction));//glm::rotate(glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Window::up, 0);
		Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
		if (!Window::debugMode) {
			Window::frustumCalc();
		}
		break; }
        case GLFW_KEY_RIGHT:
		{Window::direction = glm::rotate(glm::radians(-1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(Window::direction, 0.0f);
		Window::center = Window::direction + Window::eye;
		Window::right = glm::normalize(glm::cross(Window::up, -Window::direction));//glm::rotate(glm::radians(-1.0f), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::vec4(Window::up, 0);
		Window::view = glm::lookAt(Window::eye, Window::center, Window::up);
		if (!Window::debugMode) {
			Window::frustumCalc();
		}
		break; }
		default:
			break;
		}
	}
}
void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//double dif = 0;
	if (yoffset > 0) {
		//dif = 0.5;
		Window::fov += 0.5;
	}
	if (yoffset < 0) {
		//dif = -0.5;
		Window::fov -= 0.5;
	}
	//Window::fov = std::min(Window::fov + dif, double(120.0));
	//Window::fov = std::max(Window::fov + dif, double(20.0));
	Window::projection = glm::perspective(glm::radians(Window::fov),
		double(Window::width) / double(Window::height), 1.0, 1000.0);
	//skybox->updateProjection(projection);
	if (!debugMode) {
		Window::frustumCalc();
	}
	//glm::translate(view,glm::vec3(0.0f,0.0f,yoffset*0.1f));
}
void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
//    if(button == GLFW_MOUSE_BUTTON_LEFT)
//    {
//        if(action == GLFW_PRESS){
//            buttonDown = true;
//            double xpos, ypos;
//            glfwGetCursorPos(window, &xpos, &ypos);
//            lastPoint.x = (2*xpos-width)/width;
//            lastPoint.y = (height-2*ypos)/height;
//            lastPoint.z = 0;
//            float d;
//            d = glm::length(lastPoint);
//            d = (d<1)?d:1;
//            lastPoint.z = float(sqrtf(1.001-d*d));
//            lastPoint = glm::normalize(lastPoint);
//        }else if(action == GLFW_RELEASE){
//            buttonDown = false;
//        }
//    }
}
void Window::mouseButtonCheck(GLFWwindow* window, double xpos, double ypos)
{
//    if(buttonDown)
//    {
//        glm::vec3 v;
//        float d;
//        v.x = float((2*xpos-width)/width);
//        v.y = float((height-2*ypos)/height);
//        v.z = 0.0f;
//        d = glm::length(v);
//        d = (d<1)?d:1;
//        v.z = float(sqrtf(1.001-d*d));
//        v = glm::normalize(v);
//        if((glm::length(lastPoint - v) > 0.0001))
//        {
//            glm::vec3 rAxis = glm::cross(lastPoint,v);
//            float angle = glm::dot(lastPoint,v)*0.2;
//            view=glm::rotate(glm::radians(angle),rAxis)*view;
//        }
//        lastPoint = v;
//    }
}
void Window::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos){
//        mouseButtonCheck(window, xpos, ypos);
}
void Window::shuffle(int* arr, size_t n) {
	if (n > 1) {
		size_t i;
		srand(time(0));
		for (i = 0; i < n - 1; i++) {
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = arr[j];
			arr[j] = arr[i];
			arr[i] = t;
		}
	}
}
void Window::frustumCalc() {
	Window::fc = Window::eye+glm::normalize(Window::direction)*float(1000);
	float hnh = float(glm::tan(glm::radians(Window::fov) / 2)) * 1.0f;
	float hnw = float(Window::width) / float(Window::height) * hnh;
	glm::vec3 nU = glm::normalize(Window::up);
	glm::vec3 nR = glm::normalize(Window::right);
	Window::ntl = nc + (nU * hnh) - (nR * hnw);
	Window::ntr = nc + (nU * hnh) + (nR * hnw);
	Window::nbl = nc - (nU * hnh) - (nR * hnw);
	Window::nbr = nc - (nU * hnh) + (nR * hnw);
	Window::nc = Window::eye + glm::normalize(Window::direction) * float(1);
	//float hnh = hnh*1000.0f;
	//float hnw = hnw*1000.0f;
	//Window::ntl = nc + (Window::up * hnh) - (Window::right * hnw);
	//Window::ntr = nc + (Window::up * hnh) + (Window::right * hnw);
	//Window::nbl = nc - (Window::up * hnh) - (Window::right * hnw);
	//Window::nbr = nc - (Window::up * hnh) + (Window::right * hnw);
	Window::normF = glm::normalize(Window::direction);
	Window::normN = glm::normalize(-Window::direction);
	Window::normR = glm::cross(glm::normalize((nc + nR * hnw) - Window::eye),nU);
	Window::normL = glm::cross(nU,glm::normalize((nc - nR * hnw) - Window::eye));
	Window::normU = glm::cross(nR,glm::normalize((nc + nU * hnh) - Window::eye));
	Window::normB = glm::cross(glm::normalize((nc - nU * hnh) - Window::eye),nR);
	//Window::unCalc = true;
}
