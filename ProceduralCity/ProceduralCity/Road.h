#pragma once

#include "Point.h"

#include <vector>

//typedef boost::geometry::model::d2::point_xy<float> Point;

///Abstract class defining a simple road. 
class Road {
public:
	Road(Point start, Point end);
	~Road();

	//std::vector<Road> getConnected();
	Point getStart();
	Point getEnd();

	std::string printRoad();
protected:
	Point start, end;

	//std::vector<Road, Point> connected;
	//std::vector<Point> intersections;

	//TODO :: Main / Minor Roads

	//TODO :: Roads to be of various types, i.e. Normal roads, Overground Roads (which ignore clipping over roads), Underground, Bridges ...
};

///Road defined by two points. The road is a straight line between them.
class StraightRoad
	: public Road {
public:
	StraightRoad(Point start, Point end);

	std::string printRoad();
	//Point start, end;
};

//Circular / Splines
