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
	std::map<roadPtr, bool> anticlockwiseVisited;
	std::map<roadPtr, bool> clockwiseVisited;

	bool hasVisited(bool side, bool forwards, const roadPtr traversing);
	bool isValidRegion(bool side, bool forwards, const roadPtr traversing);
	//Checks size of region to see if valid.
	bool isValidRegion(BuildingRegion& test);
	void flagRoads(std::list<roadPtr> traversing, std::list<bool> side, bool flagFound);

	//Region generation
	Point getRegionPoint(roadPtr r1, roadPtr r2, intersectionPtr intersection, bool cw1, bool cw2, float angle1, float angle2);
	std::list<Point> toRegion(std::list<roadPtr> &roadList, std::list<bool> &travelDirection, std::list<bool> &side, std::list<float>& angles);
public:
	//Returns <validRegions, invalidRegion>
	std::pair< std::list<BuildingRegion>&, std::list<BuildingRegion>& > filterRegions(std::list<BuildingRegion>& regions);
	//Builds regions
	std::vector<BuildingRegion> createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections);
	//Takes valid regions, builds into lots.
	void subdivideRegions(std::vector<BuildingRegion>& const buildings);
	void createLotsFromConvexPoly(std::list<Point>& bounds, std::vector<BuildingLot>& out);
	std::pair<std::pair<Point, Point>, std::pair<Point, Point>> getLongestEdgePair(const std::list<Point> &bounds);
	BuildingLot createLot(std::list<Point> bounds);

	void setMaxEdges(int max);

	CityRegionGenerator();
	~CityRegionGenerator();
};

