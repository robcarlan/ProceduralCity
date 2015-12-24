#include "Point.h"

std::string Point::printPoint() {
	float x = this->get<0>();
	float y = this->get<1>();

	std::ostringstream stream;
	stream << "{ " << x << ", " << y << " }";
	return stream.str();
}

float Point::getX() {
	return this->get<0>();
}

float Point::getY() {
	return this->get<1>();
}

void Point::setX(float newX) {
	this->set<0>(newX);
}

void Point::setY(float newY) {
	this->set<1>(newY);
}

Point::Point(float x, float y) 
	: boost::geometry::model::point<float, 2, boost::geometry::cs::cartesian>(x, y) {
}

Point::Point() {
}


Point::~Point() {
}
