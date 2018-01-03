#include "maptexturegenerator.h"

#include <Qt3DRender/QTextureImageDataGenerator>
#include <QEventLoop>
#include <QImageReader>
#include <QTextOption>
#include <QPainter>

#include "maptexturegenerator.h"

MapTextureGenerator::MapTextureGenerator(const Map3D& map)
	: map(map)
	, lastJobId(0)
{
	// connect(nam, &QNetworkReply::finished, this, &MapTextureGenerator::onRenderingFinished);
}

int MapTextureGenerator::render(const Tile & tile, const QString & debugText)
{
	QNetworkAccessManager* nam = new QNetworkAccessManager();
	connect(nam, &QNetworkAccessManager::finished, this, &MapTextureGenerator::onRenderingFinished);

	QUrl url = baseUrl(tile);

	QNetworkRequest request(url);
	QNetworkReply* reply = nam->get(request);

	JobData jobData;
	jobData.jobId = ++lastJobId;
	jobData.job = reply;
	jobData.tile = tile;
	jobData.debugText = debugText;

	jobs.insert(reply, jobData);

	return jobData.jobId;
}

void MapTextureGenerator::cancelJob(int jobId)
{
	Q_FOREACH(const JobData& jd, jobs)
	{
		if (jd.jobId == jobId)
		{
			jd.job->abort();
			disconnect(jd.job, &QNetworkReply::finished, this, &MapTextureGenerator::onRenderingFinished);
			jd.job->deleteLater();
			jobs.remove(jd.job);
			return;
		}
	}
	Q_ASSERT(false && "requested job ID does not exist!");
}

QImage MapTextureGenerator::renderSynchronously(const Tile &tile, const QString &debugText)
{
	QNetworkAccessManager* nam = new QNetworkAccessManager();
	QEventLoop loop;

	connect(nam, &QNetworkAccessManager::finished, &loop, &QEventLoop::quit);

	QUrl url = baseUrl(tile);

	QNetworkRequest request(url);
	QNetworkReply* reply = nam->get(request);

	loop.exec();

	QImage img;

	if (reply->error() != QNetworkReply::OperationCanceledError)
	{
		// Did the reply return with errors...
		if (reply->error() != QNetworkReply::NoError)
		{
			// Log error.
			qDebug() << "Failed to download " << reply->url() << "' with error '" << reply->errorString();
		}
		else
		{
			// Log success.
			qDebug() << "Downloaded image " << reply->url();

			// Read image from reply
			QImageReader render(reply);
			img = render.read();
		}
	}

	if (!debugText.isEmpty())
	{
		// extra tile information for debugging
		QPainter p(&img);
		p.setPen(Qt::white);
		p.drawRect(0, 0, img.width() - 1, img.height() - 1);
		p.drawText(img.rect(), debugText, QTextOption(Qt::AlignCenter));
		p.end();
	}

	return img;
}

QUrl MapTextureGenerator::baseUrl(const Tile &tile)
{
	QString url = QString("https://api.mapbox.com/v4/mapbox.satellite/%3/%1/%2.png?access_token=pk.eyJ1IjoibnV0c3RpY2siLCJhIjoiY2o4aTh1anUxMTB2bTJ3bDlqYmo5ODJvaSJ9.YN8ymbV5tq9XsSHGflhblw")
		.arg(tile.x).arg(tile.y).arg(tile.z);
	return QUrl(url);
}

void MapTextureGenerator::onRenderingFinished(QNetworkReply* reply)
{
	Q_ASSERT(jobs.contains(reply));
	JobData jobData = jobs.value(reply);

	QImage img;
	if (reply->error() != QNetworkReply::OperationCanceledError)
	{
		// Did the reply return with errors...
		if (reply->error() != QNetworkReply::NoError)
		{
			// Log error.
			qDebug() << "Failed to download " << reply->url() << "' with error '" << reply->errorString();
		}
		else
		{
			// Log success.
			qDebug() << "Downloaded image " << reply->url();

			// Read image from reply
			QImageReader render(reply);
			img = render.read();
		}
	}

	if (!jobData.debugText.isEmpty())
	{
		// extra tile information for debugging
		QPainter p(&img);
		p.setPen(Qt::white);
		p.drawRect(0, 0, img.width() - 1, img.height() - 1);
		p.drawText(img.rect(), jobData.debugText, QTextOption(Qt::AlignCenter));
		p.end();
	}
}
