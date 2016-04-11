#pragma once
#include "BuildingRegion.h"
#include "Point.h"
#include "Road.h"
#include <list>

class BuildingRegion;

///Defines a region for a building lot - comprised of a set of points.
///Owned by a building region
class BuildingLot {
protected:
	std::list<Point> points;
	BuildingRegion* owner;
	buildingStyle style;
	float popDensity;
	Point centroid;

public:
	std::list<Point>& getPoints();
	void setOwner(BuildingRegion* owner);
	void setStyle(buildingStyle style);
	void setPopDensity(float popDensity);
	Point &getCentroid();
	buildingStyle getStyle();
	BuildingRegion * getOwner();
	BuildingLot(std::list<Point> points);
	BuildingLot();
	~BuildingLot();
};

