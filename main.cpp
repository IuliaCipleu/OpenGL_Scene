#if defined (__APPLE__)
#define GLFW_INCLUDE_GLCOREARB
#define GL_SILENCE_DEPRECATION
#else
#define GLEW_STATIC
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>

#include <glm/glm.hpp> //core glm functionality
#include <glm/gtc/matrix_transform.hpp> //glm extension for generating common transformation matrices
#include <glm/gtc/matrix_inverse.hpp> //glm extension for computing inverse matrices
#include <glm/gtc/type_ptr.hpp> //glm extension for accessing the internal data structure of glm types
#include <glm/gtx/string_cast.hpp>


#include "Window.h"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Model3D.hpp"
#include "SplineUtils.hpp"

// window
gps::Window myWindow;

// matrices
glm::mat4 model;
glm::mat4 modelLed;
glm::mat4 view;
glm::mat4 viewLed;
glm::mat4 projection;
glm::mat4 projectionLed;
glm::mat3 normalMatrix;
//glm::mat4 lightRotation;

// light parameters
glm::vec3 lightDir;
glm::vec3 lightColor;
glm::vec3 outsideLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

// shader uniform locations
GLint modelLoc;
GLint modelLocRope;
GLint viewLoc;
GLint projectionLoc;
GLint normalMatrixLoc;
GLint normalMatrixLocRope;
GLint lightDirLoc;
GLint lightColorLoc;

// camera
gps::Camera myCamera(
	glm::vec3(0.0f, 0.0f, 3.0f),
	glm::vec3(0.0f, 0.0f, -10.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));

glm::vec3 pointLightPositions[] = {
	glm::vec3(-148.63f, 13.3121f, 202.507f),
	glm::vec3(-146.471f, 13.4099f, 202.148f),
	glm::vec3(-146.349f,  16.3358f, 202.298f),
	glm::vec3(-146.347f,  10.692f, 202.451f)
	//glm::vec3(-147.4f,  14.89f, 200.1f),
	//glm::vec3(-145.6f,  10.92f, 203.7f)
};
glm::vec3 pctLightPos = glm::vec3(-146.63f, 13.312f, 202.51f);

GLfloat cameraSpeed = 0.8f;

GLboolean pressedKeys[1024];

// models
gps::Model3D teapot;
gps::Model3D land;
gps::Model3D sky;
gps::Model3D nightSky;
gps::Model3D dock;
gps::Model3D lotus;
gps::Model3D boat;
gps::Model3D dadDuck;
gps::Model3D momDuck;
gps::Model3D babyDuck;
gps::Model3D trees;
gps::Model3D windmill;
gps::Model3D cat;
gps::Model3D house;
gps::Model3D dogHouse;
gps::Model3D dog;
gps::Model3D pinwheel;
gps::Model3D tulip;
gps::Model3D rope;
gps::Model3D sun;
gps::Model3D lampBox;
gps::Model3D lamp;
gps::Model3D stageLampBox;
gps::Model3D led;
gps::Model3D fire;
gps::Model3D candle;
gps::Model3D candleSupport;

GLfloat angle;
GLfloat lightAngle;

// shaders
gps::Shader myBasicShader;
gps::Shader myLightShader;
gps::Shader myCubeShader;
gps::Shader myBlinnPhongShader;
gps::Shader myLampLightShader;
gps::Shader myLedLightShader;
gps::Shader myPunctiformShader;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

//glm::vec3 pointLightPositions[] = {
//	glm::vec3(0.7f,  0.2f,  2.0f),
//	glm::vec3(2.3f, -3.3f, -4.0f),
//	glm::vec3(-4.0f,  2.0f, -12.0f),
//	glm::vec3(0.0f,  0.0f, -3.0f)
//};

std::vector<glm::vec3> controlPoints = {
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(33.0f, 1.0f, 6.5f),
	glm::vec3(35.5f, 1.2f, 53.5f),
	glm::vec3(74.13f, 1.35f, 136.9f),
	glm::vec3(11.1f, 1.5f, 80.7f),
	glm::vec3(-111.5f, 1.5f, 77.65f),
	glm::vec3(-180.0f, 1.7f, 62.38f),
	glm::vec3(-209.8f, 1.7f, 18.66f),
	glm::vec3(-138.6f, 1.6f, -134.6f),
	glm::vec3(-128.3f, 1.6f, -174.9f),
	glm::vec3(-74.3f, 1.6f, -279.8f),
	glm::vec3(-0.3f, 1.6f, -195.7f),
	glm::vec3(82.4f, 1.5f, -227.9f),
	glm::vec3(74.4f, 1.0f, -53.35f),
	glm::vec3(-41.35f, 1.0f, -99.2f),
	glm::vec3(0.0f, 0.0f, 0.0f)
};

