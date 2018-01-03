#pragma once

#include "terraingenerator.h"
#include "chunkloader.h"

namespace Qt3DExtras
{
	class QPlaneGeometry;
}

class FlatTerrainGenerator : public TerrainGenerator
{
public:
	FlatTerrainGenerator();

	virtual ChunkLoader *createChunkLoader(ChunkNode* node) const override;

	Type type() const override;
	QRect extent() const override;
	virtual	void rootChunkHeightRange(float& hMin, float& hMax) const override;
	void setExtent(const QRect& extent);

private:
	Qt3DExtras::QPlaneGeometry* tileGeometry;

	void updateTilingScheme();

	QRect mExtent;
};

