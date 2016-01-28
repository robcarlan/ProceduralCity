#include "Road.h"

const float Road::PEN_WIDTH = 25.0f;

Road::Road(Point start, Point end, RoadIntersection * intersection, Road * parent, roadType rType) : QGraphicsItem(), QLineF(start, end) {
	intersections.clear();

	assert(start.getDistanceSq(intersection->location) < 2.0f);

	this->addStartIntersection(intersection);
	intersection->attachRoad(this);

	this->setP1(start);
	this->setP2(end);
	this->start = start;
	this->end = end;
	this->rType = rType;
	this->parent = parent;
	

	//calculate bounds
	bounds = boundsFromPointPair(start, end);
	calcAngle();
}

Road::Road(Point start, Point end, Road *parent, roadType rType) : QGraphicsItem(), QLineF(start, end) {
	this->setP1(start);
	this->setP2(end);
	this->start = start;
	this->end = end;
	this->rType = rType;
	this->parent = parent;

	//calculate bounds
	bounds = boundsFromPointPair(start, end);
	calcAngle();
}

Road::Road(const Road &road) : QLineF(road) {
	this->bounds = road.bounds;
	start = road.getStart();
	end = road.getEnd();
	this->rType = road.rType;
	this->parent = parent;
	angle = road.getAngle();
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

void Road::calcAngle() {
	angle = atan2f(end.y() - start.y(), end.x() - start.x());
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

void Road::addStartIntersection(RoadIntersection * start) {
	roadStartIntersection = start;
	intersections.push_back(start);
}

void Road::addEndIntersection(RoadIntersection * end) {
	roadEndIntersection = end;
	intersections.push_back(end);
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

float Road::getAngle() const {
	return angle;
}

bool Road::isInBounds(int sizeX, int sizeY) const {
	return (start.isWithinBounds(sizeX, sizeY) && end.isWithinBounds(sizeX, sizeY));
}

QPointF Road::lerp(float amount) {
	float finx = dx() * amount + x1();
	float finy = dy() * amount + y1();
	return QPointF(finx, finy);
}

void Road::setStart(Point start) {
	this->start = start;
	this->setP1(start);

	bounds = boundsFromPointPair(start, end);
	calcAngle();
}

void Road::setEnd(Point end) {
	this->end = end;
	this->setP2(end);

	bounds = boundsFromPointPair(start, end);
	calcAngle();
}



