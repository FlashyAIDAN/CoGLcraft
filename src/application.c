#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/camera.h"
#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/emath.h"
#include "ui.h"
#include "player.h"
#include "world.h"

#include <stb_image.h>

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "io.h"

#include "utils/file.h"

#include <stddef.h>
#include <ctype.h>
#include <string.h>


#define TIME_MAX 2400

#define DAY_COLOR_R 0.25f
#define DAY_COLOR_G 0.25f
#define DAY_COLOR_B 1.0f
#define NIGHT_COLOR_R 0.0f
#define NIGHT_COLOR_G 0.0f
#define NIGHT_COLOR_B 0.75f

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
struct Texture2D inventoryTexture;
struct Texture2D hotbarTexture;
struct Texture2D breakTexture[10];
struct Texture2D cloudsTexture;
struct Shader uiShader;
struct Shader playerShader;
struct Player player;
struct UIrenderer uiRenderer;

ivec3s oldBLockBreaking = {0, 0, 0};
float currentBlockBreakingHealth = 0.0f;
float currentBlockBreakingMaxHealth = 0.0f;
bool playerBreaking = false;

bool debugLines = false;

int seed = 69420;

ivec2s currentChunk = {0, 0};
ivec2s oldChunk = {0, 0};

uint8_t currentBlock = 1;

