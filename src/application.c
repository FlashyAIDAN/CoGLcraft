#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "utils/camera.h"
#include "utils/texture.h"
#include "utils/shader.h"
#include "utils/vector.h"

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

	texture = CreateTextureData(GL_RGB, GL_RGB, GL_REPEAT, GL_REPEAT, GL_NEAREST, GL_NEAREST);
	texture = MakeTexture("res/textures/dirt.jpg", &texture, true);
	shader = MakeShader("res/shaders/vertex.glsl", "res/shaders/fragment.glsl", NULL);
	camera = MakeCamera(&shader, (vec3s){ 0.0f, 0.0f, 3.0f }, -90.0f, 0.0f, screenWidth, screenHeight);

	float vertices[12][3] = 
	{
	 	{ 1.0f, 1.0f, 0.0f },
	 	{ 1.0f, 0.0f, 0.0f },

	 	{ 0.0f, 0.0f, 0.0f },
	 	{ 0.0f, 1.0f, 0.0f },

	 	{ 1.0f, 1.0f, 1.0f },
	 	{ 1.0f, 0.0f, 1.0f },

	 	{ 0.0f, 0.0f, 1.0f },
	 	{ 0.0f, 1.0f, 1.0f },

	 	{ 0.0f, 1.0f, 1.0f },
	 	{ 1.0f, 0.0f, 1.0f },

	 	{ 1.0f, 1.0f, 1.0f }, 
	 	{ 0.0f, 0.0f, 1.0f }
	};

	float uvs[12][2] =
	{
		{ 1.0f, 1.0f },
	 	{ 1.0f, 0.0f },

	 	{ 0.0f, 0.0f },
	 	{ 0.0f, 1.0f },

	 	{ 0.0f, 1.0f },
	 	{ 0.0f, 0.0f },

		{ 1.0f, 0.0f },
		{ 1.0f, 1.0f },

		{ 0.0f, 0.0f },
		{ 1.0f, 1.0f },

		{ 1.0f, 0.0f },
		{ 0.0f, 1.0f }
	};

	unsigned int indices[6][6] =
	{
		{ 7, 6, 5,  7, 5,  4 }, // FRONT
		{ 0, 1, 3,  1, 2,  3 }, // BACK
		{ 2, 6, 7,  7, 3,  2 }, // LEFT
		{ 1, 4, 5,  4, 1,  0 }, // RIGHT
		{ 0, 3, 10, 8, 10, 3 }, // TOP
		{ 2, 9, 11, 9, 2,  1 }  // BOTTOM
	};

	VECTOR_INIT(chunkVertices);
	VECTOR_INIT(chunkIndices);
	VECTOR_INIT(chunkColors);
	VECTOR_INIT(chunkUVs);
	int indiceIndex = 0;
	int x = 10;
	int y = 0;
	int z = 0;


	int j = 0;
	for(int i = 0; i < 6; i++)
	{
		float val0 = vertices[i + j][0] + 1.0f;
		float val1 = vertices[i + j][1] + 1.0f;		
		float val2 = vertices[i + j][2] + 1.0f;		
		float val3 = vertices[i + j + 1][0] + 1.0f;
		float val4 = vertices[i + j + 1][1] + 1.0f;
		float val5 = vertices[i + j + 1][2] + 1.0f;

		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j][0]);
		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j][1]);
		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j][2]);
		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j + 1][0]);
		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j + 1][1]);
		chunkVertices.pfVectorAdd(&chunkVertices, &vertices[i + j + 1][2]);

		float r = 1.0f;
		float g = 1.0f;
		float b = 1.0f;

		chunkColors.pfVectorAdd(&chunkColors, &r);
		chunkColors.pfVectorAdd(&chunkColors, &g);
		chunkColors.pfVectorAdd(&chunkColors, &b);

		chunkColors.pfVectorAdd(&chunkColors, &r);
		chunkColors.pfVectorAdd(&chunkColors, &g);
		chunkColors.pfVectorAdd(&chunkColors, &b);

		chunkUVs.pfVectorAdd(&chunkUVs, &uvs[i + j][0]);
		chunkUVs.pfVectorAdd(&chunkUVs, &uvs[i + j][1]);

		chunkUVs.pfVectorAdd(&chunkUVs, &uvs[i + j + 1][0]);
		chunkUVs.pfVectorAdd(&chunkUVs, &uvs[i + j + 1][1]);

		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][0] + indiceIndex);
		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][1] + indiceIndex);
		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][2] + indiceIndex);
		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][3] + indiceIndex);
		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][4] + indiceIndex);
		chunkIndices.pfVectorAdd(&chunkIndices, &indices[i][5] + indiceIndex);
		indiceIndex += 12;
		j += 1;
	}

	for (int i = 0; i < chunkVertices.pfVectorTotal(&chunkVertices); i++)
    {
        printf("%f\n", (float)((float *)(chunkVertices.vectorList.items))[100]);
    }

	printf("%f\n", (float)chunkVertices.vectorList.total);

	unsigned int VBO, VAO, EBO, CBO, UVBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenBuffers(1, &CBO);
	glGenBuffers(1, &UVBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, chunkVertices.pfVectorTotal(&chunkVertices) * sizeof(float), (float *)chunkVertices.vectorList.items[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunkIndices.pfVectorTotal(&chunkIndices) * sizeof(float), (float *)chunkIndices.vectorList.items[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glBufferData(GL_ARRAY_BUFFER, chunkColors.pfVectorTotal(&chunkColors) * sizeof(float), (float *)chunkColors.vectorList.items [0], GL_STATIC_DRAW);
	
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, chunkUVs.pfVectorTotal(&chunkUVs) * sizeof(float), (float *)chunkUVs.vectorList.items[0], GL_STATIC_DRAW);

	// position attribute
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glBindBuffer(GL_ARRAY_BUFFER, CBO);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(2);

	SetShaderInteger(&shader, "texture1", 0, true);
	SetShaderMatrix4(&shader, "projection", camera.viewProjection.projection, false);

	while(!glfwWindowShouldClose(window))
	{
		currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		ProcessInput();
		glfwPollEvents();

		glClearColor(0.25f, 0.25f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		UpdateCameraVectors(&camera, &shader);

		mat4s model = { 1.0f, 0.0f, 0.0f, 0.0f,
                        0.0f, 1.0f, 0.0f, 0.0f,
                        0.0f, 0.0f, 1.0f, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f };

		model = glms_translate(model, (vec3s){ 0.0f, 0.0f, 0.0f });
		SetShaderMatrix4(&shader, "model", model, false);

		BindTexture(&texture);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, chunkIndices.pfVectorTotal(&chunkIndices), GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteBuffers(1, &CBO);
	glDeleteBuffers(1, &UVBO);

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