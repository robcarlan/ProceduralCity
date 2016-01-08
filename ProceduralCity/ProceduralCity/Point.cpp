#include "Point.h"

std::string Point::printPoint() {
	std::ostringstream stream;
	stream << "{ " << x() << ", " << y() << " }";
	return stream.str();
}

float Point::getDistanceSq(const QPointF p2) const {
	float x = p2.x() - this->x();
	float y = p2.y() - this->y();
	return (x*x + y*y);
}

bool Point::isWithinBounds(const QPointF p2, float epsilon) {
	return (getDistanceSq(p2) < epsilon * epsilon);
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