//std::vector<gps::Model3D> route = { house, dog, windmill, lotus, dock, dadDuck, momDuck, babyDuck, tulip, trees };

GLint fogColorLoc;
GLint fogDensityLoc;
//GLuint shadowMapFBO;
//GLuint depthMapTexture;
int startFog;
float fogDensity;
glm::vec3 lampColor = glm::vec3(1.0f, 1.0f, 1.0f);

GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	GLFWwindow* currentWindow = static_cast<GLFWwindow*>(glfwGetWindowUserPointer(window));

	// Update the window dimensions
	myWindow.setWindowDimensions({ width, height });
	fprintf(stdout, "Window resized! New width: %d , and height: %d\n", width, height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			pressedKeys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			pressedKeys[key] = false;
		}
	}
}

bool firstMouse = true;
float lastX = myWindow.getWindowDimensions().width / 2, lastY = myWindow.getWindowDimensions().height / 2;
float yaw = -90.0f;
float pitch = 0.0f;

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	//TODO
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	xoffset *= cameraSpeed;
	yoffset *= cameraSpeed;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	myCamera.rotate(pitch, yaw);

	view = myCamera.getViewMatrix();
	myBasicShader.useShaderProgram();
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
}

static bool isAnimating = false;
float animationDuration = 10.0f;  // Adjust the duration of the animation as needed
float animationStartTime;
bool nightMode = false;
static bool wireframeMode = false;
static bool wireframeMode2 = false;
glm::vec3 dogPosition(0.0f, 0.0f, 0.0f);
float moveDog = 0.1f;

