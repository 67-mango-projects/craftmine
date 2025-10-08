#pragma once
#include<string>
#include <array>
#include <vector>

#include "../renderer/Interface.h"
#include "Weapon.h"

enum class ITEMID {
    WOOD,
    STONE,
    IRON,
    DIAMOND,
    URANIUM,
    CRIMSON,
    SOLARIUM,
    ENDERIUM,
    WOODSWORD,
    STONESWORD,
    RAPIER,
    DIAMONDSWORD,
    URANIUMSWORD,
    CRIMSONSWORD,
    SINSCYTHE,
    ELECTRICSWORD,
    HELLHARBINGER,
    ENDSWORD,
    TERMINATOR,
    CAlAMITYSWORD,
    HYPERION,
    ENDERWARP,
    NETHERWARP
};

struct ItemStats {
    bool isWeapon = false;
    bool isTool = false;
    bool isMaterial;
    bool isBlock = false;
};



struct Item {
    std::string name;
    ITEMID id;
    ItemStats stats;
    const Mesh* icon;
    Weapon* weapon;
};

struct ItemStack {
    Item* item;
    unsigned int count;

    ItemStack(Item* i = nullptr, unsigned int c = 0)
        : item(i), count(c) {
    }

    bool isEmpty() const { return item == nullptr || count == 0; }
};

struct Recipe {
    std::vector<std::pair<Item*, unsigned int>> ingredients;
    Item* result;
    unsigned int resultCount;

    Recipe(std::initializer_list<std::pair<Item*, unsigned int>> ingr,
        Item* res, unsigned int resCount = 1)
        : ingredients(ingr), result(res), resultCount(resCount) {
    }
};

struct ItemList {
    Item* Wood;
    Item* Stone;
    Item* Iron;
    Item* Diamond;
    Item* Uranium;
    Item* Crimson;
    Item* Solarium;
    Item* Enderium;
    Item* WoodSword;
    Item* StoneSword;
    Item* Rapier;
    Item* UraniumSword;
    Item* DiamondSword;
    Item* CrimsonSword;
    Item* ElectricSword;
    Item* HellHarbinger;
    Item* EndSword;
    Item* Terminator;
    Item* SinScythe;
    Item* CalamitySword;
    Item* Hyperion;
    Item* NetherWarp;
    Item* EnderWarp;
};

struct EntityWeaponList {
    Weapon* zombieslash;
    Weapon* skeletonarrows;
    Weapon* wizardslashprojectile;
    Weapon* blazefireball;
    Weapon* aarush;
    Weapon* undeadBossBarrage;
    Weapon* witherBossInfernoBlast;
    Weapon* witherBossHeadBall;
    Weapon* EnderDragonBall;
};

class Inventory {
private:
    static const int MAX_SLOTS = 20;
    static const int MAX_STACK = 64;
    inline static std::unordered_map<ITEMID, Recipe*> m_recipes;

    std::array<ItemStack, MAX_SLOTS> m_slots;
    

    Item* equippedItem = nullptr;
    int equippedSlot = 0;
public:                             
    Inventory() {}
    static void initializeItems();

    bool addItem(Item* item, unsigned int count = 1);
    bool removeItem(Item* item, unsigned int count = 1);
    const std::array<ItemStack, MAX_SLOTS>& getSlots() const { return m_slots; }
    
    bool craft(const Recipe& recipe);
    bool canCraft(const Recipe& recipe) const;

    bool equipItem(int slotIndex);
    void unequipItem();
    Item* getEquippedItem() const { return equippedItem; }
    bool hasEquippedItem() const { return equippedItem != nullptr; }
    void renderInventory(Interface &interface,Shader &s, QuadRenderer* quadrender);

    ItemStack getItemFromSlot(size_t index);
    inline static ItemList ITEMLIST;
    inline static EntityWeaponList ENTWEAPLIST;
};

