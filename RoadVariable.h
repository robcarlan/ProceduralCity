#pragma once
#include <string>
#include "Road.h"
#include "Variable.h"

struct ruleAttr {
	//Do we end at a crossing? Did this crossing exist before or did we create it
	int depth;
	int followPeakFor;
	float *roadPatternVals;

	//Something to control direction

	//Indicates where a block points. 
	float manhattanBlockDirection;
	QPointF peakTarget;
};

struct roadAttr {
	float length, angle;
	Point start, end;
	roadType rtype;
	roadPattern majorityPattern;
	float manhattanFactor;
	float sfFactor;
	float radialFactor;
	float naturalFactor;
	//null => create an intersection at start
	RoadIntersection *branchSource;
	//null => create an intersection at end
	RoadIntersection *target;

	//The road that generated this.
	Road *parentRoad;
	//The source of the whole path
	Road *rootRoad;
	//Incident road, if exists.
	Road *targetRoad;
	//The road generated by this rule
	Road *generated;
	
	bool connected;
	bool failed;

	roadAttr() {
		parentRoad = rootRoad = generated = targetRoad  = nullptr;
		target = branchSource = nullptr;
		connected = false;
		failed = false;
	}

	void createFromIntersection(RoadIntersection *source) {
		branchSource = source;
		start = source->location;
	}

	void branchFromParent(Road *parent) {
		this->parentRoad = parent;
	}

	//Recalculate end point from length and angle
	void recalculateEndPoint();

	void flagFail() {
		failed = true;
	}

	bool hasFailed() {
		return failed;
	}
	
};

enum class Zone {
	RESIDENTIAL,
	COMMERTICAL,
	INDUSTRIAL
};

enum class solutionState {
	UNASSIGNED,
	SUCCEED,
	FAILED
};

enum class variableType {
	ROAD,
	INSERTION,
	BRANCH,
	EMPTY
};

//Represents the basic variable
class RoadVariable : Variable {
public:
	int delay;
	ruleAttr rules;
	roadAttr road;
	variableType varType;
	solutionState state;

	RoadVariable(variableType vType, int delay, ruleAttr rules, roadAttr road);
	RoadVariable(variableType vType);
	RoadVariable(variableType vType, int delay);
	RoadVariable();
	~RoadVariable();

	std::string toString();
};

