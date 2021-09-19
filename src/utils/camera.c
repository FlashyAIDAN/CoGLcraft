#include "camera.h"

void UpdateCameraVectors(struct Camera *camera, struct Shader *shader)
{
    vec3s front;
    front.x = cos(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    front.y = sin(glm_rad(camera->pitch));
    front.z = sin(glm_rad(camera->yaw)) * cos(glm_rad(camera->pitch));
    camera->front = glms_normalize(front);
    camera->right = glms_normalize(glms_cross(camera->front, camera->worldUp));
    camera->up = glms_normalize(glms_cross(camera->right, camera->front));
    camera->forward = glms_cross(camera->worldUp, camera->right);

    camera->viewProjection.view = glms_lookat(camera->position, glms_vec3_add(camera->position, camera->front), camera->up);
    SetShaderMatrix4(shader, "view", camera->viewProjection.view, true);
}

struct Camera MakeCamera(struct Shader *shader, vec3s position, float yaw, float pitch, unsigned int width, unsigned int height)
{
    struct ViewProjection viewProjection;
    viewProjection.projection = (mat4s){ 1.0f, 0.0f, 0.0f, 0.0f,
                                         0.0f, 1.0f, 0.0f, 0.0f,
                                         0.0f, 0.0f, 1.0f, 0.0f,
                                         0.0f, 0.0f, 0.0f, 1.0f };
    viewProjection.view = (mat4s){ 1.0f, 0.0f, 0.0f, 0.0f,
                                   0.0f, 1.0f, 0.0f, 0.0f,
                                   0.0f, 0.0f, 1.0f, 0.0f,
                                   0.0f, 0.0f, 0.0f, 1.0f };
    

    struct Camera camera;
    camera.viewProjection = viewProjection;
    camera.worldUp = (vec3s){0.0f, 1.0f, 0.0f};
    camera.yaw = -90.0f;
    camera.pitch = 0.0f;
    camera.speed = 5.0f;
    camera.sensitivity = 0.1f;
    camera.fov = 90.0f;
    camera.position = position;
    camera.yaw = yaw;
    camera.pitch = pitch;
    camera.nearZ = 0.1f;
    camera.farZ = 1000.0f;

    UpdateCameraPerspective(&camera, shader, width, height);

    UpdateCameraVectors(&camera, shader);

    return camera;
}

void UpdateCameraPerspective(struct Camera *camera, struct Shader *shader, unsigned int width, unsigned int height)
{
    camera->viewProjection.projection = glms_perspective(glm_rad(camera->fov), (float)width / (float)height, camera->nearZ, camera->farZ);
    SetShaderMatrix4(shader, "projection", camera->viewProjection.projection, true);
};