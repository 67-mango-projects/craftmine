#pragma once
#include "../renderer/QuadRenderer.h"
#include "Inventory.h"
#define ORE_SIZE 20
#define ORE_DENSITY 0.05 // 0.2 percent of tiels are ore
class Interactable {
public:
	inline static std::vector<const Interactable*> g_Claimables;
	inline static std::vector<const Interactable*> g_Walls;

	std::string name;
	float chance;
	const Mesh* mesh;
	Item* item;
	Vector2 position;
	Vector2 chunkPosition;
	

	int droprate = 1;
public:
	static void initializeInteractables();
};


