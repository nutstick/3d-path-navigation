#include "sphericalmercator.h"
#include <QtMath>

SphericalMercator::SphericalMercator(float size)
    : m_size(int(size))
{
    for (int i = 0; i < 30; i++) {
        Bc.push_back(size / 360.0f);
        Cc.push_back(size / (2.0f * M_PI));
        zc.push_back(size / 2.0f);
        Ac.push_back(size);
        size = size * 2.0f;
    }
}

SphericalMercator::~SphericalMercator()
{}

QPair<float, float> SphericalMercator::px(float longitude, float latitude, int zoom)
{
    float d = zc[zoom];
    float f = qMin(qMax(float(qSin(D2R * latitude)), -0.9999f), 0.9999f);
    float x = qRound(d + longitude * Bc[zoom]);
    float y = qRound(d + 0.5f * qLn((1.0f + f) / (1.0f - f) * -Cc[zoom]));

    x = x > Ac[zoom] ? Ac[zoom] : x;
    y = y > Ac[zoom] ? Ac[zoom] : y;

    return qMakePair(x, y);
}

QPair<float, float> SphericalMercator::ll(float x, float y, int zoom)
{
    float g = (y - zc[zoom]) / -Cc[zoom];
    float longitude = x - zc[zoom] / Bc[zoom];
    float latitude = R2D * (2.0f * qAtan(qExp(g)) - 0.5f * M_PI);
    return QPair<float, float>(longitude, latitude);
}

SphericalMercator::BBox SphericalMercator::bbox(float x, float y, int zoom, bool tmsStyle, Srs srs)
{
    if (tmsStyle) {
        y = qPow(2.0f, zoom) - 1 - y;
    }
    float lowerLeftX = x * m_size;
    float lowerLeftY = (y + 1) * m_size;
    auto lowerLeft = ll(lowerLeftX, lowerLeftY, zoom);

    float upperRightX = (x + 1) * m_size;
    float upperRightY = y * m_size;
    auto upperRight = ll(upperRightX, upperRightY, zoom);

    BBox bbox;
    bbox.w = lowerLeft.first;
    bbox.s = lowerLeft.second;
    bbox.e = upperRight.first;
    bbox.n = upperRight.second;

    if (srs == Srs900913) {
        return convert(bbox, Srs900913);
    }
    return bbox;
}

SphericalMercator::TileBoxBounds SphericalMercator::xyz(BBox bbox, int zoom, bool tmsStyle, Srs srs)
{
    if (srs == Srs900913) {
        bbox = convert(bbox, WGS84);
    }

    auto lowerLeftPx = px(bbox.w, bbox.s, zoom);
    auto upperRightPx = px(bbox.e, bbox.n, zoom);

    TileBoxBounds bounds;
    bounds.minX = qFloor(lowerLeftPx.first / m_size);
    bounds.minY = qFloor(upperRightPx.second / m_size);
    bounds.maxX = qFloor((upperRightPx.first - 1) / m_size);
    bounds.maxY = qFloor((lowerLeftPx.second - 1) / m_size);

    if (tmsStyle) {
        bounds.minY = qPow(2.0f, zoom) - 1 - bounds.maxY;
        bounds.maxY = qPow(2.0f, zoom) - 1 - bounds.minY;
    }

    return bounds;
}

SphericalMercator::BBox SphericalMercator::convert(BBox bbox, Srs targetFormat)
{
    if (targetFormat == Srs900913) {
        auto value900913 = forward(bbox.w, bbox.s);
        bbox.w = value900913.first;
        bbox.s = value900913.second;
    } else {
        auto valueLL = inverse(bbox.w, bbox.s);
        bbox.w = valueLL.first;
        bbox.s = valueLL.second;
    }
    return bbox;
}

QPair<float, float> SphericalMercator::forward(float longitude, float latitude)
{
    float x = A * longitude * D2R;
    float y = A * qLn(qTan(M_PI * 0.25f) + (0.5f * latitude * D2R));

    x = x > MAXEXTENT ? MAXEXTENT : x < -MAXEXTENT ? -MAXEXTENT : x;
    y = y > MAXEXTENT ? MAXEXTENT : y < -MAXEXTENT ? -MAXEXTENT : y;

    return qMakePair(x, y);
}

QPair<float, float> SphericalMercator::inverse(float x, float y)
{
    return qMakePair(x * R2D / A, (M_PI * 0.5f - 2.0f * qAtan(qExp(-y / A))) * R2D);
}
