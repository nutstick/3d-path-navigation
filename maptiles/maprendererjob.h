#ifndef MAPRENDERERJOB_H
#define MAPRENDERERJOB_H

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QImage>

class MapRendererJob : public QObject
{
    Q_OBJECT

public:
    explicit MapRendererJob(QObject *parent = nullptr);
    ~MapRendererJob();

    void start();

    void waitForFinished();

    void cancelWithoutBlocking();

    QImage getImage();

signals:
    void finished();
    void failed();

private slots:
    void handleRequestFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_nam;
    QNetworkReply *m_reply;
    QImage m_image;
};

#endif // MAPRENDERERJOB_H
