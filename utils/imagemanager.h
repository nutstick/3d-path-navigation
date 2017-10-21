#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#pragma once

#include <QDir>
#include <QObject>
#include <QList>
#include <QUrl>
#include <QImage>

#include <chrono>
#include <map>
#include <memory>

#include "networkmanager.h"

class ImageManager : public QObject
{
    Q_OBJECT
public:
    static ImageManager& get();

    static void destroy();

private:
    explicit ImageManager(QObject *parent = nullptr);

public:
    ImageManager(const ImageManager&) = delete;

    ImageManager& operator=(const ImageManager&) = delete;

    ~ImageManager() = default;

public:
    void setProxy(const QNetworkProxy& proxy);
    
    void setFailedExpiry(const std::chrono::seconds& failed_expiry = std::chrono::seconds(30));

    bool enablePersistentCache(const std::chrono::minutes& expiry, const QDir& path);

    void abortLoading();

    int loadQueueSize() const;

    QImage image(const QUrl& url, const QSize& size_px);

    QImage prefetchImage(const QUrl& url, const QSize& size_px);

private slots:
    void imageDownloaded(const QUrl& url, const QImage& image);

    void imageFailed(const QUrl& url);

private:
    QImage imageLoading(const QSize& size_px);

    QImage imageLoadFailed(const QSize& size_px);

    QString md5hex(const QUrl& url, const QSize& size_px);

    QString persistentCacheFilename(const QUrl& url, const QSize& size_px);

    bool persistentCacheFind(const QUrl& url, const QSize& size_px, QImage& return_image);

    bool persistentCacheInsert(const QUrl& url, const QImage& image);

signals:
    void downloadImage(const QUrl& url, const QSize& size_px);

    void downloadingInProgress(const int& count);

    void downloadingFinished();

    void imageUpdated(const QUrl& url);

private:
    NetworkManager m_nm;

    std::map<QString, QImage> m_image_cache;

    std::map<QString, QImage> m_image_loading;

    std::map<QString, QImage> m_image_failed;

    std::chrono::seconds m_failed_expiry;

    QList<QUrl> m_prefetch_urls;

    bool m_persistent_cache;

    QDir m_persistent_cache_directory;

    std::chrono::minutes m_persistent_cache_expiry;

    QMap<QUrl, QDateTime> m_current_failed_images;

    int size_px = 256;
};

#endif // IMAGEMANAGER_H
