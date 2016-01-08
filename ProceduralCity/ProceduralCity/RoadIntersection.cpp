#include "RoadIntersection.h"

const float RoadIntersection::boxWidth = 10.0f;

///Called when a point is created
void RoadIntersection::calculateBoundingBox() {
	boundingBox.setX(location.x() - boxWidth / 2);
	boundingBox.setY(location.y() - boxWidth / 2);
	boundingBox.setWidth(boxWidth);
	boundingBox.setHeight(boxWidth);
}

///Returns a rectangle surrounding the bounding box
QRectF RoadIntersection::boundingRect() const {
	return boundingBox;
}

void RoadIntersection::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
	painter->drawRect(boundingBox);
}

void RoadIntersection::setParent(Road * road) {
	parent = road;

}

void RoadIntersection::setIncident(Road * road) {

}

void RoadIntersection::attachRoad(Road *road) {
	//Ensure position is within bounds
	connected.push_back(road);
}

bool RoadIntersection::removeRoad(Road *road) {
	assert(std::find(connected.begin(), connected.end(), road) != connected.end());

	connected.remove(road);
	return (connected.size() <= 1);
}

RoadIntersection::RoadIntersection(Point Position, Road *creator, Road *incidentOnto) : QGraphicsItem() {
	this->location = Position;
	this->setPos(location);
	parent = creator;
	connected.push_front(creator);
	connected.push_front(incidentOnto);

	isCrossing = true;
	calculateBoundingBox();
}

RoadIntersection::RoadIntersection(Point Position, Road *creator) : QGraphicsItem() {
	this->location = Position;
	this->setPos(location);
	parent = creator;

	isCrossing = false;
	calculateBoundingBox();
}

RoadIntersection::RoadIntersection(const RoadIntersection &road) : QGraphicsItem() {
	this->location = road.location;
	this->setPos(location);
	this->boundingBox = road.boundingBox;
	this->isCrossing = road.isCrossing;
	this->parent = road.parent;
	this->connected = road.connected;
	calculateBoundingBox();
}

RoadIntersection::RoadIntersection() : QGraphicsItem() {
}


RoadIntersection::~RoadIntersection() {
}
