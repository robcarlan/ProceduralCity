#pragma once
#include "BuildingVariable/GeometryVariable.h"

///Represents a cube that cannot be divided further
class CubeVariable : public GeometryVariable {
public:
	CubeVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	CubeVariable();
	~CubeVariable();

	virtual bool getNextProductions(std::list<BuildingVariable> &out) override;

};

