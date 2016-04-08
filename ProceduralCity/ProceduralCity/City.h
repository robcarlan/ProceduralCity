#pragma once
#include "RoadGeometry.h"
#include "IntersectionGeometry.h"
#include "BuildingLot.h"
#include "BuildingRegion.h"
#include "SimpleStreetGeometryCreator.h"
#include "CityRegionGenerator.h"

//Contains graphical representation of city - geometry for intersections, roads, as well as building lots.
class City {
	bool hasGeometry, regionsCreated, lotsCreated;
	CityRegionGenerator regionCreator;
	float mainRoadWidth, streetWidth;

public:
	//TODO :: What collection needs to be used here?
	std::list<roadPtr> roads;
	std::list<intersectionPtr> intersections;
	std::vector<BuildingRegion> regions;
	std::vector<BuildingLot*> lots;

	//Store building lots in some spatial order?
	void createRegions();
	void createLots();
	void getGeometry(std::pair<std::list<roadPtr>, std::list<intersectionPtr>> geometry);
	void setImageData(QImage& density, QImage& buildingType);
	CityRegionGenerator& getRegionGenerator();
	void resetRegions();
	void resetStreets();

	std::vector<BuildingLot*>& getLots();
	std::vector<BuildingRegion>* getRegions();


	City();
	~City();
};

