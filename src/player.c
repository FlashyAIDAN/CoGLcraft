#include "player.h"
#include "world.h"

struct Player MakePlayer(struct Shader shader, unsigned int screenWidth, unsigned int screenHeight)
{
    struct Player player;

    player.horizontal = 0.0f;
	player.vertical = 0.0f;
	player.mouseHorizontal = 0.0f;
	player.mouseVertical = 0.0f;
	player.velocity = (vec3s){0.0f, 0.0f, 0.0f};
	player.verticalMomentum = 0.0f;
	player.jumpRequest = false;

    player.position = (vec3s){0.0f, 0.0f, 0.0f};
    player.shader = shader;
	player.camera = MakeCamera(&player.shader, (vec3s){0.0f, 0.0f, 0.0f}, -90.0f, 0.0f, screenWidth, screenHeight);

	player.gravity = -9.8f;
	player.speed = 3.0f;
	player.jumpForce = 5.0f;
	player.sensitivity = 0.3f;
	player.isGrounded = false;
	player.isSprinting = false;

	player.checkIncrement = 0.1f;
	player.reach = 8.0f;

	player.width = 0.15f;
	player.height = 1.8f;

    SetShaderInteger(&player.shader, "texture1", 0, true);
	SetShaderMatrix4(&player.shader, "projection", player.camera.viewProjection.projection, false);

    return player;
}

void DeletePlayer(struct Player *player)
{
    SavePlayer(player);
}

void LoadPlayer(struct Player *player)
{
    char *str = ReadFile("res/saves/main/player/player.data");
	const char *s1 = "Position.x = ";
	const char *s2 = "Position.y = ";
	const char *s3 = "Position.z = ";
	const char *s4 = "Yaw = ";
	const char *s5 = "Pitch = ";
    char word[100];

    sscanf(strstr(str, s1) + strlen(s1), "%99s", word);
	player->position.x = strtof(word, NULL);
    sscanf(strstr(str, s2) + strlen(s2), "%99s", word);
	player->position.y = strtof(word, NULL);
    sscanf(strstr(str, s3) + strlen(s3), "%99s", word);
	player->position.z = strtof(word, NULL);
    sscanf(strstr(str, s4) + strlen(s4), "%99s", word);
	player->camera.yaw = strtof(word, NULL);
    sscanf(strstr(str, s5) + strlen(s5), "%99s", word);
	player->camera.pitch = strtof(word, NULL);
}

void SavePlayer(struct Player *player)
{
    FILE *inf;
	inf = fopen ("res/saves/main/player/player.data", "w");
	if (inf == NULL)
        printf("Cant open file!");
    
    fprintf(inf, "Position.x = %f\nPosition.y = %f\nPosition.z = %f\nYaw = %f\nPitch = %f", player->position.x, player->position.y, player->position.z, player->camera.yaw, player->camera.pitch);

	fclose(inf);
}

void PlayerUpdate(struct Player *player, float deltaTime)
{
    CalculateVelocity(player, deltaTime);
    if (player->jumpRequest)
        PlayerJump(player);

    player->position = glms_vec3_add(player->position, player->velocity);

    player->camera.position = (vec3s){player->position.x, player->position.y + player->height, player->position.z};

    UpdateCameraVectors(&player->camera, &player->shader);
}

void PlayerJump(struct Player *player)
{
    player->verticalMomentum = player->jumpForce;
    player->isGrounded = false;
    player->jumpRequest = false;
}

void CalculateVelocity(struct Player *player, float deltaTime)
{
    // Affect vertical momentum with gravity
    if (player->verticalMomentum > player->gravity) // TODO: players vertical momentum is way bigger if they havent jumped yet
        player->verticalMomentum += deltaTime * player->gravity;

    // If we're sprinting, use the sprint multiplier.
    if (player->isSprinting)
        player->velocity = glms_vec3_mul(glms_vec3_add(glms_vec3_mul(player->camera.forward, (vec3s){player->vertical, player->vertical, player->vertical}), glms_vec3_mul(player->camera.right, (vec3s){player->horizontal, player->horizontal, player->horizontal})), (vec3s){deltaTime * player->speed * 2.0f, deltaTime * player->speed * 2.0f, deltaTime * player->speed * 2.0f});
    else
        player->velocity = glms_vec3_mul(glms_vec3_add(glms_vec3_mul(player->camera.forward, (vec3s){player->vertical, player->vertical, player->vertical}), glms_vec3_mul(player->camera.right, (vec3s){player->horizontal, player->horizontal, player->horizontal})), (vec3s){deltaTime * player->speed, deltaTime * player->speed, deltaTime * player->speed});

    // Apply vertical momentum
    player->velocity = glms_vec3_add(player->velocity, glms_vec3_mul(player->camera.worldUp, (vec3s){player->verticalMomentum * deltaTime, player->verticalMomentum * deltaTime, player->verticalMomentum * deltaTime}));

    if ((player->velocity.z > 0 && PlayerCheckFront(player)) || (player->velocity.z < 0 && PlayerCheckBack(player)))
        player->velocity.z = 0;
    if ((player->velocity.x > 0 && PlayerCheckRight(player)) || (player->velocity.x < 0 && PlayerCheckLeft(player)))
        player->velocity.x = 0;

    if (player->velocity.y < 0)
        player->velocity.y = CheckDownSpeed(player, player->velocity.y);
    if (player->velocity.y > 0)
        player->velocity.y = CheckUpSpeed(player, player->velocity.y);
}

