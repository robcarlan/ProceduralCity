#include "BuildingRegion.h"



std::list<Point> BuildingRegion::getPoints() {
	return bounds;
}

BuildingRegion::BuildingRegion(std::list<roadPtr> roadList, std::list<bool> travelDirection, std::list<bool> side) {
	auto rItr = roadList.begin();
	auto dItr = travelDirection.begin();

	bounds.push_back(*dItr ? (*rItr)->getStart()->getIntersectionPoint() : (*rItr)->getEnd()->getIntersectionPoint());

	while (rItr != roadList.end()) {
		Point intersection = *dItr ? (*rItr)->getEnd()->getIntersectionPoint() : (*rItr)->getStart()->getIntersectionPoint();
		
		bounds.push_back(intersection);
		rItr++;
		dItr++;
	}
}

BuildingRegion::BuildingRegion() {
}


BuildingRegion::~BuildingRegion() {
}
