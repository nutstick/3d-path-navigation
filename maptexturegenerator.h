#pragma once

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QRect>
#include <QImage>
#include <QWaitCondition>
#include <QMutex>  

#include "tile.h"

class Map3D;

class MapTextureGenerator : public QObject
{
	Q_OBJECT
public:
	MapTextureGenerator(const Map3D& map);

	//! Start async rendering of a map for the given extent (must be a square!)
	//! Returns job ID
	int render(const Tile& tile, const QString& debugText = QString());

	//! Cancels a rendering job
	void cancelJob(int jobId);

	//! Render a map and return rendered image
	QImage renderSynchronously(const Tile& tile, const QString& debugText = QString());

signals:
	void tileReady(int jobId, const QImage& image);

private slots:
	void onRenderingFinished(QNetworkReply* reply);

private:
	QUrl baseUrl(const Tile& tile);

	const Map3D& map;

	struct JobData
	{
		int jobId;
		QNetworkReply* job;
		Tile tile;
		QString debugText;
	};

	QMutex syncPause;
	QWaitCondition pauseCond;

	QHash<QNetworkReply*, JobData> jobs;
	int lastJobId;
};

