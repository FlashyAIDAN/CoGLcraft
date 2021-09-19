#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/camera.h"
#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/emath.h"
#include "player.h"
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
bool fullscreen = false;
unsigned int screenWidth = 960;
unsigned int screenHeight = 540;

bool keys[1024] = { false };
bool keysProcessed[1024] = { false };

float currentFrame = 0.0f;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

struct Texture2D texture;
struct Texture2D crosshair;
struct Texture2D currentBlockItems[(sizeof(voxels) / sizeof(voxels[0])) - 1];
struct Shader uiShader;
struct Shader playerShader;
struct Player player;

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
	crosshair = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	crosshair = MakeTexture("res/textures/simple_crosshair.png", &crosshair, true);
	for(int i = 0; i < (sizeof(voxels) / sizeof(voxels[0])) - 1; i++)
	{
		currentBlockItems[i] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);

		const char *s1 = "res/textures/items/";
		const char *s2 = voxels[i + 1].name;
		const char *s3 = ".png";
		char *result = malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    	// in real code you would check for errors in malloc here
    	strcpy(result, s1);
    	strcat(result, s2);

		char *result1 = malloc(strlen(result) + strlen(s3) + 1); // +1 for the null-terminator
    	// in real code you would check for errors in malloc here
    	strcpy(result1, result);
    	strcat(result1, s3);

		currentBlockItems[i] = MakeTexture(result1, &currentBlockItems[i], true);
	}
	playerShader = MakeShader("res/shaders/chunk_vertex.glsl", "res/shaders/chunk_fragment.glsl", NULL);
	uiShader = MakeShader("res/shaders/ui_vertex.glsl", "res/shaders/ui_fragment.glsl", NULL);
	player = MakePlayer(playerShader, screenWidth, screenHeight);
	player.position = (vec3s){ (NUMBER_OF_CHUNKS_X / 2) * CHUNK_SIZE_X , CHUNK_SIZE_Y - (CHUNK_SIZE_Y - 100) + 10, (NUMBER_OF_CHUNKS_Z / 2) * CHUNK_SIZE_Z };

	// Read Config File
	char *str = ReadFile("res/settings.config");
	const char *sensString = "Sensitivity = ";
	const char *vdString = "View Distance = ";
	const char *seedString = "Seed = ";
    char word[100];
    sscanf(strstr(str, sensString) + strlen(sensString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, sensString);
	player.camera.sensitivity = strtof(word, NULL);

	sscanf(strstr(str, vdString) + strlen(vdString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, vdString);
	viewDistance  = (int)strtol(word, (char **)NULL, 10);

	sscanf(strstr(str, seedString) + strlen(seedString), "%99s", word);
    //printf("Found \"%s\" after \"%s\"\n\n", word, seedString);
	SimplexInit((int)strtol(word, (char **)NULL, 10));

	WorldStart(&player.shader);

	SetShaderInteger(&uiShader, "sprite", 0, true);
	SetShaderMatrix4(&uiShader, "projection", glms_ortho(-((float)(screenWidth / igcd(screenWidth, screenHeight)) / 2.0f / 10.0f), (float)(screenWidth / igcd(screenWidth, screenHeight)) / 2.0f / 10.0f, -((float)(screenHeight / igcd(screenWidth, screenHeight)) / 2.0f / 10.0f), (float)(screenHeight / igcd(screenWidth, screenHeight) / 2.0f / 10.0f), -1.0f, 1.0f), false);

	currentChunk = GetCurrentChunkCoordinates(player.position.x, player.position.z);
	oldChunk = currentChunk;

	// Outline Cube Test
	unsigned int oCubeVAO, oCubeVBO, oCubeEBO = 0;
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

	float uiVertices[16] =
	{
        // pos      // tex     // colors           // texture coords
        -0.5, -0.5, 1.0f, 1.0f, // top right
        0.5, -0.5, 1.0f, 0.0f, // bottom right
        0.5, 0.5, 0.0f, 0.0f, // bottom left
        -0.5, 0.5, 0.0f, 1.0f  // top left 
    };

	unsigned int uiIndices[6] =
	{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int uiVBO, uiVAO, uiEBO = 0;

    glGenVertexArrays(1, &uiVAO);
    glGenBuffers(1, &uiVBO);
    glGenBuffers(1, &uiEBO);

    glBindVertexArray(uiVAO);

    glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uiVertices), uiVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, uiEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uiIndices), uiIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, uiVBO);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
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
		currentChunk = GetCurrentChunkCoordinates(player.position.x, player.position.z);
		if(oldChunk.x != currentChunk.x || oldChunk.y != currentChunk.y)
		{
			oldChunk = currentChunk;
			UpdateViewDistance(currentChunk);
		}

		PlayerUpdate(&player, deltaTime);

		// UseShader(&shader);
		WorldRender(&texture, &player.shader);

		// Outline Cube Test
		ivec3s oCubePos = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, false);
		mat4s oCubeModel = { 1.0f, 0.0f, 0.0f, 0.0f,
                 		 	0.0f, 1.0f, 0.0f, 0.0f,
                    	 	0.0f, 0.0f, 1.0f, 0.0f,
                    	 	0.0f, 0.0f, 0.0f, 1.0f };
		oCubeModel = glms_translate(oCubeModel, (vec3s){oCubePos.x - 0.025f, oCubePos.y - 0.025f, oCubePos.z - 0.025f});
		oCubeModel = glms_scale(oCubeModel, (vec3s){1.05f, 1.05f, 1.05f});
		SetShaderMatrix4(&player.shader, "model", oCubeModel, true);
		glBindVertexArray(oCubeVAO);
		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);

		UseShader(&uiShader);
		mat4s uiModel = (mat4s){ 1.0f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 1.0f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f };
		 
    	uiModel = glms_scale(uiModel, (vec3s){0.025f, 0.025f, 1.0f});

		SetShaderMatrix4(&uiShader, "model", uiModel, false);

    	// render textured quad
   		SetShaderVector3fv(&uiShader, "spriteColor", (vec3s){1.0f, 1.0f, 1.0f}, false);

    	glActiveTexture(GL_TEXTURE0);
    	BindTexture(&crosshair);

    	glBindVertexArray(uiVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		UseShader(&uiShader);
		mat4s uiModelBlock = (mat4s){ 1.0f, 0.0f, 0.0f, 0.0f,
                                 	  0.0f, 1.0f, 0.0f, 0.0f,
                                	  0.0f, 0.0f, 1.0f, 0.0f,
                                	  0.0f, 0.0f, 0.0f, 1.0f };
		
		uiModelBlock = glms_translate(uiModelBlock, (vec3s){-0.675f, -0.33f, 0.0f});
		uiModelBlock = glms_rotate(uiModelBlock, glm_rad(270.0f), (vec3s){0.0f, 0.0f, 1.0f});
    	uiModelBlock = glms_scale(uiModelBlock, (vec3s){0.25f, 0.25f, 1.0f});

		SetShaderMatrix4(&uiShader, "model", uiModelBlock, false);

    	// render textured quad
   		SetShaderVector3fv(&uiShader, "spriteColor", (vec3s){1.0f, 1.0f, 1.0f}, false);

    	glActiveTexture(GL_TEXTURE0);
    	BindTexture(&currentBlockItems[currentBlock - 1]);

    	glBindVertexArray(uiVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &oCubeVAO);
	glDeleteBuffers(1, &oCubeVBO);
	glDeleteBuffers(1, &oCubeEBO);
	glDeleteVertexArrays(1, &uiVAO);
	glDeleteBuffers(1, &uiVBO);
	glDeleteBuffers(1, &uiEBO);

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

	if(GetKeyDown(GLFW_KEY_F11))
	{
		if (fullscreen)
		{
			unsigned int width = 960, height = 540;
			glfwSetWindowMonitor(window, NULL, width / 4, height / 4, width, height, 0);
			fullscreen = false;
		}
		else
		{
			GLFWmonitor *monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			fullscreen = true;
		}
	}

	if (GetKey(GLFW_KEY_W))
        player.vertical = 1;
    if (GetKey(GLFW_KEY_S))
        player.vertical = -1;
    if (GetKeyReleased(GLFW_KEY_W) && GetKeyReleased(GLFW_KEY_S))
        player.vertical = 0;

    if (GetKey(GLFW_KEY_A))
        player.horizontal = -1;
    if (GetKey(GLFW_KEY_D))
        player.horizontal = 1;
    if (GetKeyReleased(GLFW_KEY_A) && GetKeyReleased(GLFW_KEY_D))
        player.horizontal = 0;

    if (GetKey(GLFW_KEY_LEFT_SHIFT))
        player.isSprinting = true;
    else
        player.isSprinting = false;

    if (player.isGrounded && GetKey(GLFW_KEY_SPACE))
    {
        player.jumpRequest = true;
    }

	if(GetKeyDown(GLFW_KEY_EQUAL))
		player.speed++;
	if(GetKeyDown(GLFW_KEY_MINUS))
	{
		player.speed--;
		if(player.speed <= 1.0f)
			player.speed = 1.0f;
	}

	if(GetKeyDown(GLFW_MOUSE_BUTTON_LEFT))
	{
		ivec3s block = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, false);
		if(block.x >= 0 && block.y >= 0 && block.z >= 0)
		{
			struct Chunk *chunk = GetChunk((int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z));
			ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
			EditVoxel(chunk, chunkBlock, 0);
		}
	}
	if(GetKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
	{
		ivec3s block = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, true);
		if(block.x >= 0 && block.y >= 0 && block.z >= 0)
		{
			struct Chunk *chunk = GetChunk((int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z));
			ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
			EditVoxel(chunk, chunkBlock, currentBlock);
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
	UpdateCameraPerspective(&player.camera, &player.shader, width, height);
	//SetShaderMatrix4(&uiShader, "projection", glms_ortho(-((float)(width / igcd(width, height)) / 2.0f / 10.0f), (float)(width / igcd(width, height)) / 2.0f / 10.0f, -((float)(height / igcd(width, height)) / 2.0f / 10.0f), (float)(height / igcd(width, height) / 2.0f / 10.0f), -1.0f, 1.0f), true);

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

    xOffset *= player.camera.sensitivity;
    yOffset *= player.camera.sensitivity;

    player.camera.yaw += xOffset;
    player.camera.pitch += yOffset;

    player.camera.yaw += xOffset;
    player.camera.pitch += yOffset;

    if(player.camera.pitch > 89.0f)
        player.camera.pitch = 89.0f;
    if(player.camera.pitch < -89.0f)
        player.camera.pitch = -89.0f;
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
    if(yOffset > 0)
	{
		if(currentBlock == (sizeof(voxels) / sizeof(voxels[0])) - 1)
			currentBlock = 1;
		else
			currentBlock++;
	}
	else if(yOffset < 0)
	{
		if(currentBlock == 1)
			currentBlock = (sizeof(voxels) / sizeof(voxels[0])) - 1;
		else
			currentBlock--;
	}
}