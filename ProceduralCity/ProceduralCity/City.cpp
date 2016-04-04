#include "City.h"

void City::createLots() {
	if (!regionsCreated) return;
	regionCreator.subdivideRegions(regions);

	//Build lots vector
	BOOST_FOREACH(BuildingRegion region, regions) {
		BOOST_FOREACH(BuildingLot lot, region.getLots()) {
			lots.push_back(lot);
		}
	}

	lotsCreated = true;
}

void City::createRegions() {
	if (!hasGeometry) return;
	regions = regionCreator.createRegions(roads, intersections);
	regionsCreated = true;
}

void City::getGeometry(std::pair<std::list<roadPtr>, std::list<intersectionPtr>> geometry) {
	roads = geometry.first;
	intersections = geometry.second;

	hasGeometry = true;
}

std::vector<BuildingLot&>* City::getLots() {
	return &lots;
}

std::vector<BuildingRegion>* City::getRegions() {
	return &regions;
}

City::City() {
	hasGeometry = regionsCreated = lotsCreated = false;
	regionCreator = CityRegionGenerator();
}


City::~City() {
}
