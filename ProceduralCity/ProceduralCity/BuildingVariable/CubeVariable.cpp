#include "CubeVariable.h"



CubeVariable::CubeVariable() {
}


CubeVariable::CubeVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot) {
	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);
	LOD = 3;
}

CubeVariable::~CubeVariable() {
}

bool CubeVariable::getNextProductions(std::list<BuildingVariable> &out) {
	//Shouldn't be called! This is lod max
	return false;
}
