#ifndef MAPTEXTUREIMAGE_H
#define MAPTEXTUREIMAGE_H

#include <QImage>
#include <QUrl>
#include <QAbstractTextureImage>

#include "maptexturegenerator.h"

class MapTextureImage : public Qt3DRender::QAbstractTextureImage
{
    Q_OBJECT
public:
    MapTextureImage(const QUrl url, Qt3DCore::QNode *parent = nullptr);
    MapTextureImage(const QImage& image, Qt3DCore::QNode *parent = nullptr);
    ~MapTextureImage();

    virtual Qt3DRender::QTextureImageDataGeneratorPtr dataGenerator() const override;

private slots:
    void onTextureReady(int jobId, const QImage& image);

signals:
    void textureReady();

private:
    MapTextureGenerator *m_mapTextureGenerator;
    QImage m_img;

    int m_jobId;
    bool m_jobFinished;
};

#endif // MAPTEXTUREIMAGE_H
