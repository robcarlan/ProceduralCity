#pragma once

#include "Road.h"

#include <iostream>
#include <String.h>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/algorithms/area.hpp>

#include <boost\random.hpp>

///Main Class handling street generation
///Stores input parameters / output parameters as well as genearting functions.
class StreetGen
{
public:
	void Run();

	std::vector<Road> generatedRoads;

	//World space of the drawing area
	Point topLeft, bottomRight;

	StreetGen();
	~StreetGen();
};

