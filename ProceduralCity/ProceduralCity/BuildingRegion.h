#pragma once
#include "RoadGeometry.h"
#include "BuildingLot.h"
#include "Point.h"
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

	//TODO :: Bounding box?

	void calculateArea();
public:
	std::list<Point> getPoints();
	std::vector<BuildingLot>& getLots();
	bool isValid();
	bool isConvex();
	void flagInvalid();
	void setLots(std::vector<BuildingLot>& lots);
	BuildingRegion(std::list<Point> bounds, std::list<float> angles);
	BuildingRegion();
	~BuildingRegion();

	//Returns siged area - cw => area -ve
	static float getPolyArea(const std::list<Point>& bounds);
	static Point getCentroid(const std::list<Point>& bounds);
	//Splits polygon into multiple convex polygons.
	static std::vector<std::list<Point>> splitConvex(const std::list<Point>& bounds);
	//Returns index of point which breaks convexity. If convex, returns end iterator.
	static std::list<Point>::const_iterator getConcave(const std::list<Point>& bounds);
	static std::pair<std::list<Point>, std::list<Point>> splitPolygon(const std::list<Point>& bounds, const std::list<Point>::const_iterator splitStart, const std::list<Point>::const_iterator splitEnd);
};

