#pragma once
#include "Enums.cpp"
#include "GeometryVariable.h"

class LotVariable;

class TowerVariable : public GeometryVariable {
	
public:
	TowerVariable(LotVariable *start);
	TowerVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	TowerVariable();
	~TowerVariable();

	bool hasExtrusions;

	bool getNextProductions(std::list<BuildingVariable> &out);
};

