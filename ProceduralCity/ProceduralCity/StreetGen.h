#pragma once

#include "Road.h"

#include <iostream>
#include <String.h>

#ifndef Q_MOC_RUN 
#include <boost\random.hpp>
#endif

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

