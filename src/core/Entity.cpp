#include "Entity.h"
#include<glm/gtc/matrix_transform.hpp>
#include "World.h"
void Entity::initializeEntitys() {
	g_entitys.insert({ EntityType::ZOMBIE, new Entity(EntityType::ZOMBIE,100,110,Quad(QuadRenderer::getMesh("zombie.png"),Vector2(0,0),Vector2(50,50)),Inventory::ENTWEAPLIST.zombieslash)});
	g_entitys.insert({ EntityType::SKELETON, new Entity(EntityType::SKELETON,100,50,Quad(QuadRenderer::getMesh("skeleton.png"),Vector2(0,0),Vector2(50,50)),Inventory::ENTWEAPLIST.skeletonarrows )});
	g_entitys.insert({ EntityType::WIZARD, new Entity(EntityType::WIZARD,1000,70,Quad(QuadRenderer::getMesh("wizard.png"),Vector2(0,0),Vector2(50,50)),Inventory::ENTWEAPLIST.wizardslashprojectile) });
	g_entitys.insert({ EntityType::BLAZE, new Entity(EntityType::BLAZE,12000,70,Quad(QuadRenderer::getMesh("blaze.png"),Vector2(0,0),Vector2(50,50)),Inventory::ENTWEAPLIST.blazefireball) });
	g_entitys.insert({ EntityType::AARUSH, new Entity(EntityType::AARUSH,100000,150,Quad(QuadRenderer::getMesh("aarush.png"),Vector2(0,0),Vector2(70,70)),Inventory::ENTWEAPLIST.aarush) });
	g_entitys.insert({ EntityType::UNDEADBOSS, new Entity(EntityType::UNDEADBOSS,2000,40,Quad(QuadRenderer::getMesh("undeadboss.png"),Vector2(0,0),Vector2(150,150)),Inventory::ENTWEAPLIST.undeadBossBarrage) });
	g_entitys.insert({ EntityType::WITHER, new Entity(EntityType::WITHER,500000,50,Quad(QuadRenderer::getMesh("witherboss.png"),Vector2(0,0),Vector2(150,150)),Inventory::ENTWEAPLIST.witherBossHeadBall) });
	g_entitys.insert({ EntityType::DRAGON, new Entity(EntityType::DRAGON,1000000,95,Quad(QuadRenderer::getMesh("dragonboss.png"),Vector2(0,0),Vector2(200,200)),Inventory::ENTWEAPLIST.EnderDragonBall) });
}
Entity* Entity::spawn(EntityType type, Vector2 position) {
    Entity* e = g_entitys[type]->clone();
	e->setPosition(position);
	Entity::g_entityList.push_back(e);
	return e;
}

Entity* Entity::clone() const {
    Entity* e = new Entity(*this);
    e->m_model = new Quad(*m_model); // deep copy mesh/quad
    return e;
}
 
