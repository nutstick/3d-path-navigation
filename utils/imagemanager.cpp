#include "imagemanager.h"

namespace
{
    /// Singleton instance of Image Manager.
    std::unique_ptr<ImageManager> m_instance(nullptr);

    QString toString(const QSize& size_px)
    {
        return QString::number(size_px.width()) + "x" + QString::number(size_px.height());
    }
}

ImageManager& ImageManager::get()
{
    if (m_instance == nullptr) {
        m_instance.reset(new ImageManager);
    }

    return *(m_instance.get());
}

void ImageManager::destroy()
{
    m_instance.reset(nullptr);
}

ImageManager::ImageManager(QObject *parent)
    : QObject(parent)
    , m_failed_expiry(30)
    , m_persistent_cache(false)
    , m_persistent_cache_expiry(0)
{
    QObject::connect(this, &ImageManager::downloadImage, &m_nm, &NetworkManager::downloadImage);
    QObject::connect(&m_nm, &NetworkManager::downloadedImage, this, &ImageManager::imageDownloaded);
    QObject::connect(&m_nm, &NetworkManager::downloadFailed, this, &ImageManager::imageFailed);
    QObject::connect(&m_nm, &NetworkManager::downloadingInProgress, this, &ImageManager::downloadingInProgress);
    QObject::connect(&m_nm, &NetworkManager::downloadingFinished, this, &ImageManager::downloadingFinished);
}

void ImageManager::setProxy(const QNetworkProxy& proxy)
{
    m_nm.setProxy(proxy);
}

void ImageManager::setFailedExpiry(const std::chrono::seconds& failed_expiry)
{
    m_failed_expiry = failed_expiry;
}

bool ImageManager::enablePersistentCache(const std::chrono::minutes& expiry, const QDir& path)
{
    bool success(path.mkpath(path.absolutePath()));

    if (success) {
        m_persistent_cache_directory = path;
        m_persistent_cache_expiry = expiry;
        m_persistent_cache = true;
    } else {
        qDebug() << "Unable to create directory for persistent cache '" << path.absolutePath() << "'";
    }

    return success;
}

void ImageManager::abortLoading()
{
    m_nm.abortDownloads();

    m_current_failed_images.clear();
}

int ImageManager::loadQueueSize() const 
{
    return m_nm.downloadQueueSize();
}

QImage ImageManager::image(const QUrl& url, const QSize& size_px)
{
    QImage return_image(imageLoading(size_px));

    if (m_nm.isDownloading(url, size_px) == false) {
        const auto itr_find(m_image_cache.find(md5hex(url, size_px)));

        if (itr_find != m_image_cache.end()) {
            return_image = itr_find->second;
        } else if (m_persistent_cache && persistentCacheFind(url, size_px, return_image)) {
            m_image_cache[md5hex(url, size_px)] = return_image;
        } else if (m_current_failed_images.contains(url)) {
            if (m_current_failed_images.value(url).secsTo(QDateTime::currentDateTimeUtc()) < m_failed_expiry.count()) {
                return_image = imageLoadFailed(size_px);
            } else {
                m_current_failed_images.remove(url);

                // Emit
                emit downloadImage(url, size_px);
            }
        }
    } else {
        emit downloadImage(url, size_px);
    }

    return return_image;
}

QImage ImageManager::prefetchImage(const QUrl& url, const QSize& size_px)
{
    m_prefetch_urls.append(url);

    return image(url, size_px);
}

void ImageManager::imageDownloaded(const QUrl& url, const QImage& image)
{
    m_image_cache[md5hex(url, image.size())] = image;

    if (m_persistent_cache) {
        persistentCacheInsert(url, image);
    }

    if (m_prefetch_urls.contains(url)) {
        m_prefetch_urls.removeAt(m_prefetch_urls.indexOf(url));
    } else {
        emit imageUpdated(url);
    }
}

void ImageManager::imageFailed(const QUrl& url)
{
    qDebug() << "ImageManager::imagedFailed '" << url << "'";

    m_current_failed_images[url] = QDateTime::currentDateTimeUtc();

    emit imageUpdated(url);
}

QImage ImageManager::imageLoading(const QSize& size_px)
{
    if (m_image_loading.find(toString(size_px)) == m_image_loading.end()) {
        QImage image(size_px, QImage::Format_RGB32);

        // TODO:
        m_image_loading[toString(size_px)] = image;
    }

    return m_image_loading[toString(size_px)];
}

QImage ImageManager::imageLoadFailed(const QSize& size_px)
{
    if (m_image_failed.find(toString(size_px)) == m_image_failed.end()) {
        QImage image(size_px, QImage::Format_RGB32);

        m_image_failed[toString(size_px)] = image;
    }

    return m_image_failed[toString(size_px)];
}

QString ImageManager::md5hex(const QUrl& url, const QSize& size_px)
{
    return toString(size_px) + "_" + QString(QCryptographicHash::hash(url.toString().toUtf8(), QCryptographicHash::Md5).toHex());
}

QString ImageManager::persistentCacheFilename(const QUrl& url, const QSize& size_px)
{
    return m_persistent_cache_directory.absolutePath() + QDir::separator() + md5hex(url, size_px);
}

bool ImageManager::persistentCacheFind(const QUrl& url, const QSize& size_px, QImage& return_image)
{
    bool success(false);

    QFile file(persistentCacheFilename(url, size_px));

    if (file.exists()) {
        QFileInfo file_info(file);

        if (m_persistent_cache_expiry.count() > 0 && file_info.lastModified().msecsTo(QDateTime::currentDateTime()) > std::chrono::duration_cast<std::chrono::milliseconds>(m_persistent_cache_expiry).count()) {
            m_persistent_cache_directory.remove(file.fileName());

            qDebug() << "Removing '" << file.fileName() << "' from persistent cache for url '" << url << "'";
        } else {
            success = return_image.load(persistentCacheFilename(url, size_px));
        }
    }

    return success;
}

bool ImageManager::persistentCacheInsert(const QUrl& url, const QImage& image)
{
    // Return the result of saving the pixmap to the persistent cache.
    return image.save(persistentCacheFilename(url, image.size()), "PNG");
}
