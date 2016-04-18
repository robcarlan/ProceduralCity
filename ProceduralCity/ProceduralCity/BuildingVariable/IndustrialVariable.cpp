#include "IndustrialVariable.h"



IndustrialVariable::IndustrialVariable() {
}


IndustrialVariable::IndustrialVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot) {
	LOD = 1;

	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);

	this->heightEnd = heightEnd;
	this->heightStart = heightStart;
	this->origin = origin;
	this->width = width;
	this->depth = depth;
	this->rot = rot;
}

IndustrialVariable::~IndustrialVariable() {
}

bool IndustrialVariable::getNextProductions(std::list<BuildingVariable> &out) {
	return false;
}