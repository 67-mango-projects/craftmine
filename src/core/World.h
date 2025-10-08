#pragma once
#include "Chunk.h"
#include "Math.h"
#include "Player.h"
#include "../renderer/QuadRenderer.h"
#include <unordered_map>
#include <algorithm>

#define RENDER_DST 3
#define DAYLENGTH 60

enum DIMENSION {
	WORLD,
	NETHER,
	END
};

class World {
private:
	std::unordered_map<long long,Chunk*> m_chunks;
	std::vector<Chunk*> m_loadedChunks;

	const Mesh* dimensiontile[3] = {QuadRenderer::getMesh("tilegrass.png"),QuadRenderer::getMesh("tilenether.png") ,QuadRenderer::getMesh("tileend.png")};
	unsigned int m_seed = 0;
	float m_timeOfDay = 0.5f; 
	float m_lightLevel = 0;
	int m_day = 0;

	DIMENSION m_dimension = DIMENSION::WORLD;
	float m_secondsPerMobSpawn = 20;
	float m_timeleftPerMobSpawn = 0;
public:
	const std::vector<Chunk*>& getLoadedChunks() const { return m_loadedChunks; }
	void updateWorld( float dt);
	void renderWorld(QuadRenderer& r, Shader& s, Camera* c);
	DIMENSION getDimension() { return m_dimension; }
	void setDimension(DIMENSION d) { m_dimension = d; }
	World(unsigned int s) : m_seed(s) {};
	float getTimeHours() {
		float hours = fmod(m_timeOfDay * 24.0f, 24.0f);
		int h = (int)hours;
		return h;
	}
	float getTimeMin() {
		float hours = fmod(m_timeOfDay * 24.0f, 24.0f);
		int h = (int)hours;

		float minutes = (hours - h) * 60.0f;
		int m = (int)minutes;


		int roundedMinutes = (int)std::round(minutes / 10.0f) * 10;
		if (roundedMinutes == 60) {
			roundedMinutes = 0;
			h = (h + 1) % 24;
		}

		return roundedMinutes;
	}
	std::pair<Vector2, Chunk*> getChunkDataFromWorldPos(const Vector2& worldPos, const Camera* camera);
	Vector2 getWorldPosFromChunkData(const Vector2& RelativeChunkCoord, Chunk* chunk);
	static Vector2 roundWorldToTile(const Vector2& worldPos)
{
    // Which chunk are we in?
    int chunkX = static_cast<int>(std::floor((worldPos.x + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));
    int chunkY = static_cast<int>(std::floor((worldPos.y + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));

    // Chunk origin in world-space (bottom-left corner)
    float chunkOriginX = chunkX * CHUNK_SIZE - CHUNK_SIZE * 0.5f;
    float chunkOriginY = chunkY * CHUNK_SIZE - CHUNK_SIZE * 0.5f;

    // Local tile position
    float localX = worldPos.x - chunkOriginX;
    float localY = worldPos.y - chunkOriginY;

    int tileX = static_cast<int>(std::floor(localX / TILE_SIZE));
    int tileY = static_cast<int>(std::floor(localY / TILE_SIZE));

    // Convert back to tile *center*
    float snappedX = chunkOriginX + tileX * TILE_SIZE + TILE_SIZE * 0.5f;
    float snappedY = chunkOriginY + tileY * TILE_SIZE + TILE_SIZE * 0.5f;

    return Vector2(snappedX, snappedY);
}

	static Vector2 worldToChunkPos(const Vector2& worldPos)
	{
		int chunkX = static_cast<int>(std::floor((worldPos.x + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));
		int chunkY = static_cast<int>(std::floor((worldPos.y + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));
		return Vector2(chunkX, chunkY);
	}

	static Vector2 worldToChunkRelative(const Vector2& worldPos)
	{
		// Which chunk are we in?
		int chunkX = static_cast<int>(std::floor((worldPos.x + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));
		int chunkY = static_cast<int>(std::floor((worldPos.y + CHUNK_SIZE * 0.5f) / CHUNK_SIZE));

		// Chunk’s world-space origin (bottom-left corner)
		float chunkOriginX = chunkX * CHUNK_SIZE - CHUNK_SIZE * 0.5f;
		float chunkOriginY = chunkY * CHUNK_SIZE - CHUNK_SIZE * 0.5f;

		// Local position within that chunk
		float localX = worldPos.x - chunkOriginX;
		float localY = worldPos.y - chunkOriginY;

		// Convert to tile indices
		int tileX = static_cast<int>(std::floor(localX / TILE_SIZE));
		int tileY = static_cast<int>(std::floor(localY / TILE_SIZE));

		// Clamp to valid tile range
		tileX = std::clamp(tileX, 0, CHUNK_RESOLUTION - 1);
		tileY = std::clamp(tileY, 0, CHUNK_RESOLUTION - 1);

		return Vector2(tileX, tileY);
	}

	Chunk* getChunk(int cx, int cy);
	int getDay() {
		return m_day;
	}
private:
	
};