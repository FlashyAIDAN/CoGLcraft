#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/camera.h"
#include "utils/texture.h"
#include "utils/shader.h"
#include "world.h"

#include <stb_image.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void ProcessInput();

bool GetKey(int keyCode);
bool GetKeyDown(int keyCode);
bool GetKeyReleased(int keyCode);

GLFWwindow *window;
unsigned int screenWidth = 960;
unsigned int screenHeight = 540;

bool keys[1024] = { false };
bool keysProcessed[1024] = { false };

float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Camera camera;
struct Texture2D texture;
struct Shader shader;

bool debugLines = false;

int seed = 69420;

ivec2s currentChunk = {0, 0};
ivec2s oldChunk = {0, 0};

int main(int argc, const char *argv[])
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	#endif
	glfwWindowHint(GLFW_RESIZABLE, true);

	window = glfwCreateWindow(screenWidth, screenHeight, "CoGLcraft", NULL, NULL);
	glfwMakeContextCurrent(window);

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		printf("Failed to initialize GLAD\n");
		return -1;
	}

	glfwSetKeyCallback(window, KeyCallback);
	glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
	glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glViewport( 0, 0, screenWidth, screenHeight);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Initialization Background
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glfwSwapBuffers(window);
	
	// Set Icon
	GLFWimage images[1];
	images[0].pixels = stbi_load("res/icons/first.png", &images[0].width, &images[0].height, 0, 4);
	glfwSetWindowIcon(window, 1, images);
	stbi_image_free(images[0].pixels);

	SimplexInit(seed);

	texture = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	texture = MakeTexture("res/textures/testatlas_from_google.png", &texture, true);
	shader = MakeShader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl", NULL);
	camera = MakeCamera(&shader, (vec3s){ (NUMBER_OF_CHUNKS_X / 2) * CHUNK_SIZE_X , CHUNK_SIZE_Y - (CHUNK_SIZE_Y - 100) + 10, (NUMBER_OF_CHUNKS_Z / 2) * CHUNK_SIZE_Z }, -90.0f, 0.0f, screenWidth, screenHeight);

	WorldStart();

	SetShaderInteger(&shader, "texture1", 0, true);
	SetShaderMatrix4(&shader, "projection", camera.viewProjection.projection, false);

	currentChunk = GetCurrentChunkCoordinates(camera.position.x, camera.position.z);
	oldChunk = currentChunk;

	while(!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput();
		glfwPollEvents();

		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get Current Chunk
		printf("Current Chunk(X: %i, Z: %i)\n", currentChunk.x, currentChunk.y);
		currentChunk = GetCurrentChunkCoordinates(camera.position.x, camera.position.z);
		if(oldChunk.x != currentChunk.x || oldChunk.y != currentChunk.y)
		{
			oldChunk = currentChunk;
			UpdateViewDistance(currentChunk);
		}

		UpdateCameraVectors(&camera, &shader);

		WorldRender(&texture, &shader);

		glfwSwapBuffers(window);
	}

	SimplexFree();
	WorldDelete();

	glfwTerminate();
	return 0;
}

void ProcessInput()
{
	if(GetKeyDown(GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);

	if(GetKeyDown(GLFW_KEY_F1))
	{
		if(!debugLines)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			debugLines = true;
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			debugLines = false;
		}
	}

	float velocity = camera.speed * deltaTime;
    if(GetKey(GLFW_KEY_W))
        camera.position = glms_vec3_add(camera.position, glms_vec3_mul(camera.front, (vec3s){velocity, velocity, velocity}));
    if(GetKey(GLFW_KEY_S))
        camera.position = glms_vec3_sub(camera.position, glms_vec3_mul(camera.front, (vec3s){velocity, velocity, velocity}));
    if(GetKey(GLFW_KEY_A))
        camera.position = glms_vec3_sub(camera.position, glms_vec3_mul(camera.right, (vec3s){velocity, velocity, velocity}));
    if(GetKey(GLFW_KEY_D))
        camera.position = glms_vec3_add(camera.position, glms_vec3_mul(camera.right, (vec3s){velocity, velocity, velocity}));
	if(GetKey(GLFW_KEY_SPACE))
		camera.position = glms_vec3_add(camera.position, glms_vec3_mul(camera.worldUp, (vec3s){velocity, velocity, velocity}));
	if(GetKey(GLFW_KEY_LEFT_SHIFT))
		camera.position = glms_vec3_sub(camera.position, glms_vec3_mul(camera.worldUp, (vec3s){velocity, velocity, velocity}));
	if(GetKey(GLFW_KEY_Q))
		camera.position = glms_vec3_add(camera.position, glms_vec3_mul(camera.up, (vec3s){velocity, velocity, velocity}));
	if(GetKey(GLFW_KEY_E))
		camera.position = glms_vec3_sub(camera.position, glms_vec3_mul(camera.up, (vec3s){velocity, velocity, velocity}));

	if(GetKeyDown(GLFW_KEY_EQUAL))
		camera.speed++;
	if(GetKeyDown(GLFW_KEY_MINUS))
	{
		camera.speed--;
		if(camera.speed <= 1.0f)
			camera.speed = 1.0f;
	}
}

bool GetKey(int keyCode)
{
    if(keys[keyCode] && !keysProcessed[keyCode])
        return true;
    else
        return false;
}

bool GetKeyDown(int keyCode)
{
    if (keys[keyCode] && !keysProcessed[keyCode])
    {
        keysProcessed[keyCode] = true;
        return true;
    }
    else
        return false;
}

bool GetKeyReleased(int keyCode)
{
    if (!keys[keyCode])
    {
        return true;
    }
    else
        return false;
}

void KeyCallback(GLFWwindow *window, int key, int scancodem, int action, int mode)
{
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			keysProcessed[key] = false;
		}
	}
}

void FramebufferSizeCallback(GLFWwindow *window, int width, int height)
{
	screenWidth = width;
	screenHeight = height;
	UpdateCameraPerspective(&camera, &shader, width, height);

	glViewport(0, 0, width, height);
}

bool firstMouse = true;
float lastX, lastY = 0;
void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if(firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos; // Reversed since y-coordinates range from bottom to top unlike x-coordinates

    lastX = xPos;
    lastY = yPos;

    xOffset *= camera.sensitivity;
    yOffset *= camera.sensitivity;

    camera.yaw += xOffset;
    camera.pitch += yOffset;

    camera.yaw += xOffset;
    camera.pitch += yOffset;

    if(camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if(camera.pitch < -89.0f)
        camera.pitch = -89.0f;
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    
}