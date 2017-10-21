#include "networkmanager.h"

#include <QMutexLocker>
#include <QImageReader>
#include <QDialog>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

NetworkManager::NetworkManager(QObject* parent) : QObject(parent)
{
    // Connect signal/slot to handle proxy authentication.
    QObject::connect(&m_nam, &QNetworkAccessManager::proxyAuthenticationRequired, this, &NetworkManager::proxyAuthenticationRequired);

    // Connect signal/slot to handle finished downloads.
    QObject::connect(&m_nam, &QNetworkAccessManager::finished, this, &NetworkManager::downloadFinished);
}

NetworkManager::~NetworkManager()
{
    // Ensure all download queues are aborted.
    abortDownloads();
}

void NetworkManager::setProxy(const QNetworkProxy& proxy)
{
    // Set the proxy on the network access manager.
    m_nam.setProxy(proxy);
}

void NetworkManager::abortDownloads()
{
    // Loop through each reply to abort and then remove it from the downloading image queue.
    QMutexLocker lock(&m_mutex_downloading_queue);
    QMutableMapIterator<QNetworkReply*, std::pair<QUrl, QSize>> itr(m_downloading_queue);
    while(itr.hasNext()) {
        // Tell the reply to abort.
        itr.next().key()->abort();

        // Remove it from the queue.
        itr.remove();
    }
}

int NetworkManager::downloadQueueSize() const
{
    // Default return value.
    int return_size(0);

    // Return the size of the downloading image queue.
    QMutexLocker lock(&m_mutex_downloading_queue);
    return_size += m_downloading_queue.size();

    // Return the size.
    return return_size;
}

bool NetworkManager::isDownloading(const QUrl& url, const QSize& size_px) const
{
    // Return whether we requested url is downloading image queue.
    QMutexLocker lock(&m_mutex_downloading_queue);
    return m_downloading_queue.values().contains(std::make_pair(url, size_px));
}

void NetworkManager::downloadImage(const QUrl& url, const QSize& size_px)
{
    // Keep track of our success.
    bool success(false);

    // Scope this as we later call "downloadQueueSize()" which also locks all download queue mutexes.
    {
        // Gain a lock to protect the downloading image container.
        QMutexLocker lock(&m_mutex_downloading_queue);

        // Check this is a new request.
        if(m_downloading_queue.values().contains(std::make_pair(url, size_px)) == false) {
            // Generate a new request.
            QNetworkRequest request(url);
            request.setRawHeader("User-Agent", "QWidgetMap");

            // Send the request.
            QNetworkReply* reply(m_nam.get(request));

            // Store the request into the downloading image queue.
            m_downloading_queue[reply] = std::make_pair(url, size_px);

            // Mark our success.
            success = true;

            // Log success.
            qDebug() << "Downloading image '" << url << "'";
        }
    }

    // Was we successful?
    if(success) {
        // Emit that we are downloading a new image (with details of the current queue size).
        emit downloadingInProgress(downloadQueueSize());
    }
}

void NetworkManager::proxyAuthenticationRequired(const QNetworkProxy& proxy, QAuthenticator* authenticator)
{
    // Log proxy authentication request.
    qDebug() << "Proxy Authentication Required for '" << proxy.hostName() << "' with the authenticator '" << &authenticator << "'";

    // We need to capture the proxy login details.
    // Setup a form to capture these details.
    QDialog dialog;
    QGridLayout layout;

    // Add username/password text fields.
    QLabel username, password;
    username.setText("Username:");
    password.setText("Password:");
    layout.addWidget(&username, 0, 0);
    layout.addWidget(&password, 1, 0);
    QLineEdit user, pass;
    pass.setEchoMode(QLineEdit::Password);
    layout.addWidget(&user, 0, 1);
    layout.addWidget(&pass, 1, 1);

    // Setup signals to exit the form when the return key is entered.
    QObject::connect(&user, &QLineEdit::returnPressed, &dialog, &QDialog::accept);
    QObject::connect(&pass, &QLineEdit::returnPressed, &dialog, &QDialog::accept);

    // Add an "OK" button.
    QPushButton button;
    button.setText("OK");
    layout.addWidget(&button, 2, 0, 1, 2, Qt::AlignCenter);

    // Setup signal to exit the form when the button is clicked.
    QObject::connect(&button, &QPushButton::clicked, &dialog, &QDialog::accept);

    // Set the layout to the dialog to display.
    dialog.setLayout(&layout);

    // Run the form.
    dialog.exec();

    // Set the form's username/password values into the authenticator to use.
    authenticator->setUser(user.text());
    authenticator->setPassword(pass.text());
}

void NetworkManager::downloadFinished(QNetworkReply* reply)
{
    // If the reply did not fail due to cancellation... (as cancellation locks our mutexes!)
    /// @note beware, do not lock downloading image mutex for abort operations.
    if(reply->error() != QNetworkReply::OperationCanceledError) {
        // Scope the work with the downloading image mutex.
        {
            // Is the reply in the downloading image queue?
            QMutexLocker lock(&m_mutex_downloading_queue);
            if(m_downloading_queue.contains(reply)) {
                // Did the reply return with errors...
                if(reply->error() != QNetworkReply::NoError) {
                    // Log error.
                    qDebug() << "Failed to download '" << reply->url() << "' with error '" << reply->errorString() << "'";

                    // Emit that we failed to download the image.
                    emit downloadFailed(reply->url());
                } else {
                    // Log success.
                    qDebug() << "Downloaded image '" << reply->url() << "'";

                    // Read in pixmap from reply.
                    QImageReader reader(reply);

                    QImage image = reader.read();

                    // Is the pixmap the required size?
                    if(image.size() != m_downloading_queue[reply].second) {
                        // Resize the pixmap to the required size.
                        image = image.scaled(m_downloading_queue[reply].second, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    }

                    // Emit that we have downloaded an image.
                    emit downloadedImage(m_downloading_queue[reply].first, image);
                }

                // Remove it from the downloading image queue.
                m_downloading_queue.remove(reply);
            }
        }

        // Check if the current download queue is empty.
        if(downloadQueueSize() == 0) {
            // Emit that all queued downloads have finished.
            emit downloadingFinished();
        }
    }
}
