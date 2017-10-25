#include "maptextureimage.h"

#include <QTextureImageDataGenerator>

class MapTextureImageDataGenerator : public Qt3DRender::QTextureImageDataGenerator
{
public:
    QImage m_img;

    static QImage placeholderImage()
    {
        QImage i(2, 2, QImage::Format_RGB32);
        i.setPixelColor(0, 0, Qt::darkGray);
        i.setPixelColor(1, 0, Qt::lightGray);
        i.setPixelColor(0, 1, Qt::lightGray);
        i.setPixelColor(1, 1, Qt::darkGray);

        return i;
    }

    MapTextureImageDataGenerator(const QImage& img)
        : m_img(img) {}

    virtual Qt3DRender::QTextureImageDataPtr operator ()() override
    {
        Qt3DRender::QTextureImageDataPtr dataPtr = Qt3DRender::QTextureImageDataPtr::create();

        dataPtr->setImage(m_img.isNull() ? placeholderImage() : m_img);

        return dataPtr;
    }

    virtual bool operator ==(const QTextureImageDataGenerator &other) const override
    {
        const MapTextureImageDataGenerator *otherFunctor = functor_cast<MapTextureImageDataGenerator>(&other);
        return otherFunctor != nullptr && otherFunctor->m_img.isNull() == m_img.isNull();
    }

    QT3D_FUNCTOR(MapTextureImageDataGenerator)
};

MapTextureImage::MapTextureImage(const QUrl url, Qt3DCore::QNode *parent)
    : Qt3DRender::QAbstractTextureImage(parent)
    , m_jobId(0)
    , m_jobFinished(false)
{
    m_mapTextureGenerator = &MapTextureGenerator::get();
    connect(m_mapTextureGenerator, &MapTextureGenerator::finished, this, &MapTextureImage::onTextureReady);

    m_jobId = m_mapTextureGenerator->getTexture(url);
}

MapTextureImage::MapTextureImage(const QImage &image, Qt3DCore::QNode *parent)
    : Qt3DRender::QAbstractTextureImage(parent)
    , m_img(image)
    , m_jobFinished(true)
{
}

MapTextureImage::~MapTextureImage()
{
    if (!m_jobFinished) {
        m_mapTextureGenerator->cancelJob(m_jobId);
    }
}

Qt3DRender::QTextureImageDataGeneratorPtr MapTextureImage::dataGenerator() const
{
    return Qt3DRender::QTextureImageDataGeneratorPtr(new MapTextureImageDataGenerator(m_img));
}

void MapTextureImage::onTextureReady(int jobId, const QImage &image)
{
    if (jobId == this->m_jobId)
    {
        this->m_img = image;
        this->m_jobFinished = true;
        notifyDataGeneratorChanged();
        emit textureReady();
    }
}
