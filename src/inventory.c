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
    slot.itemStack.maxAmount = 0;
    slot.empty = true;

    return slot;
}