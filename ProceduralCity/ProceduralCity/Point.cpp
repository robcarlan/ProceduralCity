#include "Point.h"

std::string Point::printPoint() {
	std::ostringstream stream;
	stream << "{ " << x() << ", " << y() << " }";
	return stream.str();
}

float Point::getDistanceSq(const Point p2) const {
	float x = p2.x() - this->x();
	float y = p2.y() - this->y();
	return (x*x + y*y);
}

Point::Point(QPointF val) : QPointF(val) {
}

Point::Point(float x, float y)
	: QPointF(x, y) {
}

Point::Point() : QPointF() {
}


Point::~Point() {
}
