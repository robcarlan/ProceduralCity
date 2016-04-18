#include "RoofVariable.h"



RoofVariable::RoofVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot)
	: GeometryVariable() {
	//LOD 1 roof
	LOD = 1;

	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);
	this->heightEnd = heightEnd;
	this->heightStart = heightStart;
	this->origin = origin;
	this->width = width;
	this->depth = depth;
	this->rot = rot;
}

RoofVariable::RoofVariable() {
}


RoofVariable::~RoofVariable() {
}

bool RoofVariable::getNextProductions(std::list<BuildingVariable>& out) {
	return false;
}
