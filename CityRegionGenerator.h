#pragma once

#ifndef Q_MOC_RUN 
#include <boost/foreach.hpp>
#endif

#include <vector>
#include <qdebug.h>
#include <QElapsedTimer>

#include "BuildingRegion.h"
#include "BuildingLot.h"
#include "IntersectionGeometry.h"
#include "RoadGeometry.h"
#include "Road.h"
#include "Enums.h"

class CityRegionGenerator {
	int maxEdgeTraversal;
	std::map<roadPtr, bool> anticlockwiseVisited;
	std::map<roadPtr, bool> clockwiseVisited;

	bool hasVisited(bool side, bool forwards, const roadPtr traversing);
	bool isValidRegion(bool side, bool forwards, const roadPtr traversing);
	bool coversIllegalTerritory(std::list<Point> &bounds);
	//Checks size of region to see if valid.
	bool isValidRegion(BuildingRegion& test);
	void flagRoads(std::list<roadPtr> traversing, std::list<bool> side, bool flagFound);

	//Region generation
	Point getRegionPoint(roadPtr r1, roadPtr r2, intersectionPtr intersection, bool cw1, bool cw2, float angle1, float angle2);
	std::list<Point> toRegion(std::list<roadPtr> &roadList, std::list<bool> &travelDirection, std::list<bool> &side, std::list<float>& angles);
	buildingStyle getBuildingStyle(Point& pos);
	float getPopDensity(Point &pos);
	float getHeight(Point &pos);
	float getHeightValue(float factor);
	float getRoadWidth(const roadType &road);

	//Region data
	QImage *densitySampler;
	QImage *buildingTypeSampler;
	QImage *heightSampler;
	QImage *geogSampler;

	float minHeight, heightScale;
	float mainRoadWidth, streetWidth;
	float minBuildArea, maxBuildArea, randOffset;
	float minLotDim, maxLotDim;
	bool allowLotMerge;
	int seed;

public:
	//Builds regions
	std::vector<BuildingRegion> createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections);
	void subdivideRegions(std::vector<BuildingRegion>& buildings);
	void createLotsFromConvexPoly(BuildingRegion& owner, std::list<Point>& bounds, std::list<bool>& roadAccess, std::unique_ptr<std::vector<BuildingLot>>& out);

	std::list<Point> subdivideBounds(std::list<Point> &bounds);
	bool shouldSplitBoundsFurther(std::list<Point> &bounds);

	//Returns longest edge and roughly parallel other edge
	std::pair<std::pair<Point, Point>, std::pair<Point, Point>> getLongestEdgePair(const std::list<Point> &bounds, const std::list<bool>& hasRoadAccess);
	BuildingLot createLot(std::list<Point> bounds, BuildingRegion& owner);

	void setMaxEdges(int max);
	void setImageData(QImage& density, QImage& buildingType, QImage& height, QImage& geog);
	void setParams(float minBuildArea, float maxBuildArea, float randomOffset, float minLotDim, float maxLotDim,
		float mainRoadWidth, float streetWidth, bool allowLotMerge, float minHeight, float heightScale, int seed);

	CityRegionGenerator();
	~CityRegionGenerator();
};

