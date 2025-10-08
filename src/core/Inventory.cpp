#include "Inventory.h"
#include <spdlog.h>
#include "../game.h"

bool Inventory::addItem(Item* item, unsigned int count) {
    spdlog::debug("added item :{}", item->name);
    for (auto& slot : m_slots) {
        if (slot.item && slot.item->id == item->id && slot.count < MAX_STACK) {
            unsigned int space = MAX_STACK - slot.count;
            unsigned int addAmount = std::min(count, space);
            slot.count += addAmount;
            count -= addAmount;
            if (count == 0) return true;
        }
    }

    for (auto& slot : m_slots) {
        if (slot.isEmpty()) {
            unsigned int addAmount = std::min(count, (unsigned int)MAX_STACK);
            slot.item = item;
            slot.count = addAmount;
            count -= addAmount;
            if (count == 0) return true;
        }
    }

    return false;
}

bool Inventory::removeItem(Item* item, unsigned int count) {
    for (auto& slot : m_slots) {
        if (slot.item && slot.item->id == item->id) {
            if (slot.count > count) {
                slot.count -= count;
                return true;
            }
            else {
                count -= slot.count;
                slot.item = nullptr;
                slot.count = 0;
                if (count == 0) { 
                    if (equippedItem == item)
                        equippedItem = nullptr;
                    return true;
                }
            }
        }
    }
    return false;
}