void processMovement() {
	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		//update view matrix
		view = myCamera.getViewMatrix();
		myBasicShader.useShaderProgram();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// compute normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		//update view matrix
		view = myCamera.getViewMatrix();
		myBasicShader.useShaderProgram();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// compute normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		//update view matrix
		view = myCamera.getViewMatrix();
		myBasicShader.useShaderProgram();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// compute normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
		//update view matrix
		view = myCamera.getViewMatrix();
		myBasicShader.useShaderProgram();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		// compute normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		angle -= 1.0f;
		// update model matrix for teapot
		model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
		// update normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angle += 1.0f;
		// update model matrix for teapot
		model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0, 1, 0));
		// update normal matrix for teapot
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	}

	if (pressedKeys[GLFW_KEY_M] && !isAnimating) {
		// Start the animation when 'M' key is pressed
		isAnimating = true;
		animationStartTime = glfwGetTime();
	}

	if (isAnimating) {
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - animationStartTime;

		if (deltaTime < animationDuration) {
			float t = deltaTime / animationDuration;

			glm::vec3 interpolatedPosition = gps::SplineUtils::interpolateSpline(controlPoints, t);

			myCamera.setPosition(interpolatedPosition);

			myCamera.rotate(0.0f, 90.0f);

			view = myCamera.getViewMatrix();
			myBasicShader.useShaderProgram();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		}
		else {
			isAnimating = false;
		}
	}

	if (pressedKeys[GLFW_KEY_P]) {		
		wireframeMode = true;

		if (wireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (pressedKeys[GLFW_KEY_O]) {
		wireframeMode = false;

		if (wireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (pressedKeys[GLFW_KEY_I]) {
		wireframeMode2 = true;

		if (wireframeMode2) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (pressedKeys[GLFW_KEY_U]) {
		wireframeMode2 = false;

		if (wireframeMode2) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	if (pressedKeys[GLFW_KEY_F]) {
		myBasicShader.useShaderProgram();
		startFog = 1;
		std::cout << "Fog started" << std::endl;
	}

	if (pressedKeys[GLFW_KEY_G]) {
		myBasicShader.useShaderProgram();
		startFog = 0;
		std::cout << "Fog stoped" << std::endl;
	}

	if (pressedKeys[GLFW_KEY_V]) {
		fogDensity = fogDensity - 0.0001f > 0.0f ? fogDensity - 0.0001f : 0.0f;
		std::cout << "Fog decreased at " << fogDensity << std::endl;
	}

	if (pressedKeys[GLFW_KEY_T]) {
		fogDensity = fogDensity + 0.0001f < 1.0f ? fogDensity + 0.0001f : 0.0f;
		std::cout << "Fog increased at " << fogDensity << std::endl;
	}

	if (pressedKeys[GLFW_KEY_J]) {
		pctLightPos.y -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_L]) {
		pctLightPos.y += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_N]) {
		nightMode = true;
		outsideLightColor = glm::vec3(0.3f, 0.3f, 0.3f); //grey light
	}

	if (pressedKeys[GLFW_KEY_B]) {
		nightMode = false;
		outsideLightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	}

	if (pressedKeys[GLFW_KEY_1]) {
		lampColor = glm::vec3(1.0f, 0.0f, 0.0f);
	}

	if (pressedKeys[GLFW_KEY_2]) {
		lampColor = glm::vec3(0.0f, 1.0f, 0.0f);
	}

	if (pressedKeys[GLFW_KEY_3]) {
		lampColor = glm::vec3(0.0f, 0.0f, 1.0f);
	}

	if (pressedKeys[GLFW_KEY_4]) {
		lampColor = glm::vec3(1.0f, 1.0f, 0.0f);
	}

	if (pressedKeys[GLFW_KEY_5]) {
		lampColor = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	if (pressedKeys[GLFW_KEY_UP]) {
		dogPosition.z += moveDog;
	}

	if (pressedKeys[GLFW_KEY_DOWN]) {
		dogPosition.z -= moveDog;
	}

	if (pressedKeys[GLFW_KEY_RIGHT]) {
		dogPosition.x += moveDog;
	}

	if (pressedKeys[GLFW_KEY_LEFT]) {
		dogPosition.x -= moveDog;
	}
}

void initOpenGLWindow() {
	myWindow.Create(1024, 768, "OpenGL Project Core");
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
	glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
	glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
	glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void initOpenGLState() {
	glClearColor(1.0f, 0.95f, 0.65f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	glEnable(GL_CULL_FACE); // cull face
	glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise
}

void initModels() {
	land.LoadModel("models/land/landLake.obj");
	sky.LoadModel("models/sky/project.obj");
	nightSky.LoadModel("models/nightSky/nightSky.obj");
	dock.LoadModel("models/dock/myDock.obj");
	lotus.LoadModel("models/lotus/myLotus.obj");
	dadDuck.LoadModel("models/dad_duck/dadDuck.obj");
	momDuck.LoadModel("models/mom_duck/momDuck.obj");
	babyDuck.LoadModel("models/baby_duck/babyDuck.obj");
	boat.LoadModel("models/boat/boat.obj");
	trees.LoadModel("models/trees/myTrees.obj");
	windmill.LoadModel("models/windmill/windmill.obj");
	cat.LoadModel("models/cat/cat.obj");
	house.LoadModel("models/house/house.obj");
	dogHouse.LoadModel("models/doghouse/dogHouse.obj");
	dog.LoadModel("models/dog/dog.obj");
	pinwheel.LoadModel("models/pinwheel/pinwheel.obj");
	tulip.LoadModel("models/tulip/tulip.obj");
	rope.LoadModel("models/rope/rope.obj");
	sun.LoadModel("models/sun/sun.obj");
	lampBox.LoadModel("models/lamp/lampBox.obj");
	lamp.LoadModel("models/lamp/lamp.obj");
	stageLampBox.LoadModel("models/stageLight/stageLightBox.obj");
	led.LoadModel("models/stageLight/led.obj");
	candle.LoadModel("models/candle/candle.obj");
	candleSupport.LoadModel("models/candle/candleSupport.obj");
	fire.LoadModel("models/candle/fire.obj");
	glCheckError();
}

void initShaders() {
	myBasicShader.loadShader(
		"shaders/basic.vert",
		"shaders/basic.frag");
	/* myLightShader.loadShader(
		 "shaders/multiple.vert",
		 "shaders/multiple.frag");*/
	myCubeShader.loadShader(
		"shaders/cube.vert",
		"shaders/cube.frag");
	myBlinnPhongShader.loadShader(
		"shaders/BlinnPhong.vert",
		"shaders/BlinnPhong.frag");
	myLampLightShader.loadShader(
		"shaders/lampLight.vert",
		"shaders/lampLight.frag");
	myLedLightShader.loadShader(
		"shaders/ledLight.vert",
		"shaders/ledLight.frag");
	myPunctiformShader.loadShader(
		"shaders/punctiform.vert",
		"shaders/punctiform.frag");
}

void initUniforms() {
	myBasicShader.useShaderProgram();

	// create model matrix for teapot
	model = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myBasicShader.shaderProgram, "model");

	// get view matrix for current camera
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myBasicShader.shaderProgram, "view");
	// send view matrix to shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// compute normal matrix for teapot
	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myBasicShader.shaderProgram, "normalMatrix");

	// create projection matrix
	projection = glm::perspective(glm::radians(45.0f),
		(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
		0.1f, 500.0f);
	projectionLoc = glGetUniformLocation(myBasicShader.shaderProgram, "projection");
	// send projection matrix to shader
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightDir");
	// send light dir to shader
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	//set light color
	lightColor = outsideLightColor;
	lightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
	// send light color to shader
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

void initLampUniforms() {
	myLampLightShader.useShaderProgram();

	// create model matrix for the lamp
	glm::mat4 modelLamp = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLamp));

	// get view matrix for the current camera (if needed)
	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	// compute normal matrix for the lamp
	normalMatrix = glm::mat3(glm::inverseTranspose(modelLamp));  // Use modelLamp instead of (view * model)
	normalMatrixLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "normalMatrix");
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// create projection matrix (if needed)
	projection = glm::perspective(glm::radians(45.0f),
		(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
		0.1f, 500.0f);
	projectionLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightDirLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(lightDir));

	// set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f);  // white light
	lightColorLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));
}

