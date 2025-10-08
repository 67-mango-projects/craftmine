#include "Weapon.h"
#include "Player.h"
#include "World.h"
#include "Projectiles.h"
#include <spdlog.h>
#include <random>
float randomSpread(float degrees) {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-degrees, degrees);
	return glm::radians(dist(rng)); // convert to radians for trig
}
float randomSpreadP(float range) {
	static std::mt19937 rng(std::random_device{}());
	std::uniform_real_distribution<float> dist(-range, range);
	return dist(rng);
}


void MeleeWeapon::attack(Entity& player, World& world, bool canHitPlayer) {
	auto psys = ProjectileSystem::getCurrentProjectileSystem();


	Vector2 dir = glm::normalize(glm::vec2(player.getLookVector()));
	Vector2 perp(-dir.y, dir.x);

	for (int i = 0; i < hits; i++) {
		float sideOffset = randomSpreadP(precision);
		float forwardOffset = randomSpreadP(precision);

		Vector2 spawnPos = player.getPosition()
			+ dir * (slashOffset + forwardOffset)   
			+ perp * sideOffset;                    

		psys->spawn(spawnPos, hitbox, dir, this, &player, lifeTime, 100.0f, canHitPlayer);
	}
}

void ScytheWeapon::attack(Entity& player, World& world, bool canHitPlayer) {
	int degrees = glm::radians((360 / cycles) * currentCycle);

	auto psys = ProjectileSystem::getCurrentProjectileSystem();
	Vector2 dir = Vector2(cos(degrees), sin(degrees));

	Quad forwardAttackHitbox = hitbox;
	forwardAttackHitbox.size = Vector2(hitbox.size.x * forwardAttackLength, hitbox.size.y);
	   
	psys->spawn((player.getPosition() + (player.getLookVector() * (hitbox.size.x / 6))) + randomSpreadP(attackPrecision), forwardAttackHitbox, player.getLookVector(), this, &player, 0.3, 0, canHitPlayer);
	psys->spawn(player.getPosition(), hitbox, dir, this, &player, 1, 300, canHitPlayer);
	currentCycle++;
}
void BlastWeapon::attack(Entity& player, World& world, bool canHitPlayer) {
	auto psys = ProjectileSystem::getCurrentProjectileSystem();
	psys->spawn(player.getPosition() + player.getLookVector() * slashOffset, hitbox, player.getLookVector(), this, &player, 0.3, 0, canHitPlayer);
}

void ProjectileWeapon::attack(Entity& player, World& world, bool canHitPlayer) {
	auto psys = ProjectileSystem::getCurrentProjectileSystem();

	for (int p = 0; p < amountOfProj; p++) {
		Vector2 dir = player.getLookVector();


		float angleOffset = randomSpread(precision);

		Vector2 spreadDir;
		spreadDir.x = dir.x * cos(angleOffset) - dir.y * sin(angleOffset);
		spreadDir.y = dir.x * sin(angleOffset) + dir.y * cos(angleOffset);

		spreadDir = glm::normalize(glm::vec2(spreadDir));

		psys->spawn(
			player.getPosition() + spreadDir * 20.0f,
			hitbox,
			spreadDir,
			this,
			&player,
			20.0f,
			projectileSpeed,
			canHitPlayer
		);

	}

}