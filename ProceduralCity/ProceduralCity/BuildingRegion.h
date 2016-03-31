#pragma once
#include "RoadGeometry.h"
#include "BuildingLot.h"
#include "Point.h"
#include <list>

class BuildingLot;

//Encircled by roads, a possible convex (?) set of points representing where buildings can be constructed. The region handles LOD, visibility of buildings in batches.
class BuildingRegion {
protected:
	std::list<Point> bounds;
	std::list<BuildingLot> lots;

	//Region data, i.e. commercial...
	float popDensity;
public:
	std::list<Point> getPoints();
	BuildingRegion(std::list<roadPtr> roadList, std::list<bool> travelDirection);
	BuildingRegion();
	~BuildingRegion();
};