void Inventory::initializeItems() { // annoying ass boiler plate code which took me like 2 years to refactor

    // --- BLOCKS / MATERIALS ---
    ITEMLIST.Wood = new Item{
        "Wood", ITEMID::WOOD,
        { false, false, true, true },
        QuadRenderer::getMesh("wood.png"),
        nullptr
    };

    ITEMLIST.Stone = new Item{
        "Stone", ITEMID::STONE,
        { false, false, true, true },
        QuadRenderer::getMesh("stoneore.png"),
        nullptr
    };

    ITEMLIST.Iron = new Item{
        "Iron", ITEMID::IRON,
        { false, false, true, false },
        QuadRenderer::getMesh("ironore.png"),
        nullptr
    };

    ITEMLIST.Diamond = new Item{
        "Diamond", ITEMID::DIAMOND,
        { false, false, true, false },
        QuadRenderer::getMesh("diamondore.png"),
        nullptr
    };

    ITEMLIST.Uranium = new Item{
        "Uranium", ITEMID::URANIUM,
        { false, false, true, false },
        QuadRenderer::getMesh("uraniumore.png"),
        nullptr
    };

    ITEMLIST.Crimson = new Item{
        "Crimson", ITEMID::CRIMSON,
        { false, false, true, false },
        QuadRenderer::getMesh("crimsonore.png"),
        nullptr
    };

    ITEMLIST.Solarium = new Item{
        "Solarium", ITEMID::SOLARIUM,
        { false, false, true, false },
        QuadRenderer::getMesh("solariumore.png"),
        nullptr
    };

    ITEMLIST.Enderium = new Item{
        "Enderium", ITEMID::ENDERIUM,
        { false, false, true, false },
        QuadRenderer::getMesh("enderore.png"),
        nullptr
    };

    // --- WEAPONS ---
    ITEMLIST.WoodSword = new Item{
        "Wood Sword", ITEMID::WOODSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("woodsword.png"),
        new MeleeWeapon(5, 0.45,30,Quad(QuadRenderer::getMesh("normalslash.png"),Vector2(0,0),Vector2(70,70),0.95))
    };

    ITEMLIST.StoneSword = new Item{
        "Stone Sword", ITEMID::STONESWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("stonesword.png"),
        new MeleeWeapon(15.0f,0.5f, 30,Quad(QuadRenderer::getMesh("normalslash.png"),Vector2(0,0),Vector2(70,70)))
    };

    ITEMLIST.Rapier = new Item{
        "Rapier", ITEMID::RAPIER,
        { true, false, false, false },
        QuadRenderer::getMesh("rapiersword.png"),
        new MeleeWeapon(11, 0.07f,50,Quad(QuadRenderer::getMesh("rapierslash.png"),Vector2(0,0),Vector2(120,90)))
    };

    ITEMLIST.DiamondSword = new Item{
        "Diamond Sword", ITEMID::DIAMONDSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("diamondsword.png"),
        new MeleeWeapon(70.0f, 0.4f,30,Quad(QuadRenderer::getMesh("normalslash.png"),Vector2(0,0),Vector2(130,130)))
    };

    ITEMLIST.UraniumSword = new Item{
        "Uranium Sword", ITEMID::URANIUMSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("uraniumsword.png"),
        new ProjectileWeapon(75.0f, 0.7f,600,Quad(QuadRenderer::getMesh("uraniumpellets.png"),Vector2(0,0),Vector2(90,90)),45,3)
    };

    ITEMLIST.CrimsonSword = new Item{
        "Crimson Sword", ITEMID::CRIMSONSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("crimsonsword.png"),
        new ProjectileWeapon(60.0f, 0.07f,800,Quad(QuadRenderer::getMesh("crimsonprojectile.png"),Vector2(0,0),Vector2(70,35)),15,2)
    };

    ITEMLIST.ElectricSword = new Item{
        "Electric Sword", ITEMID::ELECTRICSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("electrosword.png"),
        new ProjectileWeapon(800, 1.0f,600,Quad(QuadRenderer::getMesh("electricball.png"),Vector2(0,0),Vector2(300,300)))
    };

    ITEMLIST.HellHarbinger = new Item{
        "Hell Harbinger", ITEMID::HELLHARBINGER,
        { true, false, false, false },
        QuadRenderer::getMesh("hellharbingersword.png"),
        new MeleeWeapon(10000, 0.3f,500,Quad(QuadRenderer::getMesh("infernoblast.png"),Vector2(0,0),Vector2(1000,250)))
    };

    ITEMLIST.EndSword = new Item{
        "End Sword", ITEMID::ENDSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("endsword.png"),
        new MeleeWeapon(2000, 0.05f,100,Quad(QuadRenderer::getMesh("endslash.png"),Vector2(0,0),Vector2(700,100)),100,2,0.4)
    };

    ITEMLIST.SinScythe = new Item{
        "Sin Scythe", ITEMID::SINSCYTHE,
        { true, false, false, false },
        QuadRenderer::getMesh("redscythe.png"),
        new ScytheWeapon(2000, 0.07f,8,Quad(QuadRenderer::getMesh("scytheslash.png"),Vector2(0,0),Vector2(400,200)),true,2,100)
    };
    ITEMLIST.CalamitySword = new Item{
        "Calamity Sword", ITEMID::CAlAMITYSWORD,
        { true, false, false, false },
        QuadRenderer::getMesh("calamitysword.png"),
        new ProjectileWeapon(3000, 0.09f,1000,Quad(QuadRenderer::getMesh("calamityprojectile.png"),Vector2(0,0),Vector2(300,200)),30,2)
    };
    ITEMLIST.Terminator = new Item{
        "Hyperion", ITEMID::TERMINATOR,
        { true, false, false, false },
        QuadRenderer::getMesh("terminator.png"),
        new ProjectileWeapon(200, 0.05f,900,Quad(QuadRenderer::getMesh("terminatorarrow.png"),Vector2(0,0),Vector2(80,80)),45,20)
    };
    ITEMLIST.Hyperion = new Item{
        "Hyperion", ITEMID::HYPERION,
        { true, false, false, false },
        QuadRenderer::getMesh("hyperionsword.png"),
        new MeleeWeapon(20000, 0.07f,0,Quad(QuadRenderer::getMesh("hyperionexplosion.png"),Vector2(0,0),Vector2(500,500)))
    };
    ITEMLIST.EnderWarp = new Item{
        "EnderWarp",ITEMID::ENDERWARP,
        { true, false, false, false },
        QuadRenderer::getMesh("endwarp.png"),
        nullptr

    };
    ITEMLIST.NetherWarp = new Item{
        "NetherWarp",ITEMID::NETHERWARP,
        { true, false, false, false },
        QuadRenderer::getMesh("netherwarp.png"),
        nullptr

    };

    ENTWEAPLIST.zombieslash = new MeleeWeapon(5, 0.45, 2, Quad(QuadRenderer::getMesh("normalslash.png"), Vector2(0, 0), Vector2(100, 100)));
    ENTWEAPLIST.skeletonarrows = new ProjectileWeapon(5.0f, 1, 300, Quad(QuadRenderer::getMesh("arrow.png"), Vector2(0, 0), Vector2(40, 40)), 15);
    ENTWEAPLIST.wizardslashprojectile = new ProjectileWeapon(10.0f, 1, 200, Quad(QuadRenderer::getMesh("normalslash.png"), Vector2(0, 0), Vector2(70, 70)), 5);
    ENTWEAPLIST.blazefireball = new ProjectileWeapon(20.0f,1, 200, Quad(QuadRenderer::getMesh("fireball.png"), Vector2(0, 0), Vector2(40, 40)), 45);
    ENTWEAPLIST.aarush = new MeleeWeapon(30.0f, 0.35, 0, Quad(QuadRenderer::getMesh("endslash.png"), Vector2(0, 0), Vector2(100, 100)));
    ENTWEAPLIST.witherBossHeadBall = new ProjectileWeapon(10, 0.3, 200, Quad(QuadRenderer::getMesh("witherprojectile.png"), Vector2(0, 0), Vector2(50, 50)),45,2);
    ENTWEAPLIST.EnderDragonBall = new ProjectileWeapon(150.0f, 2, 100, Quad(QuadRenderer::getMesh("endprojectile.png"), Vector2(0, 0), Vector2(400, 300)), 90,2);
    ENTWEAPLIST.undeadBossBarrage = new ProjectileWeapon(20.0f, 1, 600, Quad(QuadRenderer::getMesh("punchbarrage.png"), Vector2(0, 0), Vector2(400, 100)), 30);
    //-------------- RECIPES ------------------
    m_recipes.insert
    ({
        ITEMID::WOODSWORD,
        new Recipe(
            {
                { ITEMLIST.Wood, 10 }
            }, 
            ITEMLIST.WoodSword, 1
        )
   });

    m_recipes.insert
    ({
        ITEMID::STONESWORD,
        new Recipe(
            {
                { ITEMLIST.Wood, 5 },
                { ITEMLIST.Stone, 10 }
            },
            ITEMLIST.StoneSword, 1
        )
   });

    m_recipes.insert
    ({
        ITEMID::RAPIER,
        new Recipe(
            {
                { ITEMLIST.Iron, 15 },
            },
            ITEMLIST.Rapier, 1
        )
   });

    m_recipes.insert
    ({
        ITEMID::DIAMONDSWORD,
        new Recipe(
            {
                { ITEMLIST.Diamond, 5 },
            },
            ITEMLIST.DiamondSword, 1
        )
   });

    m_recipes.insert
    ({
        ITEMID::URANIUMSWORD,
        new Recipe(
            {
                { ITEMLIST.Uranium, 5 },
            },
            ITEMLIST.UraniumSword, 1
        )
    });

    m_recipes.insert
    ({
        ITEMID::CRIMSONSWORD,
        new Recipe(
            {
                { ITEMLIST.Crimson, 5 },
            },
            ITEMLIST.CrimsonSword, 1
        )
    });
    m_recipes.insert
    ({
        ITEMID::ELECTRICSWORD,
        new Recipe(
            {
                { ITEMLIST.Diamond, 3 },
                { ITEMLIST.Uranium, 3 },
            },
            ITEMLIST.ElectricSword, 1
        )
   });

    m_recipes.insert
    ({
        ITEMID::HELLHARBINGER,
        new Recipe(
            {
                { ITEMLIST.CrimsonSword, 1 },
                { ITEMLIST.Solarium, 10 },
            },
            ITEMLIST.HellHarbinger, 1
        )
    });

    m_recipes.insert
    ({
        ITEMID::ENDSWORD,
        new Recipe(
            {
                { ITEMLIST.Enderium, 10 },
                { ITEMLIST.Uranium, 5 },
            },
            ITEMLIST.EndSword, 1
        )
    });

    m_recipes.insert
    ({
        ITEMID::CAlAMITYSWORD,
        new Recipe(
            {
                { ITEMLIST.Solarium, 20 },
                { ITEMLIST.Uranium, 20 },
                { ITEMLIST.Enderium, 20 },
                { ITEMLIST.Crimson, 20 },
            },
            ITEMLIST.CalamitySword, 1
        )
   });
    m_recipes.insert
    ({
        ITEMID::SINSCYTHE,
        new Recipe(
            {
                { ITEMLIST.Crimson, 15 },
                { ITEMLIST.Solarium, 10 },
            },
            ITEMLIST.SinScythe, 1
        )
        });
    m_recipes.insert
    ({
        ITEMID::TERMINATOR,
        new Recipe(
            {
                { ITEMLIST.Enderium, 10 },
                { ITEMLIST.Crimson, 15 },
            },
            ITEMLIST.Terminator, 1
        )
        });

    m_recipes.insert
    ({
        ITEMID::HYPERION,
        new Recipe(
            {
                { ITEMLIST.CalamitySword, 1 },
                { ITEMLIST.EndSword, 1 },
                { ITEMLIST.HellHarbinger, 1 },
                { ITEMLIST.CrimsonSword, 1 },
            },
            ITEMLIST.Hyperion, 1
        )
        });

}


