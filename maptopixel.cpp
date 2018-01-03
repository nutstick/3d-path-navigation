#include "maptopixel.h"

#include <QTextStream>
#include <QVector>
#include <QTransform>

MapToPixel::MapToPixel(double mapUnitsPerPixel, double centerX, double centerY, int widthPixels, int heightPixels, double rotation)
	: mMapUnitsPerPixel(mapUnitsPerPixel)
	, mWidth(widthPixels)
	, mHeight(heightPixels)
	, mRotation(rotation)
	, mXCenter(centerX)
	, mYCenter(centerY)
	, xMin(centerX - (mWidth * mMapUnitsPerPixel / 2.0))
	, yMin(centerY - (mHeight * mMapUnitsPerPixel / 2.0))
{
	Q_ASSERT(mapUnitsPerPixel > 0);
	updateMatrix();
}

MapToPixel::MapToPixel(double mapUnitsPerPixel)
	: mMapUnitsPerPixel(mapUnitsPerPixel)
	, mWidth(0)
	, mHeight(0)
	, mXCenter(0)
	, mYCenter(0)
{
	updateMatrix();
}

MapToPixel MapToPixel::fromScale(double scale, double dpi)
{
	double metersPerPixel = 25.4 / dpi / 1000.0;
	// TODO: Non Meters support
	// double mapUnitsPerPixel = metersPerPixel * QgsUnitTypes::fromUnitToUnitFactor( QgsUnitTypes::DistanceMeters, mapUnits );
	return MapToPixel(metersPerPixel * scale);
}

MapToPixel::MapToPixel()
{
	updateMatrix();
}

QPoint MapToPixel::transform(const QPoint & p) const
{
	qreal x = p.x(), y = p.y();
	transformInPlace(x, y);
	// QgsDebugMsg(QString("Point to pixel...X : %1-->%2, Y: %3 -->%4").arg(p.x()).arg(dx).arg(p.y()).arg(dy));
	return QPoint(x, y);
}

void MapToPixel::transform(QPoint * p) const
{
	qreal x = p->x(), y = p->y();
	transformInPlace(x, y);
	// QgsDebugMsg(QString("Point to pixel...X : %1-->%2, Y: %3 -->%4").arg(p->x()).arg(x).arg(p->y()).arg(y));
	p->setX(x);
	p->setY(y);
}

QPoint MapToPixel::transform(qreal x, qreal y) const
{
	transformInPlace(x, y);
	return QPoint(x, y);
}

void MapToPixel::transformInPlace(double & x, double & y) const
{
	// Map 2 Pixel
	qreal mx, my;
	qreal x_qreal = x, y_qreal = y;
	mMatrix.map(x_qreal, y_qreal, &mx, &my);
	//QgsDebugMsg(QString("XXX transformInPlace X : %1-->%2, Y: %3 -->%4").arg(x).arg(mx).arg(y).arg(my));
	x = mx;
	y = my;
}

void MapToPixel::transformInPlace(float & x, float & y) const
{
	double mx = x, my = y;
	transformInPlace(mx, my);
	x = mx;
	y = my;
}

QPoint MapToPixel::toMapCoordinates(int x, int y) const
{
	return toMapPoint(x, y);
}

QPoint MapToPixel::toMapCoordinatesF(double x, double y) const
{
	return toMapPoint(x, y);
}

QPoint MapToPixel::toMapCoordinates(QPoint p) const
{
	QPoint mapPt = toMapPoint(p.x(), p.y());
	return QPoint(mapPt);
}

QPoint MapToPixel::toMapPoint(double x, double y) const
{
	bool invertible;
	QTransform matrix = mMatrix.inverted(&invertible);
	Q_ASSERT(invertible);
	qreal mx, my;
	qreal x_qreal = x, y_qreal = y;
	matrix.map(x_qreal, y_qreal, &mx, &my);
	//qDebug() < QString("XXX toMapPoint x:%1 y:%2 -> x:%3 y:%4").arg(x).arg(y).arg(mx).arg(my);

	return QPoint(mx, my);
}

