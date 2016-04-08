#include "BuildingLot.h"



std::list<Point>& BuildingLot::getPoints() {
	return points;
}

void BuildingLot::setOwner(BuildingRegion * owner) {
	this->owner = owner;
}

void BuildingLot::setStyle(buildingStyle style) {
	this->style = style;
}

void BuildingLot::setPopDensity(float popDensity) {
	this->popDensity = popDensity;
}

BuildingRegion * BuildingLot::getOwner() {
	return owner;
}

BuildingLot::BuildingLot(std::list<Point> points) {
	this->points = points;
}

BuildingLot::BuildingLot() 
	:points() {
}


BuildingLot::~BuildingLot() {
}
