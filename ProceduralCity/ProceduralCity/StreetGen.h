#pragma once

#include "Road.h"
#include "StreetManager.h"

#include <iostream>
#include <string.h>
#include <QImage>

#ifndef Q_MOC_RUN 
#include <boost/random.hpp>
#endif

#include <list>
#include "Variable.h"

///Main Class handling street generation
///Stores input parameters / output parameters as well as genearting functions.
class StreetGen
{
protected:
	std::vector<Road> *generatedRoads;
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> genX, genY;

	//Manage l sys
	std::list<Variable> current;
	std::list<InsertionVar> roadsToInsert;

	//World space of the drawing area
	Point size;

	bool ready, finished;
	bool useHeight, useGeog, usePop;

	//Store a local copy of each image, that we can sample. QImage provides suitable behaviour
	QImage hMap, gMap, pMap;

public:

	//Functions to control the system
	void Run();
	//Performs one iteration of the L-System
	void nextIteration();
	//Restarts the system to an initial state
	void initialise();
	//Returns true if the system is ready to generate the streets.
	bool isReady();
	bool isFinished();

	//Set parameters

	void setHeightMap(QImage &hMap, bool use);
	void setPopMap(QImage &pMap, bool use);
	void setGeogMap(QImage &gMap, bool use);
	void setSize(Point newSize);

	void setSeed(int seed);
	///Seeds with current system time
	void setSeed();

	//Add functions to get items which satisfy a query

	QGraphicsScene *getScene();
	StreetManager streets;
	std::vector<Road>* getGenerated();

	StreetGen();
	~StreetGen();
};

