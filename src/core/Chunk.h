#pragma once
#define CHUNK_SIZE 400
#define CHUNK_RESOLUTION 9 // how much items can fit in chunk
#define TILE_SIZE (CHUNK_SIZE / CHUNK_RESOLUTION)
#include "../renderer/QuadRenderer.h"
#include "Interactables.h"
class Chunk {
private:
	const Mesh* m_mesh;
	std::vector<Interactable> m_interactables;
	Vector2 m_position;
	Vector2 m_size = Vector2(CHUNK_SIZE, CHUNK_SIZE);
public:
	Chunk(const Mesh* m, Vector2 pos, unsigned int seed);
	const Mesh* getTileMesh() { return m_mesh; }
	Vector2 getPosition() { return m_position; }
	std::vector<Interactable>& getInteractables() { return m_interactables; }
};