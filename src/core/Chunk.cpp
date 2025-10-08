#include "Chunk.h"
#include <random>
#include <spdlog.h>
inline uint64_t createKey(Vector2 tile, Vector2 chunk, unsigned int seed) {
    uint64_t x = static_cast<uint64_t>(chunk.x * CHUNK_RESOLUTION + tile.x);
    uint64_t y = static_cast<uint64_t>(chunk.y * CHUNK_RESOLUTION + tile.y);

    // primes to xor coordinates
    uint64_t key = seed
        ^ (x * 73856093ull)
        ^ (y * 19349663ull);

    return key;
}
float generateRandom(Vector2 tile_position, Vector2 chunk_position, unsigned int seed) {
    auto key = createKey(tile_position, chunk_position, seed);
    std::mt19937_64 gen(key);
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    return dist(gen);
}

const Interactable* pickTile(Vector2 tile, Vector2 chunk, unsigned int seed) {
    float roll = generateRandom(tile, chunk, seed);

    float air = 1.0f - ORE_DENSITY;

    if (roll < air)
        return nullptr;

    roll = (roll - air) / ORE_DENSITY;


    float totalWeight = 0.0f;
    for (const auto& item : Interactable::g_Claimables)
        totalWeight += item->chance;

    float cumulative = 0.0f;
    for (const auto& item : Interactable::g_Claimables) {
        cumulative += item->chance / totalWeight; 
        if (roll < cumulative)
            return item;
    }

    return nullptr;
}

Chunk::Chunk(const Mesh* m, Vector2 pos,unsigned int seed) : m_mesh(m), m_position(pos) {
    for (int y = 0; y < CHUNK_RESOLUTION; ++y) {
        for (int x = 0; x < CHUNK_RESOLUTION; ++x) {
            const Interactable* tileInteractable = pickTile(Vector2(x, y), pos, seed /*SEED CHANGE TO GLOBAL SOON*/);
            
            if (tileInteractable == nullptr) {
                continue;
            }
            
            Interactable returnTile = *tileInteractable;
            returnTile.position = Vector2(
                (pos.x * CHUNK_SIZE - CHUNK_SIZE * 0.5f) + x * TILE_SIZE + TILE_SIZE * 0.5f,
                (pos.y * CHUNK_SIZE - CHUNK_SIZE * 0.5f) + y * TILE_SIZE + TILE_SIZE * 0.5f
            );

            returnTile.chunkPosition = Vector2(x, y);
            m_interactables.push_back(returnTile);
        }
    }

}
