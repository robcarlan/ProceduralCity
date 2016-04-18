#pragma once
#include "Building.h"
#include "BuildingRegion.h"
#include "Point.h"
#include "Road.h"
#include <list>

class BuildingRegion;
class Building;

///Defines a region for a building lot - comprised of a set of points.
///Owned by a building region
class BuildingLot {
protected:
	std::list<Point> points;
	BuildingRegion* owner;
	Building *building;
	buildingStyle style;

	float popDensity;
	float height;
	Point centroid;

	bool useSkyscraper;
	bool buildingMade;
	bool isTriangle, isQuad, isComplexPoly;

	int seed;

public:
	std::list<Point>& getPoints();
	void setOwner(BuildingRegion* owner);
	void setStyle(buildingStyle style);
	void setPopDensity(float popDensity);
	void setHeight(float height);
	void setSeed(int seed);
	void setSkyscraper(bool isSkyscraper);
	void createBuilding();
	float getPopDensity();
	float getHeight();

	int getSeed();
	bool getIsTriangle();
	bool getIsQuad();
	bool getIsComplexPoly();
	bool getIsSkyscraper();

	Point &getCentroid();
	buildingStyle getStyle();
	BuildingRegion * getOwner();
	Building* getBuilding();
	BuildingLot(std::list<Point> points);
	BuildingLot();
	~BuildingLot();

	static QRect maximumRectangle(std::list<Point>& bounds);
};

