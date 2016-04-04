#include "BuildingLot.h"



std::list<Point> BuildingLot::getPoints() {
	return points;
}

void BuildingLot::setOwner(BuildingRegion * owner) {
	this->owner = owner;
}

BuildingLot::BuildingLot(std::list<Point> points) {
	this->points = points;
}

BuildingLot::BuildingLot() {
}


BuildingLot::~BuildingLot() {
}