GLuint ReadTextureFromFile(const char* file_name) {

	int x, y, n;
	int force_channels = 4;
	unsigned char* image_data = stbi_load(file_name, &x, &y, &n, force_channels);

	if (!image_data) {
		fprintf(stderr, "ERROR: could not load %s\n", file_name);
		return false;
	}
	// NPOT check
	if ((x & (x - 1)) != 0 || (y & (y - 1)) != 0) {
		fprintf(
			stderr, "WARNING: texture %s is not power-of-2 dimensions\n", file_name
		);
	}

	int width_in_bytes = x * 4;
	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;
	int half_height = y / 2;

	for (int row = 0; row < half_height; row++) {

		top = image_data + row * width_in_bytes;
		bottom = image_data + (y - row - 1) * width_in_bytes;

		for (int col = 0; col < width_in_bytes; col++) {

			temp = *top;
			*top = *bottom;
			*bottom = temp;
			top++;
			bottom++;
		}
	}

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_SRGB, //GL_SRGB,//GL_RGBA,
		x,
		y,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,
		image_data
	);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void initLedUniforms() {

	//GLuint diffuseMap = ReadTextureFromFile("models/stageLight/glass_01_displace.jpg");
	//GLuint specularMap = ReadTextureFromFile("models/stageLight/glass_01_displace.jpg");
	//if (diffuseMap == 0) {
	//	std::cerr << "Error: Failed to load the diffuse texture." << std::endl;
	//	// Add additional error-handling logic if needed
	//}
	//if (specularMap == 0) {
	//	std::cerr << "Error: Failed to load the specular texture." << std::endl;
	//	// Add additional error-handling logic if needed
	//}

	myLedLightShader.useShaderProgram();

	GLint ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "material.diffuse");
	glUniform1i(ledLoc, 0);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for material.diffuse." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "material.specular");
	glUniform1i(ledLoc, 1);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for material.specular." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "viewPos");
	glUniform3fv(ledLoc, 1, glm::value_ptr(myCamera.getPosition()));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for viewPos." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "material.shininess");
	glUniform1f(ledLoc, 32.0f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for material.shininess." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "dirLight.direction");
	glUniform3fv(ledLoc, 1,glm::value_ptr(glm::vec3(-0.2f, -1.0f, -0.3f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for dirLight.direction." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "dirLight.ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.05f, 0.05f, 0.05f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for dirLight.ambient." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "dirLight.diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.4f, 0.4f, 0.4f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for dirLight.diffuse." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "dirLight.specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 0.5f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for dirLight.specular." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].position");
	glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[0]));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[0].position." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].position");
	glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[1]));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[1].position." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].position");
	glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[2]));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[2].position." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].position");
	glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[3]));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[3].position." << std::endl;
		// Handle the error appropriately
	}
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].position");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[4]));
	//if (ledLoc == -1) {
	//	std::cerr << "Error: Invalid uniform location for pointLights[4].position." << std::endl;
	//	// Handle the error appropriately
	//}
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].position");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(pointLightPositions[5]));
	//if (ledLoc == -1) {
	//	std::cerr << "Error: Invalid uniform location for pointLights[5].position." << std::endl;
	//	// Handle the error appropriately
	//}
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].ambient");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].ambient");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.0f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[5].ambient." << std::endl;
		// Handle the error appropriately
	}
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].diffuse");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].diffuse");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.8f, 0.8f, 0.8f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[5].diffuse." << std::endl;
		// Handle the error appropriately
	}
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[0].specular." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].specular");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].specular");
	//glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].constant");
	glUniform1f(ledLoc, 1.0f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[0].constant." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].constant");
	glUniform1f(ledLoc, 1.0f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].constant");
	glUniform1f(ledLoc, 1.0f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].constant");
	glUniform1f(ledLoc, 1.0f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].constant");
	//glUniform1f(ledLoc, 1.0f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].constant");
	//glUniform1f(ledLoc, 1.0f);
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].linear");
	glUniform1f(ledLoc, 0.09f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[0].linear." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].linear");
	glUniform1f(ledLoc, 0.09f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].linear");
	glUniform1f(ledLoc, 0.09f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].linear");
	glUniform1f(ledLoc, 0.09f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].linear");
	//glUniform1f(ledLoc, 0.09f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].linear");
	//glUniform1f(ledLoc, 0.09f);
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[0].quadratic");
	glUniform1f(ledLoc, 0.032f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for pointLights[0].quadratic." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[1].quadratic");
	glUniform1f(ledLoc, 0.032f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[2].quadratic");
	glUniform1f(ledLoc, 0.032f);
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[3].quadratic");
	glUniform1f(ledLoc, 0.032f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[4].quadratic");
	//glUniform1f(ledLoc, 0.032f);
	//ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLights[5].quadratic");
	//glUniform1f(ledLoc, 0.032f);
	glCheckError();

	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.position");
	glUniform3fv(ledLoc, 1, glm::value_ptr(myCamera.getPosition()));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.position." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.direction");
	glUniform3fv(ledLoc, 1, glm::value_ptr(myCamera.getFrontDirection()));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.direction." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.ambient");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.ambient." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.diffuse");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.diffuse." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.specular");
	glUniform3fv(ledLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.specular." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.constant");
	glUniform1f(ledLoc, 1.0f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.constant." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.linear");
	glUniform1f(ledLoc, 0.09f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.linear." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.quadratic");
	glUniform1f(ledLoc, 0.032f);
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.quadratic." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.cutOff");
	glUniform1f(ledLoc, glm::cos(glm::radians(lightAngle)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.cutOff." << std::endl;
		// Handle the error appropriately
	}
	ledLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "spotLight.outerCutOff");
	glUniform1f(ledLoc, glm::cos(glm::radians(lightAngle + 2.5f)));
	if (ledLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.outerCutOff." << std::endl;
		// Handle the error appropriately
	}
	glCheckError();

	modelLed = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLed));
	if (modelLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.position." << std::endl;
		// Handle the error appropriately
	}

	// get view matrix for the current camera (if needed)
	viewLed = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewLed));
	if (viewLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.position." << std::endl;
		// Handle the error appropriately
	}

	//// compute normal matrix for the lamp
	//normalMatrix = glm::mat3(glm::inverseTranspose(modelLed));  // Use modelLamp instead of (view * model)
	//normalMatrixLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "normalMatrix");
	//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	
	projectionLed = glm::perspective(glm::radians(45.0f),
		(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
		0.1f, 500.0f);
	projectionLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projectionLed));
	if (projectionLoc == -1) {
		std::cerr << "Error: Invalid uniform location for spotLight.position." << std::endl;
		// Handle the error appropriately
	}

	//// bind diffuse map
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, diffuseMap);
	//// bind specular map
	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, specularMap);
}
glm::mat4 modelPct = glm::mat4(1.0f);
GLint modelPctLoc;

