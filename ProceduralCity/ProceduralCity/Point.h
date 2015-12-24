#pragma once
#include <string>

#ifndef Q_MOC_RUN 
#include <boost\geometry\geometries\point.hpp>
#include <boost\geometry\geometry.hpp>
#endif

//Defines a 2d point in cartesian space
class Point :
	private boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian> {
public:
	std::string printPoint();

	float getX();
	float getY();

	void setX(float newX);
	void setY(float newY);

	Point(float x, float y);
	Point();
	~Point();
};
