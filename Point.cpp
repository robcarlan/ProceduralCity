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

float Point::getDistance(const QPointF p2) const {
	float x = p2.x() - this->x();
	float y = p2.y() - this->y();
	return sqrtf(x*x + y*y);
}

bool Point::isWithinBounds(const QPointF p2, float epsilon) const {
	return (getDistanceSq(p2) < epsilon * epsilon);
}

bool Point::isWithinBounds(int xSize, int ySize) const {
	return (0 <= x() && x() < xSize) && (0 <= y() && y() < ySize);
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

float Point::getManhattanDist(const QPointF p2) const {
	return (abs(p2.y() - y()) + abs(p2.x() - x()));
}