void initPunctiformUniforms() {
	myPunctiformShader.useShaderProgram();

	GLint pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.position");
	glUniform3fv(pctLoc, 1, glm::value_ptr(pctLightPos));
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "viewPos");
	glUniform3fv(pctLoc, 1, glm::value_ptr(myCamera.getPosition()));
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.ambient");
	glUniform3fv(pctLoc, 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.diffuse");
	glUniform3fv(pctLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.specular");
	glUniform3fv(pctLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.0f)));
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.constant");
	glUniform1f(pctLoc, 1.0f);
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.linear");
	glUniform1f(pctLoc, 0.09f);
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "light.quadratic");
	glUniform1f(pctLoc, 0.032f);
	glCheckError();
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "material.shininess");
	glUniform1f(pctLoc, 32.0f);
	glCheckError();

	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "material.diffuse");
	glUniform1i(pctLoc, 0);
	glCheckError();
	if (pctLoc == -1) {
		std::cerr << "Error: Invalid uniform location for material.diffuse." << std::endl;
		// Handle the error appropriately
	}
	pctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "material.specular");
	glUniform1i(pctLoc, 1);
	glCheckError();
	if (pctLoc == -1) {
		std::cerr << "Error: Invalid uniform location for material.specular." << std::endl;
		// Handle the error appropriately
	}

	glm::mat4 projectionPct = glm::perspective(glm::radians(45.0f),
		(float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height,
		0.1f, 500.0f);
	GLint projectionPctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionPctLoc, 1, GL_FALSE, glm::value_ptr(projectionPct));
	glCheckError();
	glm::mat4 viewPct = myCamera.getViewMatrix();
	GLint viewPctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "view");
	glUniformMatrix4fv(viewPctLoc, 1, GL_FALSE, glm::value_ptr(viewPct));
	glCheckError();
	modelPctLoc = glGetUniformLocation(myPunctiformShader.shaderProgram, "model");
	glUniformMatrix4fv(modelPctLoc, 1, GL_FALSE, glm::value_ptr(modelPct));
	glCheckError();
}


