#pragma once

#include <memory>
#include <QColor>
#include <QMatrix4x4>

class MapTextureGenerator;
class TerrainGenerator;

//! how to handle altitude of vector features
enum AltitudeClamping
{
	AltClampAbsolute,   //!< z_final = z_geometry
	AltClampRelative,   //!< z_final = z_terrain + z_geometry
	AltClampTerrain,    //!< z_final = z_terrain
};

QString altClampingToString(AltitudeClamping altClamp);
AltitudeClamping altClampingFromString(const QString& str);

//! how to handle clamping of vertices of individual features
enum AltitudeBinding
{
	AltBindVertex,      //!< clamp every vertex of feature
	AltBindCentroid,    //!< clamp just centroid of feature
};

QString altBindingToString(AltitudeBinding altBind);
AltitudeBinding altBindingFromString(const QString& str);

//! Basic shading material used for rendering
class PhongMaterialSettings
{
public:
	PhongMaterialSettings()
		: mAmbient(QColor::fromRgbF(0.05f, 0.05f, 0.05f, 1.0f))
		, mDiffuse(QColor::fromRgbF(0.7f, 0.7f, 0.7f, 1.0f))
		, mSpecular(QColor::fromRgbF(0.01f, 0.01f, 0.01f, 1.0f))
		, mShininess(150.0f)
	{
	}

	QColor ambient() const { return mAmbient; }
	QColor diffuse() const { return mDiffuse; }
	QColor specular() const { return mSpecular; }
	float shininess() const { return mShininess; }

	void setAmbient(const QColor &ambient) { mAmbient = ambient; }
	void setDiffuse(const QColor &diffuse) { mDiffuse = diffuse; }
	void setSpecular(const QColor &specular) { mSpecular = specular; }
	void setShininess(float shininess) { mShininess = shininess; }

private:
	QColor mAmbient;
	QColor mDiffuse;
	QColor mSpecular;
	float mShininess;
};

class Map3D
{
public:
	Map3D();

	void resolveReferences();

	double originX, originY, originZ;   //!< coordinates in map CRS at which our 3D world has origin (0,0,0)
	QColor backgroundColor;   //!< background color of the scene
	
	//
	// terrain related config
	//

	double zExaggeration;   //!< multiplier of terrain heights to make the terrain shape more pronounced
	int tileTextureSize;   //!< size of map textures of tiles in pixels (width/height)
	int maxTerrainError;   //!< maximum allowed terrain error in pixels
	std::unique_ptr<TerrainGenerator> terrainGenerator;  //!< implementation of the terrain generation

	//
	// 3D renderers
	//

	// QList<PolygonRenderer> polygonRenderers;   //!< stuff to render as polygons
	// QList<PointRenderer> pointRenderers;   //!< stuff to render as points
	// QList<LineRenderer> lineRenderers;  //!< stuff to render as lines

	bool skybox;  //!< whether to render skybox
	QString skyboxFileBase;
	QString skyboxFileExtension;

	bool showBoundingBoxes;  //!< whether to show bounding boxes of entities - useful for debugging
	bool drawTerrainTileInfo;  //!< whether to draw extra information about terrain tiles to the textures - useful for debugging
};

