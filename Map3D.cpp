#include "map3d.h"

#include "flatterraingenerator.h"
#include "demterraingenerator.h"
#include "quantizedmeshterraingenerator.h"

static QString _matrix4x4toString(const QMatrix4x4& m)
{
	const float* d = m.constData();
	QStringList elems;
	for (int i = 0; i < 16; ++i)
		elems << QString::number(d[i]);
	return elems.join(' ');
}

static QMatrix4x4 _stringToMatrix4x4(const QString& str)
{
	QMatrix4x4 m;
	float* d = m.data();
	QStringList elems = str.split(' ');
	for (int i = 0; i < 16; ++i)
		d[i] = elems[i].toFloat();
	return m;
}

QString altClampingToString(AltitudeClamping altClamp)
{
	switch (altClamp)
	{
	case AltClampAbsolute: return QStringLiteral("absolute");
	case AltClampRelative: return QStringLiteral("relative");
	case AltClampTerrain: return QStringLiteral("terrain");
	default: Q_ASSERT(false); return QString();
	}
}

AltitudeClamping altClampingFromString(const QString& str)
{
	if (str == "absolute")
		return AltClampAbsolute;
	else if (str == "terrain")
		return AltClampTerrain;
	else   // "relative"  (default)
		return AltClampRelative;
}

QString altBindingToString(AltitudeBinding altBind)
{
	switch (altBind)
	{
	case AltBindVertex: return QStringLiteral("vertex");
	case AltBindCentroid: return QStringLiteral("centroid");
	default: Q_ASSERT(false); return QString();
	}
}

AltitudeBinding altBindingFromString(const QString& str)
{
	if (str == "vertex")
		return AltBindVertex;
	else  // "centroid"  (default)
		return AltBindCentroid;
}

Map3D::Map3D()
	: originX(0)
	, originY(0)
	, originZ(0)
	, backgroundColor(Qt::black)
	, zExaggeration(1)
	, tileTextureSize(512)
	, maxTerrainError(3.f)
	, skybox(false)
	, showBoundingBoxes(false)
	, drawTerrainTileInfo(false)
{
}

void Map3D::resolveReferences()
{
	terrainGenerator->resolveReferences();

	// for (int i = 0; i < polygonRenderers.count(); ++i)
	// {
		// PolygonRenderer& r = polygonRenderers[i];
		// r.resolveReferences(project);
	// }

	// for (int i = 0; i < pointRenderers.count(); ++i)
	// {
		// PointRenderer& r = pointRenderers[i];
		// r.resolveReferences(project);
	// }

	// for (int i = 0; i < lineRenderers.count(); ++i)
	// {
		// LineRenderer& r = lineRenderers[i];
		// r.resolveReferences(project);
	//}
}
