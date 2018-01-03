#pragma once

#include "chunkloader.h"

#include <QImage>
#include "tile.h"

class Terrain;

class TerrainChunkLoader : public ChunkLoader
{
public:
	TerrainChunkLoader(Terrain* terrain, ChunkNode *node);

	void loadTexture();
	void createTextureComponent(Qt3DCore::QEntity *entity);

protected:
	Terrain* mTerrain;

private:
	Tile mTile;
	QString mTileDebugText;
	QImage mTextureImage;
};

