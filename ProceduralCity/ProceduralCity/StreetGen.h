#pragma once

#include "Road.h"

#include <iostream>
#include <string.h>
#include <QImage>

#ifndef Q_MOC_RUN 
#include <boost/random.hpp>
#endif

///Main Class handling street generation
///Stores input parameters / output parameters as well as genearting functions.
class StreetGen
{
protected:
	std::vector<Road> *generatedRoads;

	//World space of the drawing area
	Point topLeft, bottomRight;

	//We need a way of representing the coordinate system

	//Store a local copy of each image, that we can sample. QImage provides suitable behaviour
	QImage hMap, gMap, pMap;

public:
	void Run();

	void setHeightMap(QImage &hMap);
	void unloadHMap();

	//Add functions to get items which satisfy a query
	std::vector<Road>* getGenerated();

	StreetGen();
	~StreetGen();
};

