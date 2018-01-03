#pragma once

#include "terraingenerator.h"

#include <QRect>

class QuantizedMeshTerrainGenerator : public TerrainGenerator
{
public:
	QuantizedMeshTerrainGenerator();

	void setBaseTileFromExtent(const QRect &extentInTerrainCrs);

	void quadTreeTileToBaseTile(int x, int y, int z, int &tx, int &ty, int &tz) const;

	TerrainGenerator::Type type() const override;
	QRect extent() const override;

	virtual ChunkLoader* createChunkLoader(ChunkNode* node) const override;

	int terrainBaseX, terrainBaseY, terrainBaseZ;   //!< coordinates of the base tile
};

