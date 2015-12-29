#pragma once

#include "Road.h"
#include "StreetManager.h"

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
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> genX, genY;

	StreetManager streets;

	//World space of the drawing area
	Point size;

	//We need a way of representing the coordinate system

	//Store a local copy of each image, that we can sample. QImage provides suitable behaviour
	QImage hMap, gMap, pMap;

public:

	//Functions to control the system
	void Run();
	//Performs one iteration of the L-System
	void nextIteration();
	//Restarts the system to an initial state
	void initialise();


	//Set parameters
	void setHeightMap(QImage &hMap);
	void setPopMap(QImage &pMap);
	void setGeogMap(QImage &gMap);
	void setSize(Point newSize);

	void setSeed(int seed);
	///Seeds with current system time
	void setSeed();

	//Add functions to get items which satisfy a query
	std::vector<Road>* getGenerated();

	StreetGen();
	~StreetGen();
};

