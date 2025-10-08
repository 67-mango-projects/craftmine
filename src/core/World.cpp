#include "World.h" // this shit took too long to make finish timestamp ~3 30 am
#include "Chunk.h"
#include "../renderer/QuadRenderer.h"
#include "../renderer/Shader.h"
#include<random>
#define M_PI 3.1415926535897932384

void World::renderWorld(QuadRenderer &r,Shader& s,Camera* c) { // debugging this lump of turd was so cancerous because opengl sucks ass
	
    Vector2 playerPos = c->getPosition();

	int playerChunkX = (int)std::floor(playerPos.x / (CHUNK_SIZE));
	int playerChunkY = (int)std::floor(playerPos.y / (CHUNK_SIZE));

    s.bind();
    s.setUniform1f("u_lightLevel", m_lightLevel);
    s.setUniform3f("u_dayColor", 0.9f, 0.95f, 1.0f);
    s.setUniform3f("u_nightColor", 0.1f, 0.1f, 0.3f); 

    std::vector<Chunk*> loaded_chunks;
    for (int cy = playerChunkY - RENDER_DST; cy <= playerChunkY + RENDER_DST; ++cy) {
        for (int cx = playerChunkX - RENDER_DST; cx <= playerChunkX + RENDER_DST; ++cx) {
            Chunk* c = getChunk(cx, cy);
            loaded_chunks.push_back(c);
            Quad q2 = Quad(dimensiontile[m_dimension], Vector2(cx * CHUNK_SIZE, cy * CHUNK_SIZE), Vector2(CHUNK_SIZE, CHUNK_SIZE));
            q2.zIndex = -0.1;
            r.render(&q2);

            for (auto& interactables : c->getInteractables()) {
                Quad q = Quad(interactables.mesh, interactables.position, Vector2(TILE_SIZE, TILE_SIZE));
                
                q.zIndex = 0.7;
                r.render(&q);
                
            }
        }
    }
    m_loadedChunks = loaded_chunks;
}


Chunk* World::getChunk(int cx, int cy) {
    
    long long key = ((long long)cx << 32) | (unsigned int)cy;
    if (m_chunks.find(key) == m_chunks.end()) {
        const Mesh* tileMesh = QuadRenderer::getMesh("tilegrass.png"); // COME BACK TO THIS LATER SINCE WE NEED TO CHECK WHAT DIMENSION WE ARE IN
        m_chunks[key] = new Chunk(tileMesh, Vector2(cx, cy),m_seed);
    }
    return m_chunks[key];
}

