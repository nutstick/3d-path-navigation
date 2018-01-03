#include "quantizedmeshterraingenerator.h"

#include "map3d.h"
#include "quantizedmeshgeometry.h"
#include "terrain.h"

#include <Qt3DRender/QGeometryRenderer>

#include "chunknode.h"
#include "terrainchunkloader.h"

class QuantizedMeshTerrainChunkLoader : TerrainChunkLoader
{
	QuantizedMeshTerrainChunkLoader(Terrain* terrain, ChunkNode* node)
		: TerrainChunkLoader(terrain, node)
		, qmt(nullptr)
	{
		const Map3D& map = mTerrain->map3D();
		QuantizedMeshTerrainGenerator* generator = static_cast<QuantizedMeshTerrainGenerator*>(map.terrainGenerator.get());

		generator->quadTreeTileToBaseTile(node->x, node->y, node->z, tx, ty, tz);
		tileRect = map.terrainGenerator->terrainTilingScheme.tileToExtent(tx, ty, tz);

		// we need map settings here for access to mapToPixel
		// mapSettings.setLayers(map.layers());
		// mapSettings.setOutputSize(QSize(map.tileTextureSize, map.tileTextureSize));
		// mapSettings.setDestinationCrs(map.crs);
		// mapSettings.setExtent(mTerrain->terrainToMapTransform().transformBoundingBox(tileRect));
	}
protected:
	QuantizedMeshTile * qmt;
	int tx, ty, tz;
	QRect tileRect;
};

QuantizedMeshTerrainGenerator::QuantizedMeshTerrainGenerator()
{
	terrainBaseX = terrainBaseY = terrainBaseZ = 0;

	// TODO: EPSG:4326 meaning?
	// terrainTilingScheme = TilingScheme(QgsRectangle(-180,-90,0,90), QgsCoordinateReferenceSystem("EPSG:4326"));
	terrainTilingScheme = TilingScheme(QRect(-180, -90, 0, 90));
}

void QuantizedMeshTerrainGenerator::setBaseTileFromExtent(const QRect & extentInTerrainCrs)
{
	terrainTilingScheme.extentToTile(extentInTerrainCrs, terrainBaseX, terrainBaseY, terrainBaseZ);
}

void QuantizedMeshTerrainGenerator::quadTreeTileToBaseTile(int x, int y, int z, int & tx, int & ty, int & tz) const
{
	// true tile coords (using the base tile pos)
	int multiplier = pow(2, z);
	tx = terrainBaseX * multiplier + x;
	ty = terrainBaseY * multiplier + y;
	tz = terrainBaseZ + z;
}

TerrainGenerator::Type QuantizedMeshTerrainGenerator::type() const
{
	return TerrainGenerator::QuantizedMesh;
}

QRect QuantizedMeshTerrainGenerator::extent() const
{
	return terrainTilingScheme.tileToExtent(terrainBaseX, terrainBaseY, terrainBaseZ);
}

ChunkLoader * QuantizedMeshTerrainGenerator::createChunkLoader(ChunkNode * node) const
{
	return new QuantizedMeshTerrainChunkLoader(mTerrain, node);
}
