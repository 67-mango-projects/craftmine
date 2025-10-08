//should have abstracted this further but too late now
#include "Player.h"
#include "Controller.h"
#include "../game.h"
#include <glm/gtc/matrix_transform.hpp>
#include <windows.h>
#include "World.h"
#include <iostream>
#include <spdlog.h>

#undef min
#undef max



Player::Player() : Entity(EntityType::PLAYER,100, 100,Quad(QuadRenderer::getMesh("playerfront.png"),Vector2(100,100),Vector2(50,50),0.1),nullptr) {
    m_baseMaxHealth = 100;
    m_leftsidemesh = QuadRenderer::getMesh("playerleft.png");
    m_rightsidemesh = QuadRenderer::getMesh("playerright.png");
	m_frontmesh = QuadRenderer::getMesh("playerfront.png");
    m_backmesh = QuadRenderer::getMesh("playerback.png");
	m_currentPlayer = this;

	Controller::bindPress(handlePressInput);
	Controller::bindRelease(handleReleaseInput);
}

void Player::updatePlayer(World& world, QuadRenderer& quadrend, Shader& s, Interface& inter) {
    double dt = Game::getDeltaTime();
    m_attackTimer -= dt;

    Vector2 movement = m_velocity * dt;  // movement this frame
    // Update heal timer
    m_healTimer -= dt;

    // Recalculate max health scaling
    m_maxHealth = m_baseMaxHealth * std::pow(1.1f, m_level);
    m_dmgMultiplier = 1 * std::pow(1.1f, m_level);
    // Passive regen
    if (m_healTimer <= 0.0f && m_health < m_maxHealth) {
        m_health = std::min(m_health + (m_maxHealth * 0.01f), m_maxHealth);
        m_healTimer = m_secondstoHeal; // reset timer
    }



    if (m_health < 0) {
        MessageBoxA(NULL, "YA DIED GG", "NOOB", 1);
        exit(1);
        
    }

    m_mouseLookVector = glm::normalize(glm::vec2(Controller::getMousePositionWorld() - m_model->position));
// --- Horizontal movement ---
    m_model->position.x += movement.x;

    bool hitX = false;
    for (auto& chunk : world.getLoadedChunks()) {
        for (const auto& tile : chunk->getInteractables()) {
            Quad q(tile.mesh, tile.position, Vector2(TILE_SIZE, TILE_SIZE));
            if (m_model->isCollidingWith_NoRotation(q)) {
                hitX = true;

                // compute half sizes
                float halfA = m_model->size.x * 0.5f;
                float halfB = q.size.x * 0.5f;

                if (m_velocity.x > 0)
                    m_model->position.x = q.position.x - (halfA + halfB);
                else if (m_velocity.x < 0)
                    m_model->position.x = q.position.x + (halfA + halfB);
            }
        }
    }
    if (hitX) movement.x = 0;

    // --- Vertical movement ---
    m_model->position.y += movement.y;

    bool hitY = false;
    for (auto& chunk : world.getLoadedChunks()) {
        for (const auto& tile : chunk->getInteractables()) {
            Quad q(tile.mesh, tile.position, Vector2(TILE_SIZE, TILE_SIZE));
            if (m_model->isCollidingWith_NoRotation(q)) {
                hitY = true;

                float halfA = m_model->size.y * 0.5f;
                float halfB = q.size.y * 0.5f;

                if (m_velocity.y > 0)
                    m_model->position.y = q.position.y - (halfA + halfB);
                else if (m_velocity.y < 0)
                    m_model->position.y = q.position.y + (halfA + halfB);
            }
        }
    }
    if (hitY) movement.y = 0;


    isInteracting(world, quadrend, s);

    if (m_Attacking) {
        attack(world);
    }

    if (m_inventoryOpen) {
        inventory.renderInventory(inter, s, &quadrend);
    }

    if (inventory.getEquippedItem() != nullptr) {
        Quad weapon(inventory.getEquippedItem()->icon, Vector2(m_model->position.x + 30, m_model->position.y + 10), Vector2(30, 30));
        quadrend.render(&weapon);
    }
    quadrend.render(m_model);
}
void Player::attack(World& world) {
    if (inventory.getEquippedItem()==nullptr)
        return;
    if (!inventory.getEquippedItem()->stats.isWeapon)
        return;
    if (m_attackTimer > 0.0f)
        return;
    
    if (inventory.getEquippedItem()->id == ITEMID::NETHERWARP) {
        world.setDimension(DIMENSION::NETHER);
        m_attackTimer = 1;
        return;
    }
    if (inventory.getEquippedItem()->id == ITEMID::ENDERWARP) {
        world.setDimension(DIMENSION::END);
        m_attackTimer = 1;
        return;
    }
    inventory.getEquippedItem()->weapon->attack(*this, world);
    m_attackTimer = inventory.getEquippedItem()->weapon->getAttackSpeed();


}
void Player::isInteracting(World& world, QuadRenderer& quadrend, Shader& s) {
    
    Vector2 playerTile = World::roundWorldToTile(m_model->position);
    Quad q(Mesh::getBoxMesh(), playerTile, Vector2(TILE_SIZE, TILE_SIZE));
    q.position = World::roundWorldToTile(m_model->position + (m_mouseLookVector * float(TILE_SIZE)));
    q.zIndex = 0.9;
    q.color = Vector4::RBGAtoV4(3, 198, 252, 180);


    Vector2 currentchunkpos = World::worldToChunkPos(q.position);
    auto chunk = world.getChunk((int)currentchunkpos.x, (int)currentchunkpos.y);
    auto& interactables = chunk->getInteractables(); 



    
    for (auto it = interactables.begin(); it != interactables.end();)
    {
        if ((int)it->position.x == (int)q.position.x &&
            (int)it->position.y == (int)q.position.y)
        {
            q.color = Vector4::RBGAtoV4(252, 49, 3, 180);

            if (m_breakingBlock)
            {
                m_breakingBlock = false;
                inventory.addItem((*it).item, 1);
                std::cout << (*it).item->name;
                it = interactables.erase(it);
                continue;
            }
        }

        ++it; 
    }
    
    quadrend.render(&q, false);
}