void renderTeapot(gps::Shader shader) {
	// select active shader program
	shader.useShaderProgram();

	//send teapot model matrix data to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	//send teapot normal matrix data to shader
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	// draw teapot
	teapot.Draw(shader);
}

void renderLand(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	land.Draw(shader);
}

void renderSky(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	sky.Draw(shader);
}

void renderNightSky(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	nightSky.Draw(shader);
}

void renderDadDuck(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dadDuck.Draw(shader);
}

void renderMomDuck(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	momDuck.Draw(shader);
}

void renderBabyDuck(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	babyDuck.Draw(shader);
}

void renderDock(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dock.Draw(shader);
}

float lotusRotationAngle = 0.0f;
int dirLotus = 1;

void renderLotus(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	lotus.Draw(shader);
}

static int boatDirection = 1;
void renderBoat(gps::Shader shader) {
	shader.useShaderProgram();

	static float boatPosition = 0.0f;
	boatPosition += 0.005f * boatDirection; // adjust the speed as needed
	//std::cout << "Boat position: " << boatPosition << std::endl;
	if (boatPosition >= 50.0f || boatPosition <= 0.0f) {
		boatDirection = -boatDirection; // Change the direction
	}
	glm::mat4 modelBoat = glm::translate(glm::mat4(1.0f), glm::vec3(boatPosition, 0.0f, 0.0f));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBoat));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	boat.Draw(shader);
}

void renderTrees(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	trees.Draw(shader);
}

void renderWindmill(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	windmill.Draw(shader);
}

void renderCat(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	cat.Draw(shader);
}

void renderHouse(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	house.Draw(shader);
}

void renderDogHouse(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dogHouse.Draw(shader);
}

void renderDog(gps::Shader shader) {
	shader.useShaderProgram();
	glm::mat4 modelDog = glm::translate(glm::mat4(1.0f), dogPosition);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelDog));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	dog.Draw(shader);
}

void renderPinwheel(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	pinwheel.Draw(shader);
}

void renderTulip(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	tulip.Draw(shader);
}

GLfloat ropeRotationAngle = 0.0f;
glm::mat4 modelRope;

void renderRope(gps::Shader shader) {
	shader.useShaderProgram();

	glm::vec3 rotationCenter(-37.195f, 4.11882f, 114.449f);
	ropeRotationAngle += 0.005f * boatDirection;
	if (ropeRotationAngle > 360.0f) {
		ropeRotationAngle -= 360.0f;
	}
	else if (ropeRotationAngle < 0.0f) {
		ropeRotationAngle += 360.0f;
	}
	modelRope = glm::translate(glm::mat4(1.0f), rotationCenter);
	modelRope = glm::rotate(modelRope, glm::radians(ropeRotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelRope = glm::translate(modelRope, -rotationCenter);
	//modelRope = inverseTranslationMatrix * rotationMatrix * translationMatrix;
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelRope)));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelRope));
	//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(modelRope));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	/*glm::mat4 viewRope = glm::mat4(1.0f);
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(viewRope));*/
	rope.Draw(shader);
}

void renderSun(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	sun.Draw(shader);
}

void renderLampBox(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	lampBox.Draw(shader);
}

void renderLamp(gps::Shader shader) {
	shader.useShaderProgram();
	// Check if the uniform locations are valid
	if (modelLoc == -1 || normalMatrixLoc == -1) {
		// Handle error or print a message
		std::cerr << "Error: Invalid uniform locations for lamp shader." << std::endl;
		return;
	}
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	lamp.Draw(shader);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after rendering lamp: " << error << std::endl;
	}
}

void renderStageLampBox(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelPctLoc, 1, GL_FALSE, glm::value_ptr(modelPct));
	//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	stageLampBox.Draw(shader);
}

