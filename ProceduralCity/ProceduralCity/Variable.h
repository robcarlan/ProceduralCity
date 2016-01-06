#pragma once
#include <string>
#include "Road.h"

struct ruleAttr {
	//Do we end at a crossing? Did this crossing exist before or did we create it
	int depth;
};

struct roadAttr {
	float length, angle;
	Point start, end;
};

enum Zone {
	RESIDENTIAL,
	COMMERTICAL,
	INDUSTRIAL
};

enum solutionState {
	UNASSIGNED,
	SUCCEED,
	FAILED
};

enum variableType {
	ROAD,
	INSERTION,
	BRANCH,
	EMPTY
};

//Represents the basic variable
class Variable {
public:
	int delay;
	ruleAttr rules;
	roadAttr road;
	variableType varType;
	solutionState state;

	Variable(variableType vType, int delay, ruleAttr rules, roadAttr road);
	Variable(variableType vType);
	Variable(variableType vType, int delay);
	Variable();
	~Variable();

	std::string toString();

};

