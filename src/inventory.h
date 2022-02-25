#ifndef INVENTORY_H
#define INVENTORY_H

#include <cglm/struct.h>
#include "utils/texture.h"

typedef struct Item
{
	uint8_t ID, amount;
	struct Texture2D texture;
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

#endif