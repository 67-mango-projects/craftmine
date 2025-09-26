#pragma once
#include "Entity.h"
#include "../renderer/Renderer.h"
#include <string>
class Player : public Entity {
	
private:
	inline static Player* m_currentPlayer;
	int m_level;
	Mesh* m_sidemesh;
	Mesh* m_frontmesh;
public:
	Player();
	void renderPlayer();
	void updatePlayer();
private:
	static void handlePressInput(int key);
	static void handleReleaseInput(int key);

};