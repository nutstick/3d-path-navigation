#include "terrain.h"

#include "aabb.h"
#include "map3d.h"
#include "maptexturegenerator.h"
#include "terraingenerator.h"

Terrain::Terrain(int maxLevel, const Map3D& map, Qt3DCore::QNode* parent)
	: ChunkedEntity(map.terrainGenerator->rootChunkBbox(map),
		map.terrainGenerator->rootChunkError(map),
		map.maxTerrainError, maxLevel, map.terrainGenerator.get(), parent)
	, map(map)
{
	map.terrainGenerator->setTerrain(this);

	mMapTextureGenerator = new MapTextureGenerator(map);
}

Terrain::~Terrain()
{
	delete mMapTextureGenerator;
}