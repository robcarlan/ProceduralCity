#pragma once
#include "RoadGeometry.h"
#include "BuildingLot.h"
#include "Point.h"

#include <qdebug.h>
#include <list>


#ifndef Q_MOC_RUN 
#include <boost\geometry.hpp>
#include <boost\foreach.hpp>
#include <boost\geometry\geometries\point_xy.hpp>
#include <boost\geometry\geometries\polygon.hpp>
#endif

class BuildingLot;

//Encircled by roads, a possible convex (?) set of points representing where buildings can be constructed. The region handles LOD, visibility of buildings in batches.
class BuildingRegion {
protected:
	std::list<Point> bounds;
	std::vector<BuildingLot> lots;
	std::list<float> roadBearing; // Stores bearing of each road

	//Region data, i.e. commercial...
	float popDensity;
	float area;
	bool valid;
	bool convex;
	Point centroid;

	void calculateArea();
public:
	std::list<Point>& getPoints();
	std::vector<BuildingLot>& getLots();
	bool isValid();
	bool isConvex();
	void flagInvalid();
	void setLots(std::vector<BuildingLot> lots);
	Point &getCentroid();
	BuildingRegion(std::list<Point> bounds, std::list<float> angles);
	BuildingRegion();
	~BuildingRegion();

	//Returns siged area - cw => area -ve
	static float getPolyArea(const std::list<Point>& bounds);
	static Point getCentroid(const std::list<Point>& bounds);
	//Splits polygon into multiple convex polygons.
	static void splitConvex(const std::list<Point>& bounds, std::vector<std::list<Point>> &convex);
	//Returns index of point which breaks convexity. If convex, returns end iterator.
	static std::list<Point>::const_iterator getConcave(const std::list<Point>& bounds);
	static bool isPolyConvex(const std::list<Point>& bounds, const std::list<Point>::const_iterator itr);
	static std::pair<std::list<Point>, std::list<Point>> splitPolygon(const std::list<Point>& bounds, const std::list<Point>::const_iterator splitStart, const std::list<Point>::const_iterator splitEnd);
};