bool Inventory::equipItem(int slotIndex)
{
    if (slotIndex < 0 || slotIndex >= MAX_SLOTS)
        return false;

    ItemStack& stack = m_slots[slotIndex];
    if (stack.isEmpty())
        return false;

    equippedItem = stack.item;


    if (!(equippedItem->stats.isWeapon || equippedItem->stats.isBlock)) {
        equippedItem = nullptr;
        return false;
    }



    return true;
}

void Inventory::unequipItem()
{
    if (equippedItem)
        spdlog::info("Unequipped item: {}", equippedItem->name);

    equippedItem = nullptr;
}
ItemStack Inventory::getItemFromSlot(size_t index) {
    if (index >= m_slots.size()) {
        spdlog::warn("Attempted to get item from invalid slot {}", index);
        return nullptr;
    }
    return m_slots[index];
}

bool Inventory::canCraft(const Recipe& recipe) const
{
    for (const auto& [ingredient, neededCount] : recipe.ingredients)
    {
        unsigned int available = 0;
        for (const auto& slot : m_slots)
        {
            if (slot.item == ingredient)
                available += slot.count;
        }

        if (available < neededCount)
            return false;   
    }

    return true;
}

bool Inventory::craft(const Recipe& recipe)
{
    for (const auto& [ingredient, neededCount] : recipe.ingredients) {
        unsigned int available = 0;
        for (const auto& slot : m_slots) {
            if (slot.item == ingredient)
                available += slot.count;
        }
        if (available < neededCount)
            return false; 
    }

    auto ingredients = recipe.ingredients;

    for (auto& [ingredient, neededCount] : ingredients) {
        for (auto& slot : m_slots) {
            if (slot.item == ingredient && neededCount > 0) {
                unsigned int toRemove = std::min(slot.count, neededCount);
                slot.count -= toRemove;
                neededCount -= toRemove;

                if (slot.count == 0)
                    slot.item = nullptr;
            }
        }
    }

    addItem(recipe.result, recipe.resultCount);

    return true;
}

