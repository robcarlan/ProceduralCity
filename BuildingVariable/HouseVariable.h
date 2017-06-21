#pragma once
#include "BuildingVariable/GeometryVariable.h"
#include "Point.h"
#include "Enums.h"

#include <list>

class GeometryVariable;

class HouseVariable : public GeometryVariable {
public:
	HouseVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	HouseVariable();
	~HouseVariable();

	virtual bool getNextProductions(std::list<BuildingVariable> &out) override;

};

