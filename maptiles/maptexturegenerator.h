#ifndef MAPTEXTUREGENERATOR_H
#define MAPTEXTUREGENERATOR_H

#include <QObject>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>

class MapTextureGenerator : public QObject
{
    Q_OBJECT
public:
    static MapTextureGenerator& get();

    static void destroy();

private:
    MapTextureGenerator(QObject *parent = nullptr);

    ~MapTextureGenerator();

public:
    int getTexture(QUrl url);

    void cancelJob(int jobId);

    QImage getTextureSynchronously(QUrl url);

signals:
    void finished(int jobId, const QImage& image);

private slots:
    void onFinished(QNetworkReply *reply);

private:
    struct JobData
    {
        int jobId;
        QNetworkReply* reply;
    };

    QNetworkAccessManager *m_nam;
    QHash<QNetworkReply*, JobData> m_jobs;
    int m_lastJobId;
};

#endif // MAPTEXTUREGENERATOR_H