static bool keys[GLFW_KEY_LAST] = { false };


static int lastDirectionKey = -1;

void Player::handlePressInput(int key) { // shit input handling but gets the job done
    if (key >= 0 && key < GLFW_KEY_LAST) {
        keys[key] = true;
    }

    auto* p = Player::m_currentPlayer;

    
    p->m_velocity = { 0, 0 };
    if (keys[GLFW_KEY_W]) p->m_velocity.y += p->m_speed;
    if (keys[GLFW_KEY_S]) p->m_velocity.y -= p->m_speed;
    if (keys[GLFW_KEY_A]) p->m_velocity.x -= p->m_speed;
    if (keys[GLFW_KEY_D]) p->m_velocity.x += p->m_speed;

    
    if (key == GLFW_KEY_W || key == GLFW_KEY_S ||
        key == GLFW_KEY_A || key == GLFW_KEY_D) {
        lastDirectionKey = key;
    }

    
    switch (lastDirectionKey) {
    case GLFW_KEY_W: p->changeMesh(p->m_backmesh);  break;
    case GLFW_KEY_S: p->changeMesh(p->m_frontmesh); break;
    case GLFW_KEY_A: p->changeMesh(p->m_leftsidemesh);  break;
    case GLFW_KEY_D: p->changeMesh(p->m_rightsidemesh); break;
    }

    if (key== GLFW_MOUSE_BUTTON_RIGHT) {
        m_breakingBlock = true;
    }
    if (key == GLFW_MOUSE_BUTTON_LEFT) {
        m_Attacking = true;
    }
    if (key == GLFW_KEY_E) {
        m_inventoryOpen = !m_inventoryOpen;
    }
}

void Player::handleReleaseInput(int key) {
    if (key >= 0 && key < GLFW_KEY_LAST) {
        keys[key] = false;
    }

    auto* p = Player::m_currentPlayer;
    p->m_velocity = { 0, 0 };
    if (keys[GLFW_KEY_W]) p->m_velocity.y += p->m_speed;
    if (keys[GLFW_KEY_S]) p->m_velocity.y -= p->m_speed;
    if (keys[GLFW_KEY_A]) p->m_velocity.x -= p->m_speed;
    if (keys[GLFW_KEY_D]) p->m_velocity.x += p->m_speed;

    
    if (key == lastDirectionKey) {
        if (keys[GLFW_KEY_W]) lastDirectionKey = GLFW_KEY_W;
        else if (keys[GLFW_KEY_S]) lastDirectionKey = GLFW_KEY_S;
        else if (keys[GLFW_KEY_A]) lastDirectionKey = GLFW_KEY_A;
        else if (keys[GLFW_KEY_D]) lastDirectionKey = GLFW_KEY_D;
        else lastDirectionKey = -1;
    }
    if (key == GLFW_MOUSE_BUTTON_LEFT) {
        m_Attacking = false;
    }
}
