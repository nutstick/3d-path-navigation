#pragma once

#include <QTransform>
#include <vector>

class QPoint;

class MapToPixel
{
public:
	MapToPixel(double mapUnitsPerPixel, double centerX, double centerY, int widthPixels, int heightPixels, double rotation);

	MapToPixel(double mapUnitsPerPixel);

	static MapToPixel fromScale(double scale, /* QgsUnitTypes::DistanceUnit mapUnits */ double dpi = 96);

	MapToPixel();

	/**
	* Transform the point from map (world) coordinates to device coordinates
	* \param p Point to transform
	* \returns QgsPointXY in device coordinates
	*/
	QPoint transform(const QPoint &p) const;

	void transform(QPoint *p) const;

	/**
	* Transform the point specified by x,y from map (world)
	* coordinates to device coordinates
	* \param x x cordinate o point to transform
	* \param y y coordinate of point to transform
	* \returns QgsPointXY in device coordinates
	*/
	QPoint transform(qreal x, qreal y) const;

	/**
	* Transform device coordinates to map coordinates. Modifies the
	* given coordinates in place. Intended as a fast way to do the
	* transform.
	*/
	void transformInPlace(double &x, double &y) const;

	void transformInPlace(float &x, float &y) const;

	template <class T>
	void transformInPlace(QVector<T> &x, QVector<T> &y) const
	{
		Q_ASSERT(x.size() == y.size());
		for (int i = 0; i < x.size(); ++i)
			transformInPlace(x[i], y[i]);
	}

	QPoint toMapCoordinates(int x, int y) const;

	//! Transform device coordinates to map (world) coordinates
	QPoint toMapCoordinatesF(double x, double y) const;

	/**
	* Transform device coordinates to map (world) coordinates
	* \param p Point to be converted to map cooordinates
	* \returns QgsPointXY in map coorndiates
	*/
	QPoint toMapCoordinates(QPoint p) const;

	QPoint toMapPoint(double x, double y) const;

	/**
	* Set map units per pixel
	* \param mapUnitsPerPixel Map units per pixel
	*/
	void setMapUnitsPerPixel(double mapUnitsPerPixel);

	//! Return current map units per pixel
	double mapUnitsPerPixel() const;

	/**
	* Return current map width in pixels
	* The information is only known if setRotation was used
	* \since QGIS 2.8
	*/
	int mapWidth() const;

	/**
	* Return current map height in pixels
	* \since QGIS 2.8
	*/
	int mapHeight() const;

	/**
	* Set map rotation in degrees (clockwise)
	* \param degrees clockwise rotation in degrees
	* \param cx X ordinate of map center in geographical units
	* \param cy Y ordinate of map center in geographical units
	* \since QGIS 2.8
	*/
	void setMapRotation(double degrees, double cx, double cy);

	/**
	* Return current map rotation in degrees
	* \since QGIS 2.8
	*/
	double mapRotation() const;

	/**
	* Set parameters for use in transforming coordinates
	* \param mapUnitsPerPixel Map units per pixel
	* \param centerX X coordinate of map center, in geographical units
	* \param centerY Y coordinate of map center, in geographical units
	* \param widthPixels Output width, in pixels
	* \param heightPixels Output height, in pixels
	* \param rotation clockwise rotation in degrees
	* \since QGIS 2.8
	*/
	void setParameters(double mapUnitsPerPixel, double centerX, double centerY, int widthPixels, int heightPixels, double rotation);

	//! String representation of the parameters used in the transform
	QString showParameters() const;

	QTransform transform() const;

	/**
	* Returns the center x-coordinate for the transform.
	* \see yCenter()
	* \since QGIS 3.0
	*/
	double xCenter() const { return mXCenter; }

	/**
	* Returns the center y-coordinate for the transform.
	* \see xCenter()
	* \since QGIS 3.0
	*/
	double yCenter() const { return mYCenter; }

private:
	double mMapUnitsPerPixel = 1;
	int mWidth = 1;
	int mHeight = 1;
	double mRotation = 0.0;
	double mXCenter = 0.5;
	double mYCenter = 0.5;
	double xMin = 0;
	double yMin = 0;
	QTransform mMatrix;

	bool updateMatrix();
};