float cannotCraftTimerMessage = 0;

void Inventory::renderInventory(Interface& interface, Shader& s,QuadRenderer* quadrender) {
    
    Vector2 windowDimensions = interface.getWindowDimensions();
    Vector2 boxSize(850, 850);
    Vector2 windowCenter(windowDimensions.x * 0.5f, windowDimensions.y * 0.5f);
    Vector2 boxPos(windowCenter.x - boxSize.x * 0.5f,
    windowCenter.y - boxSize.y * 0.5f);
    
    interface.renderBox(boxPos, boxSize, Vector4::RBGAtoV4(28, 32, 38, 255),1.1);
    interface.renderText("Inventory", globalPixelFont, Vector2(100, windowCenter.y + boxSize.y * 0.5f - 50), 1, Vector4(1, 1, 1, 1), 1.2);
    
    Vector2 inventoryslotbackgroundsize(620, 700);
    interface.renderBox(Vector2(boxPos.x + 30, boxPos.y + 30), inventoryslotbackgroundsize, Vector4::RBGAtoV4(10, 10, 10, 255), 1.1);

    Vector2 craftslotbackgroundsize(150, 650);
    interface.renderBox(Vector2(boxPos.x + inventoryslotbackgroundsize.x + 50, boxPos.y + 30), craftslotbackgroundsize, Vector4::RBGAtoV4(10, 10, 10, 255), 1.1);
    interface.renderText("Crafting", globalPixelFont, Vector2(boxPos.x + inventoryslotbackgroundsize.x + 50, windowCenter.y + boxSize.y * 0.5f - 150), 0.7, Vector4(1, 1, 1, 1), 1.2);
    
    if (cannotCraftTimerMessage > 0.0f)
        cannotCraftTimerMessage -= Game::getDeltaTime(); 

    Vector2 buttonSize(120, 60);
    int rc = 0;
    for (auto& recipe : m_recipes) {
        if (interface.renderButton(std::format("{}",recipe.second->result->name), globalPixelFont, Vector2(boxPos.x + inventoryslotbackgroundsize.x + 60, boxPos.y + craftslotbackgroundsize.y - 40 - (rc * (buttonSize.y + 10))), buttonSize, Vector4::RBGAtoV4(30, 30, 30, 255),0.5,Vector2(0,20), 1.3)) {
            if (!craft(*recipe.second)) {
                cannotCraftTimerMessage = .7;
            }
        }
        rc++;
    }
    

    Vector2 slotSize(120, 120);
    
    int oldslot = equippedSlot;

    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 5; y++) {
            int currentSlot = (y * 4) + x;
            ItemStack currentItem = getItemFromSlot(currentSlot);

            if (currentSlot == equippedSlot) {
                interface.renderBox(Vector2(boxPos.x + 50 + (x * (slotSize.x + 10)), boxPos.y + 50 + (y * (slotSize.y + 10))), slotSize, Vector4::RBGAtoV4(120, 50, 50, 255), 1.1);
            }
            else {
                if (interface.renderButton("", globalPixelFont, Vector2(boxPos.x + 50 + (x * (slotSize.x + 10)), boxPos.y + 50 + (y * (slotSize.y + 10))), slotSize, Vector4::RBGAtoV4(50, 50, 50, 255), 0, Vector2(0, 0), 1.1)) {
                    equippedSlot = currentSlot;
                }
            }

            
            
            if (!currentItem.isEmpty()) {
                interface.renderImage(Vector2(boxPos.x + 50 + (x * (slotSize.x + 10)), boxPos.y + 50 + (y * (slotSize.y + 10))), slotSize, quadrender->worldtexture, new Mesh(*currentItem.item->icon), 1.2);
                interface.renderText(std::format("{}x", getItemFromSlot(currentSlot).count), globalPixelFont, Vector2(boxPos.x + 50 + (x * (slotSize.x + 10)), boxPos.y + 50 + (y * (slotSize.y + 10))), 0.7, Vector4(1, 1, 1, 1), 1.3);
            }
            
        }
    }

    if (!equipItem(equippedSlot)) {
        equippedSlot = oldslot;
        if (!equipItem(equippedSlot)) {
            equippedSlot = -1;
        }
    }

    if (cannotCraftTimerMessage > 0.0f) {
        interface.renderText("get more materials", globalPixelFont, Vector2(windowCenter.x - 400, windowCenter.y), 1.5, Vector4(1, 0, 0, 1), 1.4);
        interface.renderText("BROKE BOI!", globalPixelFont, Vector2(windowCenter.x + - 300, windowCenter.y - 100), 2, Vector4(1, 0, 0, 1), 1.4);
    }
}