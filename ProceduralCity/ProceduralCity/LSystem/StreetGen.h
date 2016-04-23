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
#include "LSystem.h"
#include "RoadVariable.h"
#include "BuildingRegion.h"
#include "BuildingLot.h"

///Main Class handling street generation
///Stores input parameters / output parameters as well as generating functions.

//To allow for different behaviour, we can subclass / parameterise by RuleAttr / RoadAttr types
//TODO :: Move rule generation outside of this class, so we can re use for the building generation.
class StreetGen : public LSystem
{
protected:
	//Simulation parameters
	float extendRadius;
	float roadSnap;
	float minLength;
	float minDistanceSq;
	float roadBranchProb;
	float streetLength;
	float highwaylength;
	float randomLengthVariation;
	//Global constraint constans
	float maxAngleSearch;
	float popDensityRadiusSearch;

	float minStreetGrowthFactor;
	float minHighwayGrowthFactor;
	float minStreetGrowthScore;
	float minHighwayGrowthScore;
	int streetDelay;
	int followPeakForDefault;

	float manhattanBlockWidth;
	float manhattanBlockHeight;
	bool useWeightedVals;

	//Fit into legal
	float maxRoadRotate;
	float roadRotateInterval;
	float maxWaterTraverse;
	float maxPruneFactor;
	float roadSampleInterval;

	static const int BRANCH1;
	static const int BRANCH2;
	static const int ROAD;

	static const int NATURAL_PATTERN_INDEX;
	static const int SF_PATTERN_INDEX;
	static const int MANHATTAN_PATTERN_INDEX;
	static const int RADIAL_PATTERN_INDEX;

	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> genX, genY;

	void applyLocalConstraints(RoadVariable *toCheck);

	//Functions for local Constraints
	//Tries to clip the line into a legal region. Returns true if the line is successfully placed.
	bool tryMakeLegal(RoadVariable *toCheck, Road *tempRoad);
	//To ensure we haven't made a road on top of another
	bool isUnique(RoadVariable *toCheck, Road *tempRoad);
	void getIllegalSegment(Road segment, bool &legal);
	//Returns true if this road is along the same angle as existing roads. 
	bool overlapsConnected(RoadVariable *toCheck, Road *tempRoad);
	//Tries to alter the road such that it connects to existing intersections.
	bool tryConnectToExisting(RoadVariable *toCheck, Road *tempRoad, bool &connectedToIntersection, bool &connectedToNewIntersection);
	bool isFinishedGrowing(ruleAttr &rules, roadAttr &road);

	VarList* applyGlobalConstraints(ruleAttr rules, roadAttr roads);


	//After a variable succeeds, we need to add it correctly to the system
	void addRoadToSystem(roadAttr &roads);

	//Implementing L System
	bool isLSystemFinished(bool changed);
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
	Point start, end;

	bool ready, genStreets, delayChanged, finished;
	bool useHeight, useGeog, usePop, usePattern;
	bool printOutput;

	//Store a local copy of each image, that we can sample. QImage provides suitable behaviour
	QImage hMap, gMap, pMap, sMap;

	//Image functions
	float sampleHeight(int x, int y);
	QColor sampleStreet(int x, int y);
	//Returns weighting for each pattern between 0 and 1
	void getPatternWeightings(int x, int y, float weights[]);

	float getGradient(QImage *map);
	//Used to find direction with largest pop density (summed)
	float maxPopDensity(float startAngle, Point &start, Point &end, QPointF *peak);
	//Used to get direction with smallest gradient. Returns minimum angle.
	float getLowestGradient(float startAngle, Point start, float length);
	
	//Returns destination if we branch based on local angle and block size
	Point manhattanRule(const roadAttr* road);
	//Determines which length to use for the block
	bool useBlockHeight(float blockAngle, float curAngle);
	//Returns destination if we follow by population density
	Point naturalRule(roadAttr* road, ruleAttr *rules);
	//Returns destination if we follow by lowest height gradient
	// # roads must fall
	Point sanFransiscoRule(const roadAttr* road);
	//Returns destination if we follow by rotating along a center
	Point radialRule(const ruleAttr *rules, const roadAttr *road);

	Point weighValues(float weights[], Point*vals, Point *start);

	//Points a road towards it's target, within angle bounds. Returns true if we reach the target.
	bool pointToTarget(roadAttr *road, ruleAttr *rules);

public:

	static const float d2rFactor;
	static const float r2dFactor;

	//Functions to control the system
	//void Run();
	//Performs one iteration of the L-System
	//void nextIteration();
	//Restarts the system to an initial state
	void initialise();
	//Returns true if the system is ready to generate the streets.
	bool isReady();
	void LSystemBeforeIterationBegin();
	void LSystemAfterIteration(bool changed);
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
	void setDefaultValues();

	//Add functions to get items which satisfy a query
	StreetManager streets;
	std::list<Road*> getGeneratedRoads();
	std::list<RoadIntersection*> getGeneratedIntersections();

	void setExtendRadius(float val);
	void setRoadSnap(float val);
	void setMinLength(float val);
	void setRoadBranchProb(float val);
	void setMaxAngleSearch(float val);
	void setManhattanBlockWidth(float val);
	void setManhattanBlockHeight(float val);
	void setMaxRoadRotate(float val);
	void setRoadRotateInterval(float val);
	void setMaxWaterTraverse(float val);
	void setMaxPruneFactor(float val);
	void setRoadSampleInterval(float val);
	void setUsePatternWeighting(bool useWeighting);
	void setRoadLength(float val);
	void setHighwayLength(float val);
	void setPopRadiusSearch(float val);
	void setHighwayGrowthFactor(float factor);
	void setStreetGrowthFactor(float factor);
	void setStreetVariation(float variation);
	void setMainroadFollowLength(int length);
	void setStreetDelay(int delay);
	void setStartParams(Point start, Point end);

	StreetGen(CityView2D *view, Point size);
	StreetGen();
	~StreetGen();
};

