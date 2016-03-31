#include "BuildingRegion.h"



std::list<Point> BuildingRegion::getPoints() {
	return bounds;
}

BuildingRegion::BuildingRegion(std::list<roadPtr> roadList, std::list<bool> travelDirection) {
	auto rItr = roadList.begin();
	auto dItr = travelDirection.begin();

	bounds.push_back(*dItr ? (*rItr)->getStart()->getIntersectionPoint() : (*rItr)->getEnd()->getIntersectionPoint());

	while (rItr != roadList.end()) {
		bounds.push_back(*dItr ? (*rItr)->getEnd()->getIntersectionPoint() : (*rItr)->getStart()->getIntersectionPoint());
		rItr++;
		dItr++;
	}
}

BuildingRegion::BuildingRegion() {
}


BuildingRegion::~BuildingRegion() {
}
