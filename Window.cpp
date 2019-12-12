
#include "Window.h"
#include "pathtools.h"
#include "Cloud.cpp"

int Window::width;
int Window::height;
bool Window::normalFlag = false;
bool Window::autoOn = false;
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
bool Window::keyF[4] = {};

bool Window::firstMouse = false;
float Window::yaw = 0.0f;
float Window::pitch = 0.0f;
float Window::lastX = float(Window::width) /2.0;
float Window::lastY = float(Window::height) / 2.0;

const char* Window::windowTitle = "Silkman";
int Window::score = 0;
bool Window::over = false;

Skybox* Window::skybox;
ISoundEngine* Window::SoundEngine;
// Objects to display.
//
//Light * Window::lightObj;
int blockStatus[100] = {}; //0 for building, 1 for park
int tokenStatus[360] = {}; //0 for gold, 1 for bomb

std::vector<Transform*> Window::blocks = std::vector<Transform*>(100);
std::vector<Transform*> Window::roads = std::vector<Transform*>(18); //0-8 vertical left to right, 9-17 herizontal bottom to top
std::vector<Token*> Window::tokens = std::vector<Token*>(360);//20 per road
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

//glm::vec3 Window::eye(-0.5f, 18.0f, -10.0f); // Camera position.
//glm::vec3 Window::eye(28.0f, 500.0f, 148.0f); // Camera position.
//glm::vec3 Window::center(0.0f, 18.0f, 148.0f); // The point we are looking at.
//glm::vec3 Window::center(28.0f, 2.0f, 148.0f); // The point we are looking at.
//glm::vec3 Window::up(0.0f, 1.0f, 0.0f); // The up direction of the camera.
//glm::vec3 Window::up(1.0f, 0.0f, 0.0f); // The up direction of the camera.

glm::vec3 Window::eye(-0.5f, 18.0f, 0.0f);
glm::vec3 Window::center(0.0f, 0.0f, 1.0f);
glm::vec3 Window::up(0.0f, 1.0f, 0.0f);

//glm::vec3 Window::direction = glm::normalize(Window::center - Window::eye);
//glm::vec3 Window::right = glm::cross(Window::up, - Window::direction);

//glm::vec3 materialAmbient;
//glm::vec3 materialDiffuse;
//glm::vec3 materialSpecular;

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::eye + Window::center, Window::up);


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
	Window::view = glm::lookAt(Window::eye, Window::eye+Window::center, Window::up);
	return true;
}

bool Window::initializeObjects()
{
	/*
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLuint FramebufferName = 0;
	glGenFramebuffers(1, &FramebufferName);
	glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	GLuint depthTexture;
	glGenTextures(1, &depthTexture);
	glBindTexture(GL_TEXTURE_2D, depthTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);

	glDrawBuffer(GL_NONE); // No color buffer is drawn to.

	glm::vec3 lightInvDir = glm::vec3(0.0, -100.0, 100.0);
	glm::mat4 depthProjectionMatrix = glm::ortho<float>(-10, 10, -10, 10, -10, 20);
	glm::mat4 depthViewMatrix = glm::lookAt(lightInvDir, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	glm::mat4 depthModelMatrix = glm::mat4(1.0);
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix * depthModelMatrix;
	*/
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
				tokens[j + 20 * i] = new Token(glm::translate(glm::vec3(-92 + i*30, 22 + rand() % 30 - 15, 2 + 4 * arr[j]))* glm::rotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
			}
		}
		else {
			for (int j = 0; j < 20; j++) {
				tokens[j + 20 * i] = new Token(glm::translate(glm::vec3(-118 + 4 * arr[j], 22 +rand() % 30-15, 28 + 30 * (i - 9))));
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
			std::vector<TopTrans*> buildings = std::vector<TopTrans*>(4);
			buildings[0] = new TopTrans(glm::translate(glm::vec3(7.0f, 0.0f, 7.0f)));
			buildings[1] = new TopTrans(glm::translate(glm::vec3(19.0f, 0.0f, 7.0f)));
			buildings[2] = new TopTrans(glm::translate(glm::vec3(7.0f, 0.0f, 19.0f)));
			buildings[3] = new TopTrans(glm::translate(glm::vec3(19.0f, 0.0f, 19.0f)));
			for (int j = 0; j < 4; j++) {
				blocks[i]->addChild(buildings[j]);
				Cell* base1 = new Cell(glm::translate(glm::vec3(0.0f, 2.0f, 0.0f)));
				base1->addChild(g_base1);
				buildings[j]->addChild(base1);
				int midnum = int(std::rand() % 6 + 3);
				for (int k = 0; k < midnum; k++) {
					int type = int(rand() % 3);
					Cell* mid = new Cell(glm::translate(glm::vec3(0.0f, 2.01f + 4.01f * (k + 1), 0.0f)));
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
				Cell* top1 = new Cell(glm::translate(glm::vec3(0.0f, 2.01f + 4.01f * (midnum + 1), 0.0f)));
				top1->addChild(g_top1);
				buildings[j]->addChild(top1);		
			}
		}
		glfwSetTime(0.0);
	}


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
	skybox = new Skybox();
	skybox->init();
	skybox->updateProjection(projection);
	skybox->updateView(view);
	SoundEngine = createIrrKlangDevice();
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
}

