#ifndef INVENTORY_H
#define INVENTORY_H

#include <cglm/struct.h>
#include "utils/texture.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils/file.h"

typedef enum ItemType
{
	Block,
	Material,
	Weapon,
	Pickaxe,
	Axe,
	Shovel,
	Shield,
	Consumable,
	Armor
} ItemType;

typedef struct Item
{
	uint8_t ID, amount;
	struct Texture2D texture;
	ItemType type;
} Item;
typedef struct ItemStack
{
	Item item;
	uint8_t maxAmount;
} ItemStack;

typedef struct Slot
{
	ItemStack itemStack;
	vec2s position;
	bool empty;
} Slot;

uint8_t CheckForEmptySlot(Slot* slots, uint8_t size);

bool AddItemToInventory(Slot* slots, uint8_t size, Item item);

Slot DecreaseItemInIneventory(Slot slot);
Slot RemoveItemInInventory(Slot slot);

void DeleteInventory(Slot* slots, uint8_t size);
void SaveInventory(Slot* slots, uint8_t size);
void LoadInventory(Slot* slots, uint8_t size);

#endif