void renderLed(gps::Shader shader) {
	//shader.useShaderProgram();
	//// Check if the uniform locations are valid
	//if (modelLoc == -1) {
	//	// Handle error or print a message
	//	std::cerr << "Error: Invalid uniform locations for lamp shader." << std::endl;
	//	return;
	//}
	//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelLed));
	////glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	//led.Draw(shader);
	//GLenum error = glGetError();
	//if (error != GL_NO_ERROR) {
	//	std::cerr << "OpenGL Error after rendering led: " << error << std::endl;
	//}
	shader.useShaderProgram();
	// Check if the uniform locations are valid
	if (modelPctLoc == -1) {
		// Handle error or print a message
		std::cerr << "Error: Invalid uniform locations for lamp shader." << std::endl;
		return;
	}
	glUniformMatrix4fv(modelPctLoc, 1, GL_FALSE, glm::value_ptr(modelPct));
	//glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	led.Draw(shader);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		std::cerr << "OpenGL Error after rendering led: " << error << std::endl;
	}
}

void renderCandleSupport(gps::Shader shader) {
	shader.useShaderProgram();
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	candleSupport.Draw(shader);
}

float meltingFactor = 0.0f;
glm::vec3 candleCenter = glm::vec3(-3.5504f, 5.8197f, -113.92f);
glm::mat4 modelCandle;
void renderCandle(gps::Shader shader) {
	shader.useShaderProgram();
	if (meltingFactor >= 0.1f) {
		meltingFactor = 0.0f;
	}
	else {
		meltingFactor += 0.0002f;
	}
	/*glm::mat4 scaleMatrix = glm::mat4(1.0f);
	scaleMatrix[1][1] = 1.0f - meltingFactor;
	glm::mat4 modelCandle = model * scaleMatrix;
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCandle));
	glm::mat3 normalMatrixCandle = glm::transpose(glm::inverse(glm::mat3(modelCandle)));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixCandle));*/
	modelCandle = glm::translate(glm::mat4(1.0f), candleCenter);
	modelCandle = glm::scale(modelCandle, glm::vec3(1.0f, 1.0f - (meltingFactor), 1.0f));
	modelCandle = glm::translate(modelCandle, -candleCenter);
	glm::mat3 normalMatrix = glm::transpose(glm::inverse(glm::mat3(modelCandle)));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCandle));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	candle.Draw(shader);
}


void renderFire(gps::Shader shader) {
	shader.useShaderProgram();
	glm::mat4 modelFire = glm::translate(model, glm::vec3(0.0f, -meltingFactor*2.5f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelFire));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	fire.Draw(shader);
}

bool isPointInPolygon(const glm::vec2& point, const glm::vec2& polyFront, const glm::vec2& polyBack) {
	return (point.x >= std::min(polyFront.x, polyBack.x) &&
		point.x <= std::max(polyFront.x, polyBack.x) &&
		point.y >= std::min(polyFront.y, polyBack.y) &&
		point.y <= std::max(polyFront.y, polyBack.y));
}

bool checkCameraDucksCollision() {
	glm::vec2 dadFront = glm::vec2(28.4803f, 2.3f);
	glm::vec2 dadBack = glm::vec2(41.9379f, 2.3f);
	glm::vec2 momFront = glm::vec2(28.4803f, -0.07f);
	glm::vec2 momBack = glm::vec2(41.9379f, -0.07f);
	glm::vec2 cameraPos = glm::vec2(myCamera.getPosition().x, myCamera.getPosition().y);
	bool isInside;// = !isPointInPolygon(cameraPos, dadFront, dadBack) && !isPointInPolygon(cameraPos, momFront, momBack) /* && !isPointInPolygon(cameraPos, dadBack, momBack) && !isPointInPolygon(cameraPos, dadFront, momFront)*/;
	if (cameraPos.x >= dadFront.x && cameraPos.x <= dadBack.x && cameraPos.y >= momBack.y && cameraPos.y <= dadBack.y && myCamera.getPosition().z >= 0.0f) {
		isInside = true;
	}
	else {
		isInside = false;
	}
	if (isInside) {
		std::cout << "Inside radius of ducks: " << cameraPos.x << " " << cameraPos.y << std::endl;
	}
	return isInside;
}


