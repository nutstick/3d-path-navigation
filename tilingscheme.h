#pragma once

#include <QRect>

//! The class encapsulates tiling scheme (just like with WMTS / TMS / XYZ layers).
//! The origin (tile [0,0]) is in bottom-left corner.
class TilingScheme
{
public:
	//! Creates invalid tiling scheme
	TilingScheme();

	//! Creates tiling scheme where level 0 tile is centered at the full extent and the full extent completely fits into the level 0 tile
	TilingScheme(const QRect &fullExtent);

	//! Returns map coordinates at tile coordinates (for lower-left corner of the tile)
	QPoint tileToMap(int x, int y, int z) const;
	//! Returns tile coordinates for given map coordinates and Z level
	void mapToTile(const QPoint& pt, int z, float &x, float &y) const;

	//! Returns map coordinates of the extent of a tile
	QRect tileToExtent(int x, int y, int z) const;

	//! Returns coordinates of a tile that most tightly fits the whole extent
	void extentToTile(const QRect& extent, int& x, int& y, int& z) const;

	QPoint mapOrigin;	//!< origin point in map coordinates: (0,0) in the tiling scheme
	double baseTileSide;	//!< length of tile side at zoom level 0 in map coordinates
};

