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

#include "utils/file.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>

void FramebufferSizeCallback(GLFWwindow *window, int width, int height);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset);
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
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

uint8_t currentBlock = 1;

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
	glfwSetMouseButtonCallback(window, MouseButtonCallback);
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

	texture = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	texture = MakeTexture("res/textures/testatlas_from_google.png", &texture, true);
	shader = MakeShader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl", NULL);
	camera = MakeCamera(&shader, (vec3s){ (NUMBER_OF_CHUNKS_X / 2) * CHUNK_SIZE_X , CHUNK_SIZE_Y - (CHUNK_SIZE_Y - 100) + 10, (NUMBER_OF_CHUNKS_Z / 2) * CHUNK_SIZE_Z }, -90.0f, 0.0f, screenWidth, screenHeight);

	// Read Config File
	char *str = ReadFile("res/settings.config");
	const char *sensString = "Sensitivity = ";
	const char *vdString = "View Distance = ";
	const char *seedString = "Seed = ";
    char word[100];
    sscanf(strstr(str, sensString) + strlen(sensString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, sensString);
	camera.sensitivity = strtof(word, NULL);

	sscanf(strstr(str, vdString) + strlen(vdString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, vdString);
	viewDistance  = (int)strtol(word, (char **)NULL, 10) / 2;

	sscanf(strstr(str, seedString) + strlen(seedString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, seedString);
	SimplexInit((int)strtol(word, (char **)NULL, 10));

	WorldStart();

	SetShaderInteger(&shader, "texture1", 0, true);
	SetShaderMatrix4(&shader, "projection", camera.viewProjection.projection, false);

	currentChunk = GetCurrentChunkCoordinates(camera.position.x, camera.position.z);
	oldChunk = currentChunk;

	// Outline Cube Test
	int oCubeVAO, oCubeVBO, oCubeEBO = 0;
	float oCubeVerticeData[24] = 
	{
		0, 0, 0,
		1, 0, 0,
		1, 1, 0,
		0, 1, 0,
		0, 0, 1,
		1, 0, 1,
		1, 1, 1,
		0, 1, 1
	};

	unsigned int oCubeIndiceData[36] =
	{
		5, 6, 4, 4, 6, 7, // Front Face
		0, 3, 1, 1, 3, 2, // Back Face
		4, 7, 0, 0, 7, 3, // Left Face
		1, 2, 5, 5, 2, 6, // Right Face
		3, 7, 2, 2, 7, 6, // Top Face
		1, 5, 0, 0, 5, 4  // Bottom Face
	};

	glGenVertexArrays(1, &oCubeVAO);
	glGenBuffers(1, &oCubeVBO);
	glGenBuffers(1, &oCubeEBO);

	glBindVertexArray(oCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oCubeVerticeData), oCubeVerticeData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oCubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oCubeIndiceData), oCubeIndiceData, GL_STATIC_DRAW);

	// position attribute
	glBindBuffer(GL_ARRAY_BUFFER, oCubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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
		//printf("Current Chunk(X: %i, Z: %i)\n", currentChunk.x, currentChunk.y);
		currentChunk = GetCurrentChunkCoordinates(camera.position.x, camera.position.z);
		if(oldChunk.x != currentChunk.x || oldChunk.y != currentChunk.y)
		{
			oldChunk = currentChunk;
			UpdateViewDistance(currentChunk);
		}

		UpdateCameraVectors(&camera, &shader);

		WorldRender(&texture, &shader);

		// Outline Cube Test
		ivec3s oCubePos = GetBlockLookedAt(camera.position, camera.front, 8.0f, 0.1f, false);
		mat4s oCubeModel = { 1.0f, 0.0f, 0.0f, 0.0f,
                 		 	0.0f, 1.0f, 0.0f, 0.0f,
                    	 	0.0f, 0.0f, 1.0f, 0.0f,
                    	 	0.0f, 0.0f, 0.0f, 1.0f };
		oCubeModel = glms_translate(oCubeModel, (vec3s){oCubePos.x - 0.025f, oCubePos.y - 0.025f, oCubePos.z - 0.025f});
		oCubeModel = glms_scale(oCubeModel, (vec3s){1.05f, 1.05f, 1.05f});
		SetShaderMatrix4(&shader, "model", oCubeModel, true);
		glBindVertexArray(oCubeVAO);
		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &oCubeVAO);
	glDeleteBuffers(1, &oCubeVBO);
	glDeleteBuffers(1, &oCubeEBO);

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

	if(GetKeyDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		ivec3s block = GetBlockLookedAt(camera.position, camera.front, 8.0f, 0.1f, false);
		if(block.x >= 0 && block.y >= 0 && block.z >= 0)
		{
			struct Chunk *chunk = GetChunk((int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z));
			ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
			BreakBlock(chunk, chunkBlock);
		}
	}
	if(GetKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		ivec3s block = GetBlockLookedAt(camera.position, camera.front, 8.0f, 0.1f, true);
		if(block.x >= 0 && block.y >= 0 && block.z >= 0)
		{
			struct Chunk *chunk = GetChunk((int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z));
			ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
			PlaceBlock(chunk, chunkBlock, currentBlock);
		}
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
			keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
			keysProcessed[key] = false;
		}
	}
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button >= 0 && button < 1024)
	{
		if (action == GLFW_PRESS)
			keys[button] = true;
		else if (action == GLFW_RELEASE)
		{
			keys[button] = false;
			keysProcessed[button] = false;
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
    if(yOffset > 0)
	{
		if(currentBlock == 15)
			currentBlock = 1;
		else
			currentBlock++;
	}
	else if(yOffset < 0)
	{
		if(currentBlock == 1)
			currentBlock = 15;
		else
			currentBlock--;
	}
}