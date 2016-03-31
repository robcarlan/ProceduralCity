#pragma once

#ifndef Q_MOC_RUN 
#include <boost\foreach.hpp>
#endif

#include <vector>
#include <qdebug.h>

#include "BuildingRegion.h"
#include "BuildingLot.h"
#include "IntersectionGeometry.h"
#include "RoadGeometry.h"

class CityRegionGenerator {
	int maxEdgeTraversal;
	std::map<roadPtr, bool> leftVisited;
	std::map<roadPtr, bool> rightVisited;

	bool hasVisited(bool side, const roadPtr traversing);
	bool isValidRegion(bool side, const roadPtr traversing);
	void flagRoads(std::list<roadPtr> traversing, std::list<bool> side, bool flagFound);
public:
	std::vector<BuildingRegion> createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections);
	std::vector<BuildingLot> subdivideRegions(std::list<BuildingRegion> const buildings);

	void setMaxEdges(int max);

	CityRegionGenerator();
	~CityRegionGenerator();
};

