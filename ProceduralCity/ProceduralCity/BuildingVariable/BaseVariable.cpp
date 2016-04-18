#include "BaseVariable.h"

//Create LODs Level 1
BaseVariable::BaseVariable(float heightStart, float heightEnd, Point origin, Point sizeEnd, float width, float depth, float rot)
	: GeometryVariable() {
	LOD = 1;

	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);
	this->heightEnd = heightEnd;
	this->heightStart = heightStart;
	this->origin = origin;
	this->width = width;
	this->depth = depth;
	this->rot = rot;
}

BaseVariable::BaseVariable() {
}


BaseVariable::~BaseVariable() {
}

bool BaseVariable::getNextProductions(std::list<BuildingVariable>& out) {
	//Switch on LOD fam?

	// I.e. one base could have some sick podiums like neo gothic stuff
	return false;
}
