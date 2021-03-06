#include "maptexturegenerator.h"

#include <QNetworkRequest>
#include <QEventLoop>
#include <QImageReader>

namespace
{
    std::unique_ptr<MapTextureGenerator> m_instance(nullptr);
}

MapTextureGenerator& MapTextureGenerator::get()
{
    if (m_instance == nullptr) {
        m_instance.reset(new MapTextureGenerator);
    }

    return *(m_instance.get());
}

void MapTextureGenerator::destroy()
{
    m_instance.reset(nullptr);
}

MapTextureGenerator::MapTextureGenerator(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager)
    , m_lastJobId(0)
{
     connect(m_nam, &QNetworkAccessManager::finished, this, &MapTextureGenerator::onFinished);
}

MapTextureGenerator::~MapTextureGenerator()
{
    Q_FOREACH(const JobData& jd, m_jobs)
    {
        jd.reply->abort();
        jd.reply->deleteLater();
        m_jobs.remove(jd.reply);
    }
}

int MapTextureGenerator::getTexture(QUrl url)
{
    QNetworkRequest request(url);
    QNetworkReply *reply = m_nam->get(request);

    JobData jobData;
    jobData.jobId = ++m_lastJobId;
    jobData.reply = reply;

    m_jobs.insert(reply, jobData);
    return jobData.jobId;
}

void MapTextureGenerator::cancelJob(int jobId)
{
    Q_FOREACH(const JobData& jd, m_jobs)
    {
        if (jd.jobId == jobId)
        {
            jd.reply->abort();
            jd.reply->deleteLater();
            m_jobs.remove(jd.reply);
            return;
        }
    }
    Q_ASSERT(false && "requested job ID doesn't exist!");
}

QImage MapTextureGenerator::getTextureSynchronously(QUrl url)
{
    QNetworkRequest request(url);

    QNetworkReply *reply = m_nam->get(request);

    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    if (reply->error() != QNetworkReply::OperationCanceledError) {
        // Did the reply return with errors...
        if(reply->error() != QNetworkReply::NoError) {
            // Log error.
            qDebug() << "Failed to download '" << reply->url() << "' with error '" << reply->errorString() << "'";
        } else {
            // Log success.
            qDebug() << "Downloaded image '" << reply->url() << "'";

            // Read image from reply
            QImageReader render(reply);
            QImage image = render.read();

            return image;
        }
    }

    return QImage();
}

void MapTextureGenerator::onFinished(QNetworkReply *reply)
{
    Q_ASSERT(m_jobs.contains(reply));

    // Read image from reply
    QImageReader render(reply);
    QImage image = render.read();

    JobData jd = m_jobs[reply];

    reply->deleteLater();
    m_jobs.remove(reply);

    emit finished(jd.jobId, image);
}
