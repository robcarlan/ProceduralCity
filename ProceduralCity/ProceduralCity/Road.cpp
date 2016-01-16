#include "Road.h"

const float Road::PEN_WIDTH = 25.0f;

Road::Road(Point start, Point end, roadType rType) : QGraphicsItem(), QLineF(start, end) {
	this->setP1(start);
	this->setP2(end);
	this->start = start;
	this->end = end;
	this->rType = rType;

	//calculate bounds
	bounds = boundsFromPointPair(start, end);
}

Road::Road(const Road &road) : QLineF(road) {
	this->bounds = road.bounds;
	start = road.getStart();
	end = road.getEnd();
	this->rType = road.rType;
}

QRectF Road::boundingRect() const {
	return bounds;
}

void Road::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
	painter->drawLine(start.x(), start.y(), end.x(), end.y());
}

QRectF Road::boundsFromPointPair(Point s, Point e) {
	float minX = std::min(s.x(), e.y());
	float maxX = std::max(s.x(), e.y());
	float minY = std::min(s.x(), e.y());
	float maxY= std::max(s.x(), e.y());

	return QRectF(minX, minY, maxX - minX, maxY - minY);
}

std::vector<RoadIntersection*>* Road::getIntersections() {
	return &intersections;
}

void Road::deleteIntersection(RoadIntersection * toDelete) {
	std::vector<RoadIntersection*>::iterator it = std::find(intersections.begin(), intersections.end(), toDelete);
	assert(it != intersections.end());
	intersections.erase(it);
}

void Road::addIntersection(RoadIntersection * toCreate) {
	intersections.push_back(toCreate);
}

bool Road::operator==(const Road compare) {
	return (getStart() == compare.getStart()) && (getEnd() == compare.getEnd());
}


Road::~Road() {
}

StraightRoad::StraightRoad(Point start, Point end) : Road(start, end) {
}

std::string StraightRoad::printRoad() {
	std::ostringstream str;

	str << "Road begins at {" << this->getStart().x() << ", " << this->getStart().y() << "} "
		<< + " and ends at {" << this->getEnd().x() << ", " << this->getEnd().y() << "} ";

	return str.str();
}

Point Road::getStart() const { return start; }
Point Road::getEnd() const { return end; }

QPointF Road::lerp(float amount) {
	float finx = dx() * amount + x1();
	float finy = dy() * amount + y1();
	return QPointF(finx, finy);
}

void Road::setStart(Point start) {
	this->start = start;
	this->setP1(start);

	bounds = boundsFromPointPair(start, end);
}

void Road::setEnd(Point end) {
	this->end = end;
	this->setP2(end);

	bounds = boundsFromPointPair(start, end);
}



