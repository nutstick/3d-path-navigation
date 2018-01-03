#pragma once

#include "tilingscheme.h"
#include "chunkloader.h"

class AABB;
class Map3D;
class QRect;
class Terrain;

class TerrainGenerator : public ChunkLoaderFactory
{
public:
	enum Type
	{
		Flat,
		Dem,
		QuantizedMesh,
	};

	virtual ~TerrainGenerator() {}

	void setTerrain(Terrain* t) { mTerrain = t; }

	//! What texture generator implementation is this
	virtual Type type() const = 0;

	//! extent of the terrain in terrain's CRS
	virtual QRect extent() const = 0;

	//! Returns bounding box of the root chunk
	virtual AABB rootChunkBbox(const Map3D& map) const;

	//! Returns error of the root chunk in world coordinates
	virtual float rootChunkError(const Map3D& map) const;

	//! Returns height range of the root chunk in world coordinates
	virtual void rootChunkHeightRange(float& hMin, float& hMax) const;

	//! Returns height at (x,y) in terrain's CRS
	virtual float heightAt(double x, double y, const Map3D& map) const;

	//! After read of XML, resolve references to any layers that have been read as layer IDs
	virtual void resolveReferences() {};

	static QString typeToString(Type type);
	
	TilingScheme terrainTilingScheme;   //!< tiling scheme of the terrain

	Terrain *mTerrain = nullptr;
};