void Entity::attack(World& world) {
    if (m_attackTimer > 0.0f)
        return;
    if (this->m_reservedEntWeapon == nullptr)
        return;
    if (this->m_reservedEntWeapon->getProjectileSpeed() == 0) {
        
    }
    if (((20 /*projectile lifetime*/ * m_reservedEntWeapon->getProjectileSpeed()) + m_reservedEntWeapon->getHitbox().size.x) > this->m_model->position.Magnitude(Player::getCurrentPlayer()->getPosition())) {
        this->m_reservedEntWeapon->attack(*this, world, true);
    }
    m_attackTimer = this->m_reservedEntWeapon->getAttackSpeed();
}
int entityKills = 0;
void Entity::renderEntitys(QuadRenderer& r, Shader& s, Camera* c) {
	for (auto it = g_entityList.begin(); it != g_entityList.end(); ) {
		float magnitude = c->getPosition().Magnitude((*it)->getPosition());
        r.render((*it)->getModel());
		if (magnitude > (RENDER_DST * CHUNK_SIZE )) { // despawn
			delete* it;
			it = g_entityList.erase(it);
			continue;
		}
        if ((*it)->m_health < 0) {
            if ((*it)->type == UNDEADBOSS) {
                Player::getCurrentPlayer()->getInv().addItem(Inventory::ITEMLIST.NetherWarp, 1);
                Player::getCurrentPlayer()->setPlayerLevel(Player::getCurrentPlayer()->getPlayerLevel() + 5);
            }
            if ((*it)->type == WITHER) {
                Player::getCurrentPlayer()->getInv().addItem(Inventory::ITEMLIST.EnderWarp, 1);
                Player::getCurrentPlayer()->setPlayerLevel(Player::getCurrentPlayer()->getPlayerLevel() + 20);
            }
            if ((*it)->type == DRAGON) { // add finisahiing the game code soon
                Player::getCurrentPlayer()->getInv().addItem(Inventory::ITEMLIST.EnderWarp, 1);
                Player::getCurrentPlayer()->setPlayerLevel(Player::getCurrentPlayer()->getPlayerLevel() + 20);
            }
            delete* it;
            entityKills++;
            it = g_entityList.erase(it);
            if (entityKills % 5 == 0) { // 5 is the amount of kills needed for level up
                
                Player::getCurrentPlayer()->setPlayerLevel(Player::getCurrentPlayer()->getPlayerLevel()+1);
            }
            
            
            continue;
        }
		++it;
		
	}	
}


void Entity::updateEntitys(float dt, World& world, Player* p) {



    for (auto it = g_entityList.begin(); it != g_entityList.end(); ++it) {
        Entity* e = *it;
        (*it)->m_attackTimer -= dt;
        
        // --- Compute movement toward player ---
        glm::vec2 diff = glm::vec2(p->getPosition() - e->getPosition());
        glm::vec2 direction = glm::length(diff) > 0.0001f
            ? glm::normalize(diff)
            : glm::vec2(0.0f);

        (*it)->lookVector = direction;
        Vector2 move = Vector2(direction) * e->getSpeed() * dt;

        // Cache half sizes for the entity (A)
        float halfAX = e->getModel()->size.x * 0.5f;
        float halfAY = e->getModel()->size.y * 0.5f;

        (*it)->attack(world);

        // --- Horizontal movement ---
        e->setPosition(e->getPosition() + Vector2(move.x, 0));

        for (auto& chunk : world.getLoadedChunks()) {
            for (const auto& tile : chunk->getInteractables()) {
                Quad q(tile.mesh, tile.position, Vector2(TILE_SIZE, TILE_SIZE));
                if (e->getModel()->isCollidingWith_NoRotation(q)) {
                    float halfBX = q.size.x * 0.5f;

                    // Resolve horizontal overlap
                    if (move.x > 0)
                        e->setPosition(Vector2(q.position.x - (halfAX + halfBX), e->getPosition().y));
                    else if (move.x < 0)
                        e->setPosition(Vector2(q.position.x + (halfAX + halfBX), e->getPosition().y));

                    move.x = 0;
                }
            }
        }

        // --- Vertical movement ---
        e->setPosition(e->getPosition() + Vector2(0, move.y));

        for (auto& chunk : world.getLoadedChunks()) {
            for (const auto& tile : chunk->getInteractables()) {
                Quad q(tile.mesh, tile.position, Vector2(TILE_SIZE, TILE_SIZE));
                if (e->getModel()->isCollidingWith_NoRotation(q)) {
                    float halfBY = q.size.y * 0.5f;

                    // Resolve vertical overlap
                    if (move.y > 0)
                        e->setPosition(Vector2(e->getPosition().x, q.position.y - (halfAY + halfBY)));
                    else if (move.y < 0)
                        e->setPosition(Vector2(e->getPosition().x, q.position.y + (halfAY + halfBY)));

                    move.y = 0;
                }
            }
        }
    }
}