float oldTime = 0.0f;
float time = 0.0f;
float globalLightLevel = 0.0f;

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

	glViewport(0, 0, screenWidth, screenHeight);
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
	inventoryTexture = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	inventoryTexture = MakeTexture("res/textures/updated_inventory.png", &inventoryTexture, false);

	hotbarTexture = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	hotbarTexture = MakeTexture("res/textures/hotbar.png", &hotbarTexture, false);

	breakTexture[0] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[0] = MakeTexture("res/textures/break/1.png", &breakTexture[0], false);
	breakTexture[1] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[1] = MakeTexture("res/textures/break/2.png", &breakTexture[1], false);
	breakTexture[2] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[2] = MakeTexture("res/textures/break/3.png", &breakTexture[2], false);
	breakTexture[3] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[3] = MakeTexture("res/textures/break/4.png", &breakTexture[3], false);
	breakTexture[4] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[4] = MakeTexture("res/textures/break/5.png", &breakTexture[4], false);
	breakTexture[5] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[5] = MakeTexture("res/textures/break/6.png", &breakTexture[5], false);
	breakTexture[6] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[6] = MakeTexture("res/textures/break/7.png", &breakTexture[6], false);
	breakTexture[7] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[7] = MakeTexture("res/textures/break/8.png", &breakTexture[7], false);
	breakTexture[8] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[8] = MakeTexture("res/textures/break/9.png", &breakTexture[8], false);
	breakTexture[9] = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	breakTexture[9] = MakeTexture("res/textures/break/10.png", &breakTexture[9], false);

	cloudsTexture = CreateTextureData(GL_RGBA, GL_RGBA, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	cloudsTexture = MakeTexture("res/textures/shit_clouds.png", &cloudsTexture, false);

	playerShader = MakeShader("res/shaders/chunk_vertex.glsl", "res/shaders/chunk_fragment.glsl", NULL);

	uiShader = MakeShader("res/shaders/ui_vertex.glsl", "res/shaders/ui_fragment.glsl", NULL);

	player = MakePlayer(playerShader, screenWidth, screenHeight);
	player.position = (vec3s){ (NUMBER_OF_CHUNKS_X / 2) * CHUNK_SIZE_X , CHUNK_SIZE_Y - (CHUNK_SIZE_Y - 100) + 10, (NUMBER_OF_CHUNKS_Z / 2) * CHUNK_SIZE_Z };
	if(access( "res/saves/main/player/player.data", 0 ) != -1)
		LoadPlayer(&player);
	
	uiRenderer = MakeUIrenderer(uiShader, screenWidth, screenHeight);

	// Read Config File
	char *str = ReadFileToString("res/settings.config");
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

	currentChunk = GetCurrentChunkCoordinates((vec2s){player.position.x, player.position.z});
	oldChunk = currentChunk;
	WorldStart(&player.shader, currentChunk);

	// Outline Cube Test
	unsigned int oCubeVAO, oCubeVBO, oCubeEBO, oCubeC, oCubeUV = 0;
	unsigned int oLineVAO, oLineVBO, oLineEBO  = 0;
	float oCubeVerticeData[36] = 
	{
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 1.0f
	};

	float oCubeUVData[24] = 
	{
		0.0f, 0.0f,
		1.0f, 0.0f,
    	1.0f, 1.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 1.0f
	};

	float oCubeColorData[36] = 
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,

		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	unsigned int oCubeIndiceData[36] =
	{
		5, 6, 4, 4, 6, 7, // Front Face
		0, 3, 1, 1, 3, 2, // Back Face
		4, 7, 0, 0, 7, 3, // Left Face
		1, 2, 5, 5, 2, 6, // Right Face
		3, 8, 2, 2, 8, 10, // Top Face
		1, 9, 0, 0, 9, 11  // Bottom Face
	};

	glGenVertexArrays(1, &oCubeVAO);
	glGenBuffers(1, &oCubeVBO);
	glGenBuffers(1, &oCubeEBO);
	glGenBuffers(1, &oCubeC);
	glGenBuffers(1, &oCubeUV);

	glBindVertexArray(oCubeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oCubeVerticeData), oCubeVerticeData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeC);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oCubeColorData), oCubeColorData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeUV);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oCubeUVData), oCubeUVData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oCubeEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oCubeIndiceData), oCubeIndiceData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeC);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, oCubeUV);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &oLineVAO);
	glGenBuffers(1, &oLineVBO);
	glGenBuffers(1, &oLineEBO);

	glBindVertexArray(oLineVAO);

	glBindBuffer(GL_ARRAY_BUFFER, oLineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(oCubeVerticeData), oCubeVerticeData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, oLineEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oCubeIndiceData), oCubeIndiceData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, oLineVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	float cloudVertices[12] =
	{
        // pos      // tex     // colors           // texture coords
        -0.5f, -0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
         0.5f,  0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f  // top left 
    };

	float cloudUVs[8] =
	{
		1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f
	};

	float cloudColors[12] =
	{
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f
	};

	unsigned int cloudIndices[6] =
	{
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int cloudVAO, cloudVBO, cloudEBO, cloudColor , cloudUV = 0;

    glGenVertexArrays(1, &cloudVAO);
    glGenBuffers(1, &cloudVBO);
    glGenBuffers(1, &cloudEBO);
    glGenBuffers(1, &cloudColor);
    glGenBuffers(1, &cloudUV);

    glBindVertexArray(cloudVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudVertices), cloudVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cloudColor);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudColors), cloudColors, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cloudUV);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cloudUVs), cloudUVs, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cloudEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cloudIndices), cloudIndices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, cloudVBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, cloudColor);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, cloudUV);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

	char *str1 = ReadFileToString("res/saves/main/world.data");
	const char *s1 = "Time = ";
    char word1[100];

    sscanf(strstr(str1, s1) + strlen(s1), "%99s", word1);
	time = strtof(word1, NULL);

	if(time <= TIME_MAX / 2)
		globalLightLevel = (time / (float)TIME_MAX) * 2;
	else
		globalLightLevel = 2.0f - ((time / (float)TIME_MAX) * 2);

	SetShaderFloat(&player.shader, "globalLightLevel", globalLightLevel, true);

	lastFrame = glfwGetTime();
	while(!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		time += deltaTime * 300;
		if(time > TIME_MAX)
		{
			oldTime = 0.0f;
			time = 0.0f;
		}


		if(time <= TIME_MAX / 2)
			globalLightLevel = (time / (float)TIME_MAX) * 2;
		else
			globalLightLevel = 2.0f - ((time / (float)TIME_MAX) * 2);

		if(time > oldTime + 50.0f)
		{
			chunkGlobalLightLevel = globalLightLevel;
			SetShaderFloat(&player.shader, "globalLightLevel", globalLightLevel, true);
			oldTime = time;
		}
		
		ProcessInput();
		glfwPollEvents();

		glClearColor(lerp(DAY_COLOR_R, NIGHT_COLOR_R, globalLightLevel), lerp(DAY_COLOR_G, NIGHT_COLOR_G, globalLightLevel), lerp(DAY_COLOR_B, NIGHT_COLOR_B, globalLightLevel), 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Get Current Chunk
		//printf("Current Chunk(X: %i, Z: %i)\n", currentChunk.x, currentChunk.y);
		currentChunk = GetCurrentChunkCoordinates((vec2s){player.position.x, player.position.z});
		if(oldChunk.x != currentChunk.x || oldChunk.y != currentChunk.y)
		{
			oldChunk = currentChunk;
			UpdateViewDistance(currentChunk);
		}

		PlayerUpdate(&player, deltaTime);

		WorldRender(&texture, &player.shader);

		int currentBreakTextureIndex = 0;

		if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - (currentBlockBreakingMaxHealth / 10.0f))
			currentBreakTextureIndex = 0;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 2))
			currentBreakTextureIndex = 1;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 3))
			currentBreakTextureIndex = 2;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 4))
			currentBreakTextureIndex = 3;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 5))
			currentBreakTextureIndex = 4;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 6))
			currentBreakTextureIndex = 5;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 7))
			currentBreakTextureIndex = 6;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 8))
			currentBreakTextureIndex = 7;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 9))
			currentBreakTextureIndex = 8;
		else if(currentBlockBreakingHealth >= currentBlockBreakingMaxHealth - ((currentBlockBreakingMaxHealth / 10.0f) * 10))
			currentBreakTextureIndex = 9;

		// Outline Cube Test
		ivec3s oCubePos = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, false);
		mat4s oCubeModel = { 1.0f, 0.0f, 0.0f, 0.0f,
                 		 	0.0f, 1.0f, 0.0f, 0.0f,
                    	 	0.0f, 0.0f, 1.0f, 0.0f,
                    	 	0.0f, 0.0f, 0.0f, 1.0f };
		oCubeModel = glms_translate(oCubeModel, (vec3s){oCubePos.x - 0.025f, oCubePos.y - 0.025f, oCubePos.z - 0.025f});
		oCubeModel = glms_scale(oCubeModel, (vec3s){1.05f, 1.05f, 1.05f});
		SetShaderMatrix4(&player.shader, "model", oCubeModel, true);
		glBindVertexArray(oLineVAO);
		glDrawElements(GL_LINES, 36, GL_UNSIGNED_INT, 0);
		glBindVertexArray(oCubeVAO);
		glActiveTexture(GL_TEXTURE0);
    	BindTexture(&breakTexture[currentBreakTextureIndex]);
		if(playerBreaking)
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		mat4s cloudModel = { 1.0f, 0.0f, 0.0f, 0.0f,
                 		 	0.0f, 1.0f, 0.0f, 0.0f,
                    	 	0.0f, 0.0f, 1.0f, 0.0f,
                    	 	0.0f, 0.0f, 0.0f, 1.0f };
		cloudModel = glms_translate(cloudModel, (vec3s){player.position.x - 500.0f, 200.0f, player.position.z + 500.0f /* SIZE / 2*/});
		cloudModel = glms_rotate(cloudModel, glm_rad(270.0f), (vec3s){1.0f, 0.0f, 0.0f});
		cloudModel = glms_scale(cloudModel, (vec3s){1000.0f, 1000.0f, 1000.0f});
		SetShaderMatrix4(&player.shader, "model", cloudModel, true);
		SetShaderInteger(&player.shader, "fog", false, false);

		glBindVertexArray(oCubeVAO);
		glActiveTexture(GL_TEXTURE0);
    	BindTexture(&cloudsTexture);
		glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

		if(!player.inInventory)
		{
			RenderSpriteUI(&uiRenderer, &crosshair, (vec2s){0.0f, 0.0f}, 0.0f, (vec2s){0.025f, 0.025f}, (vec3s){1.0f, 1.0f, 1.0f});
			RenderSpriteUI(&uiRenderer, &hotbarTexture, (vec2s){0.0f, -0.375f}, 90.0f, (vec2s){0.127f, 0.7f}, (vec3s){1.0f, 1.0f, 1.0f});
		}
		RenderSpriteUI(&uiRenderer, &currentBlockItems[currentBlock - 1], (vec2s){-0.675f, -0.33f}, 270.0f, (vec2s){0.25f, 0.25f}, (vec3s){1.0f, 1.0f, 1.0f});
		if(player.inInventory)
			RenderSpriteUI(&uiRenderer, &inventoryTexture, (vec2s){0.0f, 0.0f}, 90.0f, (vec2s){0.75f, 0.75f}, (vec3s){1.0f, 1.0f, 1.0f});

		glfwSwapBuffers(window);
	}

	FILE *inf;
	inf = fopen ("res/saves/main/world.data", "w");
	if (inf == NULL)
        printf("Cant open file!");
    fprintf(inf, "Name = main\nSeed = %i\nTime = %f", seed, time);
	fclose(inf);

	glDeleteVertexArrays(1, &oCubeVAO);
	glDeleteBuffers(1, &oCubeVBO);
	glDeleteBuffers(1, &oCubeEBO);
	glDeleteBuffers(1, &oCubeC);
	glDeleteBuffers(1, &oCubeUV);

	glDeleteVertexArrays(1, &oLineVAO);
	glDeleteBuffers(1, &oLineVBO);
	glDeleteBuffers(1, &oLineEBO);

	glDeleteVertexArrays(1, &cloudVAO);
	glDeleteBuffers(1, &cloudVBO);
	glDeleteBuffers(1, &cloudEBO);
	glDeleteBuffers(1, &cloudColor);
	glDeleteBuffers(1, &cloudUV);

	DeleteUIrenderer(&uiRenderer);

	DeletePlayer(&player);
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

	if(GetKeyDown(GLFW_KEY_F4))
		player.position = (vec3s){ player.position.x, CHUNK_SIZE_Y, player.position.z };

	if(!player.inInventory)
	{
		if (GetKey(GLFW_KEY_W))
			player.vertical = lerp(player.vertical, player.speed, player.acceleration);
		if (GetKey(GLFW_KEY_S))
			player.vertical = lerp(player.vertical, -player.speed, player.acceleration);

		if (GetKey(GLFW_KEY_A))
			player.horizontal = lerp(player.horizontal, -player.speed, player.acceleration);
		if (GetKey(GLFW_KEY_D))
			player.horizontal = lerp(player.horizontal, player.speed, player.acceleration);

		if (GetKey(GLFW_KEY_LEFT_SHIFT))
			player.isSprinting = true;
		else
			player.isSprinting = false;

		if (player.isGrounded && GetKey(GLFW_KEY_SPACE))
		{
			player.jumpRequest = true;
		}

		if(GetKey(GLFW_MOUSE_BUTTON_LEFT))
		{
			playerBreaking = true;
			ivec3s block = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, false);
			if(block.x == oldBLockBreaking.x && block.y == oldBLockBreaking.y && block.z == oldBLockBreaking.z)
			{
				currentBlockBreakingHealth -= 20.0f * deltaTime;
				if(currentBlockBreakingHealth <= 0.0f && block.x >= 0 && block.y >= 0 && block.z >= 0)
				{
					struct Chunk *chunk = GetChunk((ivec2s){(int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z)});
					ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
					EditVoxel(chunk, chunkBlock, 0);
				}
			}
			else
			{
				oldBLockBreaking = block;
				if(block.x >= 0 && block.y >= 0 && block.z >= 0)
				{
					struct Chunk *chunk = GetChunk((ivec2s){(int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z)});
					ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
					currentBlockBreakingHealth = voxels[GetVoxel(chunk, chunkBlock.x, chunkBlock.y, chunkBlock.z)].health;
					currentBlockBreakingMaxHealth = voxels[GetVoxel(chunk, chunkBlock.x, chunkBlock.y, chunkBlock.z)].health;
				}
			}
		}
		else
		{
			playerBreaking = false;
			currentBlockBreakingHealth = currentBlockBreakingMaxHealth;
		}
		if(GetKeyDown(GLFW_MOUSE_BUTTON_RIGHT))
		{
			ivec3s block = GetBlockLookedAt(player.camera.position, player.camera.front, player.reach, player.checkIncrement, true);
			if(block.x >= 0 && block.y >= 0 && block.z >= 0)
			{
				struct Chunk *chunk = GetChunk((ivec2s){(int)floorf(block.x / CHUNK_SIZE_X), (int)floorf(block.z / CHUNK_SIZE_Z)});
				ivec3s chunkBlock = (ivec3s){block.x - chunk->position.x, block.y - chunk->position.y, block.z - chunk->position.z};
				EditVoxel(chunk, chunkBlock, currentBlock);
			}
		}
	}

	if(GetKeyDown(GLFW_KEY_E))
	{
		player.inInventory = !player.inInventory;
		if(player.inInventory)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if(GetKeyDown(GLFW_KEY_EQUAL))
		player.speed++;
	if(GetKeyDown(GLFW_KEY_MINUS))
	{
		player.speed--;
		if(player.speed <= 1.0f)
			player.speed = 1.0f;
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

	if(!player.inInventory)
	{
		xOffset *= player.camera.sensitivity;
		yOffset *= player.camera.sensitivity;

		player.camera.yaw = fmod((player.camera.yaw + xOffset), 360.0f);
		player.camera.pitch += yOffset;

		if(player.camera.pitch > 89.0f)
			player.camera.pitch = 89.0f;
		if(player.camera.pitch < -89.0f)
			player.camera.pitch = -89.0f;
	}
}

void ScrollCallback(GLFWwindow *window, double xOffset, double yOffset)
{
	if(!player.inInventory)
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
}