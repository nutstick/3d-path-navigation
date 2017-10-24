#include "maprendererjob.h"
#include <QImageReader>
#include <QEventLoop>

MapRendererJob::MapRendererJob(QObject *parent = nullptr)
    : QObject(parent)
    , m_nam(this)
{
    connect(m_nam, &QNetworkAccessManager::finished, this, &MapRendererJob::handleRequestFinished);
}

MapRendererJob::~MapRendererJob()
{
}

void MapRendererJob::start(QUrl url)
{
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "QWidgetMap");

    m_reply = m_nam.get(request);

    // Log success
    qDebug() << "Downloading image "
}

void MapRendererJob::waitForFinished()
{
    QEventLoop loop;
    connect(m_reply, &QNetworkReply::finished(), &loop, &QEventLoop::quit());
    loop.exec();
}

void MapRendererJob::cancelWithoutBlocking()
{
    m_reply->abort();
}

void MapRendererJob::getImage()
{
    return m_image;
}

void MapRendererJob::handleRequestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::OperationCanceledError) {
        if (reply->error() != QNetworkReply::NoError) {
            qDebug() << "Failed to download the image from " << reply->url();

            emit failed();
        } else {
            qDebug() << "Downloaded image from " << reply->url();
            QImageReader reader(reply);

            m_image = render.read();
            emit finished();
        }
    }
}
