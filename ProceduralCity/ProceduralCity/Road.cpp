#include "Road.h"

const float Road::PEN_WIDTH = 25.0f;

Road::Road(Point start, Point end) : QGraphicsItem() {
	this->start = start;
	this->end = end;

	//calculate bounds
	bounds = boundsFromPointPair(start, end);
}

QRectF Road::boundingRect() const {
	return bounds;
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	//Switch pen based on type
	painter->drawLine(start.getX(), start.getY(), end.getX(), end.getY());
}

QRectF Road::boundsFromPointPair(Point s, Point e) {
	float minX = std::min(s.getX(), e.getX());
	float maxX = std::max(s.getX(), e.getX());
	float minY = std::min(s.getY(), e.getY());
	float maxY= std::max(s.getY(), e.getY());

	return QRectF(minX, minY, maxX - minX, maxY - minY);
}


Road::~Road() {
}

StraightRoad::StraightRoad(Point start, Point end) : Road(start, end) {
}

std::string StraightRoad::printRoad() {
	return std::string("Road begins at " + this->getStart().printPoint() + " and ends at " + this->getEnd().printPoint());
}

Point Road::getStart() { return Road::start; }
Point Road::getEnd() { return Road::end; }



