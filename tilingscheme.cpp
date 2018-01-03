#include "tilingscheme.h"

TilingScheme::TilingScheme()
	: mapOrigin()
	, baseTileSide(0)
{
}

TilingScheme::TilingScheme(const QRect &fullExtent)
{
	mapOrigin = fullExtent.bottomLeft();
	baseTileSide = qMax(fullExtent.width(), fullExtent.height());
}

QPoint TilingScheme::tileToMap(int x, int y, int z) const
{
	double tileSide = baseTileSide / pow(2, z);
	double mx = mapOrigin.x() + x * tileSide;
	double my = mapOrigin.y() + y * tileSide;
	return QPoint(mx, my);
}

void TilingScheme::mapToTile(const QPoint & pt, int z, float & x, float & y) const
{
	double tileSide = baseTileSide / pow(2, z);
	x = (pt.x() - mapOrigin.x()) / tileSide;
	y = (pt.y() - mapOrigin.y()) / tileSide;
}

QRect TilingScheme::tileToExtent(int x, int y, int z) const
{
	QPoint pt0 = tileToMap(x, y, z);
	QPoint pt1 = tileToMap(x + 1, y + 1, z);;

	return QRect(pt0, pt1);
}

void TilingScheme::extentToTile(const QRect & extent, int & x, int & y, int & z) const
{
	x = y = z = 0;	// start with root tile
	while (1)
	{
		// try to see if any child tile fully contains our extent - if so, go deeper
		if (tileToExtent(x * 2, y * 2, z + 1).contains(extent))
		{
			x = x * 2;
			y = y * 2;
		}
		else if (tileToExtent(x * 2 + 1, y * 2, z + 1).contains(extent))
		{
			x = x * 2 + 1;
			y = y * 2;
		}
		else if (tileToExtent(x * 2 + 1, y * 2 + 1, z + 1).contains(extent))
		{
			x = x * 2 + 1;
			y = y * 2 + 1;
		}
		else
		{
			return;
		}
		z++;
	}
}
