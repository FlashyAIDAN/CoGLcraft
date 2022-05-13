#include "inventory.h"

uint8_t CheckForEmptySlot(Slot* slots, uint8_t size)
{
    for(uint8_t i = 0; i < size; i++)
        if(slots[i].empty)
            return i;

    return 255;
}

bool AddItemToInventory(Slot* slots, uint8_t size, Item item)
{
    for(uint8_t i = 0; i < size; i++)
    {
        if(slots[i].itemStack.item.ID == item.ID && slots[i].itemStack.item.amount <= slots[i].itemStack.maxAmount-1)
        {
            slots[i].itemStack.item.amount++;
            return true;
        }
    }
    
    uint8_t slot = CheckForEmptySlot(slots, size);

    if(slot != 255)
    {
        slots[slot].itemStack = (ItemStack){item, 3};
        slots[slot].empty = false;
        return true;
    }
    
    return false;
}

Slot DecreaseItemInIneventory(Slot slot)
{
    slot.itemStack.item.amount--;

    if(slot.itemStack.item.amount<=0)
        slot = RemoveItemInInventory(slot);
    
    return slot;
}

Slot RemoveItemInInventory(Slot slot)
{
    slot.itemStack.item.amount = 0;
    slot.itemStack.item.ID = 0;
    slot.itemStack.item.texture = (struct Texture2D){0};
    slot.itemStack.item.type = 0;
    slot.itemStack.maxAmount = 0;
    slot.empty = true;

    return slot;
}

void DeleteInventory(Slot* slots, uint8_t size)
{
    SaveInventory(slots, size);
}

void SaveInventory(Slot* slots, uint8_t size)
{
    FILE *inf;
	inf = fopen ("res/saves/main/player/inventory.data", "w");
	if (inf == NULL)
        printf("Cant open file!");
    
    uint8_t numItems = 0;
    for(int i = 0; i < size; i++)
        if(!slots[i].empty)
            numItems++;
    fprintf(inf, "Num Items: %i\n", numItems);

    for(int i = 0; i < size; i++)
        if(!slots[i].empty)
            fprintf(inf, "Slot: %i\nID: %i\nAmount: %i\nType: %i\n", i, slots[i].itemStack.item.ID, slots[i].itemStack.item.amount, slots[i].itemStack.item.type);

	fclose(inf);
}

void LoadInventory(Slot* slots, uint8_t size)
{
    char *str = ReadFileToString("res/saves/main/player/inventory.data");
	const char *s1 = "Slot: ";
	const char *s2 = "ID: ";
	const char *s3 = "Amount: ";
	const char *s4 = "Type: ";
    const char *s5 = "Num Items: ";
    char word[100];

    sscanf(strstr(str, s5) + strlen(s5), "%99s", word);
    int num = (int)strtol(word, (char **)NULL, 10);
    int numDigits = (num == 0) ? 1 : (log10(num) + 1);
    str += 13 + numDigits; // \n counts as 2

    for(int s = 0; s < num; s++)
    {
        sscanf(strstr(str, s1) + strlen(s1), "%99s", word);
        int i = (int)strtol(word, (char **)NULL, 10);
        int slotNumDigits = (i == 0) ? 1 : (log10(i) + 1);
        sscanf(strstr(str, s2) + strlen(s2), "%99s", word);
        slots[i].itemStack.item.ID = (int)strtol(word, (char **)NULL, 10);
        int idDigits = (slots[i].itemStack.item.ID == 0) ? 1 : (log10(slots[i].itemStack.item.ID) + 1);
        sscanf(strstr(str, s3) + strlen(s3), "%99s", word);
        slots[i].itemStack.item.amount = (int)strtol(word, (char **)NULL, 10);
        int amountDigits = (slots[i].itemStack.item.amount == 0) ? 1 : (log10(slots[i].itemStack.item.amount) + 1);
        sscanf(strstr(str, s4) + strlen(s4), "%99s", word);
        slots[i].itemStack.item.type = (int)strtol(word, (char **)NULL, 10);
        int typeDigits = (slots[i].itemStack.item.type == 0) ? 1 : (log10(slots[i].itemStack.item.type) + 1);
        slots[i].empty = false;
        str += 32 + slotNumDigits + idDigits + amountDigits + typeDigits;
    }
}