void Window::idleCallback()
{   
	if (over) {
		return;
	}
	if (autoOn && (Window::eye.y <= 0 || Window::eye.z <= -0.1 || Window::eye.z >= 296.1 || Window::eye.x <= -120.1 || Window::eye.x >= 176.1)) {
		Window::gameOver();
	}
	Window::moving();
	Window::blocks[int(Window::eye.z/30) * 10 + int((Window::eye.x+120)/30)]->detectCollision(Window::eye, glm::mat4(1.0f));
	Window::allTokens->detectCollision(Window::eye, glm::mat4(1.0f));
}
bool already = false;

void Window::displayCallback(GLFWwindow* window)
{
	if (over && !already) {
		glfwSetWindowTitle(window, ("Silkman-- GameOver! Your Score is " + to_string(score)).c_str());
		SoundEngine->stopAllSounds();
		string path = Path_StripFilename(Path_GetExecutablePath()) + "\\death.wav";
		SoundEngine->play2D(path.c_str());
		glfwPollEvents();
		already = true;
		return;
	}
	else if(over && already){
		glfwPollEvents();
		return;
	}
    // Clear the color and depth buffers.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);

    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Window::view));
	//glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getTexture());
    // Render the object.
	

	//printf("Number %d", Window::cullingCount);

	glUniform3fv(lightColorLoc, 1, value_ptr(glm::vec3(1.,1.,1.)));
	glUniform3fv(lightPosLoc, 1, value_ptr(glm::vec3(0.0, -100.0, 100.0)));
	root->draw(glm::mat4(1.0f), program);

	skybox->updateProjection(projection);
	skybox->updateView(view);
	skybox->draw();   
	glfwSetWindowTitle(window, ("Silkman-- Score: "+ to_string(score)).c_str());

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
        case GLFW_KEY_N:
		{glUniform1f(flagLoc, !normalFlag);
		Window::normalFlag = !Window::normalFlag;
		break; }
		case GLFW_KEY_B:
			if (!autoOn) {
				string path = Path_StripFilename(Path_GetExecutablePath()) + "\\bk.wav";
				SoundEngine->play2D(path.c_str(), GL_TRUE);
			}
			else {
				SoundEngine->stopAllSounds();
			}
			autoOn = !autoOn;
			glfwSetTime(0.0);
			break;
		case GLFW_KEY_W:
			keyF[0] = true;
			break;
		case GLFW_KEY_S:
			keyF[1] = true;
			break;
		case GLFW_KEY_A:
			keyF[2] = true;
			break;
		case GLFW_KEY_D: 
			keyF[3] = true;
			break;
		default:
			break;
		}
	}
	else if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_W:
			keyF[0] = false;
			break;
		case GLFW_KEY_S:
			keyF[1] = false;
			break;
		case GLFW_KEY_A:
			keyF[2] = false;
			break;
		case GLFW_KEY_D:
			keyF[3] = false;
			break;
		default:
			break;
		}
	}
}
void Window::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
	//double dif = 0;
	if (Window::fov >= 1.0f && Window::fov <= 90.0f) {
		if (yoffset > 0) {
			//dif = 0.5;
			Window::fov += 0.5;
		}
		if (yoffset < 0) {
			//dif = -0.5;
			Window::fov -= 0.5;
		}
	}
	//Window::fov = std::min(Window::fov + dif, double(120.0));
	//Window::fov = std::max(Window::fov + dif, double(20.0));
	Window::projection = glm::perspective(glm::radians(Window::fov),
		double(Window::width) / double(Window::height), 1.0, 1000.0);
	skybox->updateProjection(projection);
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
void Window::mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	



	float xoffset = xpos - float(Window::width) / 2.0;
	float yoffset = float(Window::height) / 2.0 - ypos;
	glfwSetCursorPos(window, float(Window::width) / 2.0, float(Window::height) / 2.0);
	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	Window::center = glm::normalize(front);
	Window::view = glm::lookAt(Window::eye, Window::eye + Window::center, Window::up);
	skybox->updateView(view);
}
void Window::mouseButtonCheck(GLFWwindow* window, double xpos, double ypos)
{

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
void Window::cursorEnterCallback(GLFWwindow* window, int entered) {

}
void Window::moving() {
	if (autoOn) {
		Window::eye += float(glfwGetTime()) * 0.005f * Window::center;
	}
	else if (Window::keyF[0]) {
		Window::eye += 0.05f * Window::center;
	}
	if (Window::keyF[1]) {
		Window::eye -= 0.05f * Window::center;
	}
	if (Window::keyF[2]) {
		Window::eye -= glm::normalize(glm::cross(Window::center, Window::up)) * 0.05f;
	}
	if (Window::keyF[3]) {
		Window::eye += glm::normalize(glm::cross(Window::center, Window::up)) * 0.05f;
	}
	Window::view = glm::lookAt(Window::eye, Window::eye + Window::center, Window::up);

	skybox->updateView(view);
}

void Window::decScore() {
	score = score - 3;
	if (score < 0) {
		score = 0;
	}
	string path = Path_StripFilename(Path_GetExecutablePath()) + "\\exp.wav";
	SoundEngine->play2D(path.c_str());
}
void Window::incScore() {
	score++;
	string path = Path_StripFilename(Path_GetExecutablePath()) + "\\gold.wav";
	SoundEngine->play2D(path.c_str());
	
}

void Window::gameOver() {
	over = true;
}

