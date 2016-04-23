#include "City.h"

void City::createLots() {
	if (!regionsCreated) return;
	regionCreator.subdivideRegions(regions);

	//Build lots vector
	auto rItr = regions.begin();
	while (rItr != regions.end()) {
		std::vector<BuildingLot>& rLots = rItr->getLots();
		auto lItr = rLots.begin();
		while(lItr != rLots.end()) {
			lots.push_back(&(*lItr));
			lItr++;
		}

		rItr++;
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

void City::setImageData(QImage & density, QImage & buildingType, QImage &height, QImage &geog) {
	regionCreator.setImageData(density, buildingType, height, geog);
}

CityRegionGenerator & City::getRegionGenerator() {
	return regionCreator;
}

void City::resetRegions() {
	regionsCreated = lotsCreated = false;
	regions = std::vector<BuildingRegion>();
	lots = std::vector<BuildingLot*>();
}

void City::resetStreets() {
	hasGeometry = false;
	roads = std::list<roadPtr>();
	intersections = std::list<intersectionPtr>();
}

std::vector<BuildingLot*>& City::getLots() {
	return lots;
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
