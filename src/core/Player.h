#pragma once
#include "Entity.h"
#include "../renderer/Renderer.h"
#include "Inventory.h"
#include <string>

class World;
// forgot to polymorph player functions under entity functions and implement weappons for entitys so now attack system so fucked
class Player : public Entity {
	
private:
	inline static Player* m_currentPlayer;
	float m_mana = 10;
	int m_level = 0;
	float m_baseMaxHealth = 100;
	float m_maxHealth;
	const Mesh* m_leftsidemesh;
	const Mesh* m_rightsidemesh;
	const Mesh* m_backmesh;
	const Mesh* m_frontmesh;
	Vector2 m_mousePos;
	Vector2 m_mouseLookVector;
	Vector2 m_centerPosition;
	inline static bool m_breakingBlock  = false;
	inline static bool m_Attacking = true;
	inline static bool m_inventoryOpen = false;
	Inventory inventory;
	float m_attackTimer = 0;
	float m_secondstoHeal = 1;
	float m_healTimer = 0;
public:
	
	Player();
	void attack(World& world) override;  
	void updatePlayer(World &world,QuadRenderer &quadrend,Shader &s,Interface& interface);
	int getPlayerLevel() { return m_level; }
	void setPlayerLevel(int l) { m_level = l; }
	static Player* getCurrentPlayer() { return m_currentPlayer; }
	Inventory& getInv() { return inventory; }
	const Vector2& getLookVector() override { return m_mouseLookVector; }
private:
	void isInteracting(World& world, QuadRenderer& quadrend, Shader& s);
	static void handlePressInput(int key);
	static void handleReleaseInput(int key);

};