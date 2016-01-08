#pragma once

#include "StreetManager.h"

#include <iostream>
#include <string>
#include <QImage>
#include <QtDebug>

#ifndef Q_MOC_RUN 
#include <boost/random.hpp>
#include <boost/foreach.hpp>
#endif

#include <list>
#include "Variable.h"

///Main Class handling street generation
///Stores input parameters / output parameters as well as generating functions.

//To allow for different behaviour, we can subclass / parameterise by RuleAttr / RoadAttr types
class StreetGen
{
protected:
	std::vector<Road> *generatedRoads;
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> genX, genY;

	typedef std::list<Variable> VarList;
	typedef std::list<Variable>::iterator VarIterator;

	//Manage l sys
	VarList current;
	std::list<std::pair<VarIterator,VarList>> toInsert;

	void applyLocalConstraints(Variable *toCheck);
	VarList* applyGlobalConstraints(ruleAttr rules, roadAttr roads);


	//After a variable succeeds, we need to add it correctly to the system
	void addRoadToSystem(roadAttr roads);

	void afterIteration();

	///Returns true if a production has been applied
	bool applyRule(VarIterator currentVar, VarList *productions);
	///Ensure these are added in order!
	void insertProduction(VarIterator before, VarList after);
	///Returns the initial production
	VarList getInitialProduction();
	ruleAttr getInitialRuleAttr();
	roadAttr getInitialRoadAttr();

	//World space of the drawing area
	Point size;

	bool ready, finished;
	int iterationCount;
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
	void printProductions();

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

