#include "Interactables.h"
#include<spdlog.h>
void Interactable::initializeInteractables() {
	if (!Inventory::ITEMLIST.Wood)
		spdlog::error("Tried to create Interactable with null item (Wood)");
    g_Claimables.push_back(new Interactable({
        "wood",
        0.50f, 
        QuadRenderer::getMesh("wood.png"),
        Inventory::ITEMLIST.Wood
        }));

    g_Claimables.push_back(new Interactable({
        "stone",
        0.55f, 
        QuadRenderer::getMesh("stoneore.png"),
        Inventory::ITEMLIST.Stone
        }));

    g_Claimables.push_back(new Interactable({
        "iron",
        0.15f, 
        QuadRenderer::getMesh("ironore.png"),
        Inventory::ITEMLIST.Iron
        }));

    g_Claimables.push_back(new Interactable({
        "diamond",
        0.045f, 
        QuadRenderer::getMesh("diamondore.png"),
        Inventory::ITEMLIST.Diamond
        }));

    g_Claimables.push_back(new Interactable({
        "uranium",
        0.025f, 
        QuadRenderer::getMesh("uraniumore.png"),
        Inventory::ITEMLIST.Uranium
        }));

    g_Claimables.push_back(new Interactable({
        "crimson",
        0.010f, 
        QuadRenderer::getMesh("crimsonore.png"),
        Inventory::ITEMLIST.Crimson
        }));

    g_Claimables.push_back(new Interactable({
        "solarium",
        0.005f,
        QuadRenderer::getMesh("solariumore.png"),
        Inventory::ITEMLIST.Solarium
        }));

    g_Claimables.push_back(new Interactable({
        "ender",
        0.002f, 
        QuadRenderer::getMesh("enderore.png"),
        Inventory::ITEMLIST.Enderium
        }));

	g_Walls.push_back(new Interactable({ "woodwall", 1.0f, QuadRenderer::getMesh("woodwall.png"),Inventory::ITEMLIST.Wood }));
	g_Walls.push_back(new Interactable({ "stonewall",    1.0f, QuadRenderer::getMesh("stonewall.png"),Inventory::ITEMLIST.Stone }));
	
}