float CheckDownSpeed(struct Player *player, float downSpeed)
{
    if (
        (voxels[WorldGetVoxel(player->position.x - player->width, player->position.y + downSpeed, player->position.z - player->width)].collider && (!PlayerCheckLeft(player) && !PlayerCheckBack(player))) ||
        (voxels[WorldGetVoxel(player->position.x + player->width, player->position.y + downSpeed, player->position.z - player->width)].collider && (!PlayerCheckRight(player) && !PlayerCheckBack(player))) ||
        (voxels[WorldGetVoxel(player->position.x + player->width, player->position.y + downSpeed, player->position.z + player->width)].collider && (!PlayerCheckRight(player) && !PlayerCheckFront(player))) ||
        (voxels[WorldGetVoxel(player->position.x - player->width, player->position.y + downSpeed, player->position.z + player->width)].collider && (!PlayerCheckLeft(player) && !PlayerCheckFront(player)))
        )
    {
        player->isGrounded = true;
        return 0.0f;
    }
    else
    {
        player->isGrounded = false;
        return downSpeed;
    }
}

float CheckUpSpeed(struct Player *player, float upSpeed)
{
    float checkOffset = 0.1f;
    if (
        (voxels[WorldGetVoxel(player->position.x - player->width, player->position.y + player->height + checkOffset + upSpeed, player->position.z - player->width)].collider && (!PlayerCheckLeft(player) && !PlayerCheckBack(player))) ||
        (voxels[WorldGetVoxel(player->position.x + player->width, player->position.y + player->height + checkOffset + upSpeed, player->position.z - player->width)].collider && (!PlayerCheckRight(player) && !PlayerCheckBack(player))) ||
        (voxels[WorldGetVoxel(player->position.x + player->width, player->position.y + player->height + checkOffset + upSpeed, player->position.z + player->width)].collider && (!PlayerCheckRight(player) && !PlayerCheckFront(player))) ||
        (voxels[WorldGetVoxel(player->position.x - player->width, player->position.y + player->height + checkOffset + upSpeed, player->position.z + player->width)].collider && (!PlayerCheckLeft(player) && !PlayerCheckFront(player)))
        )
    {
        // set to 0 so the player falls when their head hits a block while jumping
        player->verticalMomentum = 0;
        return 0.0f;
    }
    else
        return upSpeed;
}

bool PlayerCheckFront(struct Player *player)
{
    if (
        voxels[WorldGetVoxel(player->position.x, player->position.y, player->position.z + player->width)].collider ||
        voxels[WorldGetVoxel(player->position.x, player->position.y + 1.0f, player->position.z + player->width)].collider
        )
        return true;
    else return false;
}

bool PlayerCheckBack(struct Player *player)
{
    if (
        voxels[WorldGetVoxel(player->position.x, player->position.y, player->position.z - player->width)].collider ||
        voxels[WorldGetVoxel(player->position.x, player->position.y + 1.0f, player->position.z - player->width)].collider
        )
        return true;
    else return false;
}

bool PlayerCheckLeft(struct Player *player)
{
    if (
        voxels[WorldGetVoxel(player->position.x - player->width, player->position.y, player->position.z)].collider ||
        voxels[WorldGetVoxel(player->position.x - player->width, player->position.y + 1.0f, player->position.z)].collider
        )
        return true;
    else return false;
}

bool PlayerCheckRight(struct Player *player)
{
    if (
        voxels[WorldGetVoxel(player->position.x + player->width, player->position.y, player->position.z)].collider ||
        voxels[WorldGetVoxel(player->position.x + player->width, player->position.y + 1.0f, player->position.z)].collider
        )
        return true;
    else return false;
}