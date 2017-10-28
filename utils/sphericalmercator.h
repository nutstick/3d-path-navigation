#ifndef SPHERICALMERCATOR_H
#define SPHERICALMERCATOR_H

#include <QtMath>
#include <QPair>
#include <QVector>

class SphericalMercator
{
public:
    struct TileBoxBounds {
        int minX;
        int maxX;
        int minY;
        int maxY;
    };

    struct BBox {
        int w;
        int s;
        int e;
        int n;
    };

    enum Srs {
        WGS84,
        Srs900913,
    };

    SphericalMercator(float size);

    ~SphericalMercator();

    BBox bbox(float x, float y, int zoom, bool tmsStyle, Srs srs);
    TileBoxBounds xyz(BBox bbox, int zoom, bool tmsStyle, Srs srs);

    BBox convert(BBox bbox, Srs targetFormat);

private:
    QPair<float, float> px(float longitude, float latitude, int zoom);

    QPair<float, float> ll(float x, float y, int zoom);

    QPair<float, float> forward(float longitude, float latitude);

    QPair<float, float> inverse(float x, float y);

    const float EPSLN = 1.0e-10;
    const float D2R = M_PI / 180.0f;
    const float R2D = 180.0f / M_PI;
    const float A = 6378137;
    const float MAXEXTENT = 20037508.34;

private:
    int m_size;
    QVector<float> Bc;
    QVector<float> Cc;
    QVector<float> zc;
    QVector<float> Ac;
};

#endif // SPHERICALMERCATOR_H