float randomRange(float min, float max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}
int randomRange(int min, int max) {
    static std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(gen);
}
int bossSpawnedAtLevel = -1;
void World::updateWorld(float dt) {
    // be quick or DIE
    float nightFactor = 1.0f - m_lightLevel;  
    float spawnRateMultiplier = 1.0f + nightFactor * (10.0f + (m_day* 0.01));
    m_timeleftPerMobSpawn -= dt * spawnRateMultiplier;

    if (m_timeleftPerMobSpawn < 0) {
        Camera* player = Camera::getCurrentCamera();
        float angle = randomRange(0.0f, 2.0f * M_PI);
        float radius = randomRange(300.0f, 800.0f);
        Vector2 spawnPos = player->getPosition() + Vector2(std::cos(angle), std::sin(angle)) * radius;
        if (m_dimension == DIMENSION::WORLD) {
            int randomType = randomRange(1, 2);
            Entity::spawn((EntityType)randomType, spawnPos);
            m_timeleftPerMobSpawn = m_secondsPerMobSpawn;
        }
        if (m_dimension == DIMENSION::NETHER) {
            int randomType = randomRange(1, 4);
            Entity::spawn((EntityType)BLAZE, spawnPos);
            Entity::spawn((EntityType)randomType, spawnPos);
            m_timeleftPerMobSpawn = m_secondsPerMobSpawn;
        }
        if (m_dimension == DIMENSION::END) {
            int randomType = randomRange(2, 5);
            Entity::spawn((EntityType)randomType, spawnPos);
            Entity::spawn((EntityType)randomType, spawnPos);
            Entity::spawn((EntityType)randomType, spawnPos);
            Entity::spawn((EntityType)randomType, spawnPos);
            Entity::spawn((EntityType)BLAZE, spawnPos);
            Entity::spawn((EntityType)AARUSH, spawnPos);
            m_timeleftPerMobSpawn = m_secondsPerMobSpawn;
        }
    }

    if (Player::getCurrentPlayer()->getPlayerLevel() % 100 == 0 && bossSpawnedAtLevel != Player::getCurrentPlayer()->getPlayerLevel() && m_day > 20 && m_dimension == DIMENSION::END) {
        bossSpawnedAtLevel = Player::getCurrentPlayer()->getPlayerLevel();
        float angle = randomRange(0.0f, 2.0f * M_PI);
        float radius = randomRange(300.0f, 800.0f);
        Vector2 spawnPos = Camera::getCurrentCamera()->getPosition() + Vector2(std::cos(angle), std::sin(angle)) * radius;
        Entity::spawn(EntityType::DRAGON, spawnPos);
    }
    else if (Player::getCurrentPlayer()->getPlayerLevel() % 50 == 0 && bossSpawnedAtLevel != Player::getCurrentPlayer()->getPlayerLevel() && m_day > 10 == 1 && m_dimension== DIMENSION::NETHER) {
        bossSpawnedAtLevel = Player::getCurrentPlayer()->getPlayerLevel();
        float angle = randomRange(0.0f, 2.0f * M_PI);
        float radius = randomRange(300.0f, 800.0f);
        Vector2 spawnPos = Camera::getCurrentCamera()->getPosition() + Vector2(std::cos(angle), std::sin(angle)) * radius;
        Entity::spawn(EntityType::WITHER, spawnPos);
    }
    else if (Player::getCurrentPlayer()->getPlayerLevel() % 20 == 0 && bossSpawnedAtLevel != Player::getCurrentPlayer()->getPlayerLevel() && m_day > 5) {
        bossSpawnedAtLevel = Player::getCurrentPlayer()->getPlayerLevel();
        float angle = randomRange(0.0f, 2.0f * M_PI);
        float radius = randomRange(300.0f, 800.0f);
        Vector2 spawnPos = Camera::getCurrentCamera()->getPosition() + Vector2(std::cos(angle), std::sin(angle)) * radius;
        Entity::spawn(EntityType::UNDEADBOSS, spawnPos);
    }
    // --- day/night cycle ---
    m_timeOfDay += dt / DAYLENGTH;
    if (m_timeOfDay > 1.0f) { m_timeOfDay -= 1.0f; m_day++; }
    m_lightLevel = 0.5f + 0.5f * sin(m_timeOfDay * 2.0f * M_PI - M_PI / 2.0f);
}
std::pair<Vector2, Chunk*> World::getChunkDataFromWorldPos(const Vector2& worldPos, const Camera* camera) {
    // --- Get player chunk position ---
    Vector2 playerPos = camera->getPosition();
    int playerChunkX = static_cast<int>(std::floor(playerPos.x / CHUNK_SIZE));
    int playerChunkY = static_cast<int>(std::floor(playerPos.y / CHUNK_SIZE));

    // --- Get target world position's chunk position ---
    int targetChunkX = static_cast<int>(std::floor(worldPos.x / CHUNK_SIZE));
    int targetChunkY = static_cast<int>(std::floor(worldPos.y / CHUNK_SIZE));

    // --- Compute relative offset ---
    int relX = targetChunkX - playerChunkX;
    int relY = targetChunkY - playerChunkY;

    // --- Retrieve the chunk itself ---
    Chunk* targetChunk = getChunk(targetChunkX, targetChunkY);

    // --- Return both in one structure ---
    return { Vector2((float)relX, (float)relY), targetChunk };
}

Vector2 World::getWorldPosFromChunkData(const Vector2& RelativeChunkCoord, Chunk* chunk) {
    return Vector2((chunk->getPosition().x * CHUNK_SIZE) + RelativeChunkCoord.x * TILE_SIZE, (chunk->getPosition().y * CHUNK_SIZE) + RelativeChunkCoord.y * TILE_SIZE);
}