void MapToPixel::setMapUnitsPerPixel(double mapUnitsPerPixel)
{
	double oldUnits = mMapUnitsPerPixel;
	mMapUnitsPerPixel = mapUnitsPerPixel;
	if (!updateMatrix())
	{
		mMapUnitsPerPixel = oldUnits;
	}
}

double MapToPixel::mapUnitsPerPixel() const
{
	return mMapUnitsPerPixel;;
}

int MapToPixel::mapWidth() const
{
	return mWidth;
}

int MapToPixel::mapHeight() const
{
	return mHeight;
}

void MapToPixel::setMapRotation(double degrees, double cx, double cy)
{
	double oldRotation = mRotation;
	double oldXCenter = mXCenter;
	double oldYCenter = mYCenter;
	double oldWidth = mWidth;

	mRotation = degrees;
	mXCenter = cx;
	mYCenter = cy;
	if (mWidth < 0)
	{
		// set width not that we can compute it
		mWidth = ((mXCenter - xMin) * 2) / mMapUnitsPerPixel;
	}

	if (!updateMatrix())
	{
		mRotation = oldRotation;
		mXCenter = oldXCenter;
		mYCenter = oldYCenter;
		mWidth = oldWidth;
	}
}

double MapToPixel::mapRotation() const
{
	return mRotation;
}

void MapToPixel::setParameters(double mapUnitsPerPixel, double centerX, double centerY, int widthPixels, int heightPixels, double rotation)
{
	double oldMUPP = mMapUnitsPerPixel;
	double oldXCenter = mXCenter;
	double oldYCenter = mYCenter;
	double oldWidth = mWidth;
	double oldHeight = mHeight;
	double oldRotation = mRotation;
	double oldXMin = xMin;
	double oldYMin = yMin;

	mMapUnitsPerPixel = mapUnitsPerPixel;
	mXCenter = centerX;
	mYCenter = centerY;
	mWidth = widthPixels;
	mHeight = heightPixels;
	mRotation = rotation;
	xMin = centerX - (mWidth * mMapUnitsPerPixel / 2.0);
	yMin = centerY - (mHeight * mMapUnitsPerPixel / 2.0);

	if (!updateMatrix())
	{
		mMapUnitsPerPixel = oldMUPP;
		mXCenter = oldXCenter;
		mYCenter = oldYCenter;
		mWidth = oldWidth;
		mHeight = oldHeight;
		mRotation = oldRotation;
		xMin = oldXMin;
		yMin = oldYMin;
	}
}

QString MapToPixel::showParameters() const
{
	QString rep;
	QTextStream(&rep) << "Map units/pixel: " << mMapUnitsPerPixel
		<< " center: " << mXCenter << ',' << mYCenter
		<< " rotation: " << mRotation
		<< " size: " << mWidth << 'x' << mHeight;
	return rep;
}

QTransform MapToPixel::transform() const
{
	// NOTE: operations are done in the reverse order in which
	//       they are configured, so translation to geographical
	//       center happens first, then scaling, then rotation
	//       and finally translation to output viewport center

	double rotation = mapRotation();
	if (qAbs(rotation - 0.0) <= 0.00001)
	{
		//no rotation, return a simplified matrix
		return QTransform::fromScale(1.0 / mMapUnitsPerPixel, -1.0 / mMapUnitsPerPixel)
			.translate(-xMin, -(yMin + mHeight * mMapUnitsPerPixel));
	}
	else
	{
		double cy = mapHeight() / 2.0;
		double cx = mapWidth() / 2.0;
		return QTransform::fromTranslate(cx, cy)
			.rotate(rotation)
			.scale(1 / mMapUnitsPerPixel, -1 / mMapUnitsPerPixel)
			.translate(-mXCenter, -mYCenter);
	}
}

bool MapToPixel::updateMatrix()
{
	QTransform newMatrix = transform();

	// https://issues.qgis.org/issues/12757
	if (!newMatrix.isInvertible())
		return false;

	mMatrix = newMatrix;
	return true;
}
