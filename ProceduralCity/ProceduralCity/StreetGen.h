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
	static const float extendRadius;
	static const float minDistanceSq;
	static const float minLength;
	static const float roadBranchProb;
	//Global constraint constans
	static const float d2rFactor;
	static const float r2dFactor;
	static const float maxAngleSearch;
	static const float popDensityRadiusSearch;

	static const float manhattanBlockWidth;
	static const float manhattanBlockHeight;

	//Fit into legal
	static const float maxRoadRotate;
	static const float roadRotateInterval;
	static const float maxWaterTraverse;
	static const float maxPruneFactor;

	static const float roadSampleInterval;

	static const int BRANCH1;
	static const int BRANCH2;
	static const int ROAD;

	static const int NATURAL_PATTERN_INDEX;
	static const int SF_PATTERN_INDEX;
	static const int MANHATTAN_PATTERN_INDEX;
	static const int RADIAL_PATTERN_INDEX;

	std::vector<Road> *generatedRoads;
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> genX, genY;

	typedef std::list<Variable> VarList;
	typedef std::list<Variable>::iterator VarIterator;

	//Manage l sys
	VarList current;
	std::list<std::pair<VarIterator,VarList>> toInsert;

	void applyLocalConstraints(Variable *toCheck);

	//Functions for local Constraints
	//Tries to clip the line into a legal region. Returns true if the line is successfully placed.
	bool tryMakeLegal(Variable *toCheck, Road *tempRoad);
	void getIllegalSegment(Road segment, bool &legal);
	//Tries to alter the road such that it connects to existing intersections.
	bool tryConnectToExisting(Variable *toCheck, Road *tempRoad, bool &connectedToIntersection);

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
	bool useHeight, useGeog, usePop, usePattern;

	//Store a local copy of each image, that we can sample. QImage provides suitable behaviour
	QImage hMap, gMap, pMap, sMap;
	QRgb parkCol;
	QRgb landCol;
	QRgb waterCol;

	//Image functions
	float sampleHeight(int x, int y);
	//QColor sampleGeog(int x, int y);
	geogType sampleGeog(int x, int y);
	float samplePop(int x, int y);
	QColor sampleStreet(int x, int y);
	//Returns weighting for each pattern between 0 and 1
	void getPatternWeightings(int x, int y, float weights[]);

	float getGradient(QImage *map);
	//Used to find direction with largest pop density (summed)
	float maxPopDensity(float startAngle, Point &start, Point &end);
	//Used to get direction with smallest gradient. Returns minimum angle.
	float getLowestGradient(float startAngle, Point start, float length);
	
	//Returns destination if we branch based on local angle and block size
	Point manhattanRule(const roadAttr* road);
	//Determines which length to use for the block
	bool useBlockHeight(float blockAngle, float curAngle);
	//Returns destination if we follow by population density
	Point naturalRule(const roadAttr* road);
	//Returns destination if we follow by lowest height gradient
	Point sanFransiscoRule(const roadAttr* road);
	//Returns destination if we follow by rotating along a center
	Point radialRule(const ruleAttr *rules, const roadAttr *road);

	Point weighValues(float weights[], Point*vals, Point *start);

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
	void setPatternMap(QImage &sMap, bool use);
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

