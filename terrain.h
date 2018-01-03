#pragma once

#include "chunkedentity.h"

class Map3D;
class MapTextureGenerator;
class TerrainGenerator;

class Terrain : public ChunkedEntity
{
	Q_OBJECT
public:
	explicit Terrain(int maxLevel, const Map3D& map, Qt3DCore::QNode* parent = nullptr);
	
	~Terrain();

	const Map3D& map3D() const { return map; }
	MapTextureGenerator* mapTextureGenerator() { return mMapTextureGenerator; }

private:
	const Map3D& map;
	MapTextureGenerator* mMapTextureGenerator;
};

