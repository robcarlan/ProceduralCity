#include "TowerVariable.h"



TowerVariable::TowerVariable(LotVariable * start) {
//	this->start = start;
}

TowerVariable::TowerVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot)
	: GeometryVariable() {
	//Lod 1
	LOD = 1;
	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);
	this->heightEnd = heightEnd;
	this->heightStart = heightStart;
	this->origin = origin;
	this->width = width;
	this->depth = depth;
	this->rot = rot;
}

TowerVariable::TowerVariable() {
}


TowerVariable::~TowerVariable() {
}

bool TowerVariable::getNextProductions(std::list<BuildingVariable>& out) {
	//From tower, create more refined version via extrusions
	switch (getLOD()) {
	case 1: {
		//Basic bounds in place, so we need to create extrusions
	}
	case 2: {
		//Has been extruded, add some basic decoration to the building
	}
	};

	return true;
}