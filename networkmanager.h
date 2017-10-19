#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QObject>
#include <QMutex>
#include <QUrl>
#include <QImage>
#include <QAuthenticator>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkProxy>

class NetworkManager : public QObject
{
    Q_OBJECT
public:
    explicit NetworkManager(QObject *parent = nullptr);

    NetworkManager(const NetworkManager&) = delete;

    NetworkManager& operator = (const NetworkManager&) = delete;

    ~NetworkManager();

public:
    void setProxy(const QNetworkProxy& proxy);

    void abortDownloads();

    int downloadQueueSize() const;

    bool isDownloading(const QUrl& url, const QSize& size_px) const;

public slots:
    void downloadImage(const QUrl& url, const QSize& size_px);

private slots:
    void proxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator);

    void downloadFinished(QNetworkReply* reply);

signals:
    void downloadingInProgress(const int& count);

    void downloadingFinished();

    void downloadedImage(const QUrl& url, const QImage& image);

    void downloadFailed(const QUrl& url);

private:
    QNetworkAccessManager m_nam;

    mutable QMutex m_mutex_downloading_queue;

    QMap<QNetworkReply*, std::pair<QUrl, QSize>> m_downloading_queue;
};

#endif // NETWORKMANAGER_H
