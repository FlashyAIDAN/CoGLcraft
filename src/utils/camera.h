#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <cglm/struct.h>

#include "shader.h"

#include <stdbool.h>

struct ViewProjection
{
    mat4s projection, view;
};

struct Camera
{
    struct ViewProjection viewProjection;
    vec3s position, front, forward, up, right, worldUp;
    float yaw, pitch, speed, sensitivity, fov, nearZ, farZ;
};

void UpdateCameraVectors(struct Camera *camera, struct Shader *shader);

struct Camera MakeCamera(struct Shader *shader, vec3s position, float yaw, float pitch, unsigned int width, unsigned int height);

void UpdateCameraPerspective(struct Camera *camera, struct Shader *shader, unsigned int width, unsigned int height);

#endif