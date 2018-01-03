#pragma once

#include <Qt3DRender/QAbstractTextureImage>

#include "tile.h"

class MapTextureGenerator;

class MapTextureImage : public Qt3DRender::QAbstractTextureImage
{
public:
	//! constructor that will generate image asynchronously
	MapTextureImage(MapTextureGenerator *mapGen, const Tile &tile, const QString &debugText = QString(), Qt3DCore::QNode *parent = nullptr);
	//! constructor that uses already prepared image
	MapTextureImage(const QImage &image, const Tile &tile, const QString &debugText, Qt3DCore::QNode *parent = nullptr);
	~MapTextureImage();

	virtual Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const override;

	private slots:
	void onTileReady(int jobId, const QImage &img);

signals:
	void textureReady();

private:
	MapTextureGenerator * mapGen;
	Tile tile;
	QString debugText;
	QImage img;
	int jobId;
	bool jobDone;
};

