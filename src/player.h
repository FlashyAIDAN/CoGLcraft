#ifndef PLAYER_H
#define PLAYER_H

#include "utils/camera.h"
#include "utils/shader.h"

#include <cglm/struct.h>

#include <stdbool.h>

struct Player
{
    float horizontal;
	float vertical;
	float friction;
	float airDrag;
	float acceleration;
	float maxFallSpeed;
	float mouseHorizontal;
	float mouseVertical;
	vec3s velocity;
	float verticalMomentum;
	bool jumpRequest;

    vec3s position;
    struct Shader shader;
	struct Camera camera;

	float gravity; //-9.8f;
	float speed; // 3.0f;
	float jumpForce; // 5.0f;
	float sensitivity; // 0.3f;
	bool isGrounded;
	bool isSprinting;
	bool inInventory;

	float checkIncrement; // 0.1f;
	float reach; // 8.0f;

	float width; // 0.15f;
	float height; // 1.8f;
};
	struct Player MakePlayer(struct Shader shader, unsigned int screenWidth, unsigned int screenHeight);
	void DeletePlayer(struct Player *player);
	void LoadPlayer(struct Player *player);
	void SavePlayer(struct Player *player);
	void PlayerUpdate(struct Player *player, float deltaTime);
	void PlayerJump(struct Player *player);
	void CalculateVelocity(struct Player *player, float deltaTime);
	float CheckDownSpeed(struct Player *player, float downSpeed);
	float CheckUpSpeed(struct Player *player, float upSpeed);
	bool PlayerCheckFront(struct Player *player);
	bool PlayerCheckBack(struct Player *player);
	bool PlayerCheckLeft(struct Player *player);
	bool PlayerCheckRight(struct Player *player);

#endif