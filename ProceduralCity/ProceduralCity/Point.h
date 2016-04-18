#pragma once
#include <string>
#include <qline.h>

#ifndef Q_MOC_RUN 
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometry.hpp>
#endif

//Defines a 2d point in cartesian space
class Point :
	//private boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> {
	public QPointF {
public:
	std::string printPoint();

	float getDistanceSq(const QPointF p2) const;
	float getDistance(const QPointF p2) const;
	float getManhattanDist(const QPointF p2) const;
	bool isWithinBounds(const QPointF p2, float epsilon) const;
	bool isWithinBounds(int xSize, int ySize) const;

	Point(QPointF val);
	Point(float x, float y);
	Point();
	~Point();
};