void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glfwMakeContextCurrent(myWindow.getWindow());
	myLampLightShader.useShaderProgram();
	GLint emittedColorLoc = glGetUniformLocation(myLampLightShader.shaderProgram, "emittedColor");
	glUniform3fv(emittedColorLoc, 1, glm::value_ptr(lampColor));
	glCheckError();
	if (emittedColorLoc == -1) {
		std::cerr << "Error: Invalid uniform location for emittedColor in the shader." << std::endl;
	}
	initLampUniforms();
	glCheckError();
	renderLamp(myLampLightShader);
	//renderSun(myLampLightShader);
	glCheckError();

	myLedLightShader.useShaderProgram();
	//glm::vec3 lampPosition = glm::vec3(-148.284f, 3.70126f, 203.259f);
	//GLint pointLightPosLoc = glGetUniformLocation(myLedLightShader.shaderProgram, "pointLightPos");
	//glUniform3f(pointLightPosLoc, lampPosition.x, lampPosition.y, lampPosition.z);
	

	/*initLedUniforms();
	renderLed(myLedLightShader);*/
	myPunctiformShader.useShaderProgram();
	initPunctiformUniforms();
	renderLed(myPunctiformShader);
	renderStageLampBox(myPunctiformShader);
	glCheckError();

	myBasicShader.useShaderProgram();
	/*GLint outsideLightColorLoc = glGetUniformLocation(myBasicShader.shaderProgram, "lightColor");
	glUniform3fv(outsideLightColorLoc, 1, glm::value_ptr(outsideLightColor));
	std::cout << "Light color: " << outsideLightColor[0] << std::endl;
	glCheckError();
	if (outsideLightColorLoc == -1) {
		std::cerr << "Error: Invalid uniform location for lightColor in the shader." << std::endl;
	}*/
	initUniforms();
	renderLand(myBasicShader);
	glCheckError();
	if (!nightMode) {
		renderSky(myBasicShader);
	}
	else {
		renderNightSky(myBasicShader);
	}
	renderDock(myBasicShader);
	renderLotus(myBasicShader);
	renderDadDuck(myBasicShader);
	renderMomDuck(myBasicShader);
	renderBabyDuck(myBasicShader);
	renderBoat(myBasicShader);
	renderTrees(myBasicShader);
	renderWindmill(myBasicShader);
	renderCat(myBasicShader);
	renderHouse(myBasicShader);
	renderDogHouse(myBasicShader);
	renderDog(myBasicShader);
	renderPinwheel(myBasicShader);
	renderTulip(myBasicShader);
	renderRope(myBasicShader);
	/*renderSun(myBasicShader);*/
	renderLampBox(myBasicShader);
	//renderStageLampBox(myBasicShader);
	renderCandleSupport(myBasicShader);
	renderCandle(myBasicShader);
	renderFire(myBasicShader);
	glCheckError();

	fogDensityLoc = glGetUniformLocation(myBasicShader.shaderProgram, "startFog");
	glCheckError();
	glUniform1i(fogDensityLoc, startFog);
	if (fogDensityLoc == -1) {
		std::cerr << "Error: startFog" << std::endl;
	}
	glCheckError();
	fogDensityLoc = glGetUniformLocation(myBasicShader.shaderProgram, "fogDensity");
	glCheckError();
	glUniform1f(fogDensityLoc, fogDensity);
	if (fogDensityLoc == -1) {
		std::cerr << "Error: fogDensity" << std::endl;
	}
	glCheckError();
	glfwPollEvents();
	glCheckError();
	glfwSwapBuffers(myWindow.getWindow());
	glCheckError();
}

void cleanup() {
	//glDeleteTextures(1, &depthMapTexture);
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDeleteFramebuffers(1, &shadowMapFBO);
	myWindow.Delete();
	//cleanup code for your own data
}

int main(int argc, const char* argv[]) {

	try {
		initOpenGLWindow();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	initOpenGLState();
	initModels();
	initShaders();
	//initUniforms();
	//initFBO();
	setWindowCallbacks();

	glCheckError();
	// application loop
	while (!glfwWindowShouldClose(myWindow.getWindow())) {
		processMovement();

		//// Get the current camera position
		//glm::vec3 cameraPosition = myCamera.getPosition();
		////std::cout << "Camera position: " << cameraPosition[0] << " " << cameraPosition[1] << " " << cameraPosition[2] << " " << std::endl;
		//// Get the dog's position (modify this based on your actual dog model)
		//glm::vec3 dogPosition = glm::vec3(-110.646f, 4.71517f, -108.403f);

		//// Define the collision radius (adjust as needed)
		//float collisionRadius = 1.0f;

		//// Check for collision between camera and dog
		//if (checkCameraDogCollision(cameraPosition, dogPosition, collisionRadius)) {
		//	// Collision detected, handle accordingly (e.g., stop camera movement)
		//	// For now, just print a message
		//	std::cout << "Collision with the dog!" << std::endl;
		//}

		if (checkCameraDucksCollision()) {
			myCamera.move(gps::MOVE_BACKWARD, 6 * cameraSpeed);
		}
		//std::cout<<"Camera pos: " << myCamera.getPosition().x<<" "<<myCamera.getPosition().y<<std::endl;
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
		glCheckError();
	}

	cleanup();

	return EXIT_SUCCESS;
}
