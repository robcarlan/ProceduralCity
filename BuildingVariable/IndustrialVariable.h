#pragma once
#include "BuildingVariable/GeometryVariable.h"
#include "Point.h"
#include "Enums.h"

#include <list>

class GeometryVariable;

class IndustrialVariable : public GeometryVariable {
public:
	IndustrialVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	IndustrialVariable();
	~IndustrialVariable();

	virtual bool getNextProductions(std::list<BuildingVariable> &out) override;

};

