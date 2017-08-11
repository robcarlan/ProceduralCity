#pragma once
#include "GeometryVariable.h"

class LotVariable;
class BuildingVariable;

class RoofVariable : public GeometryVariable {
public:
	RoofVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	RoofVariable();
	~RoofVariable();

	// Inherited via GeometryVariable
	virtual bool getNextProductions(std::list<BuildingVariable>& out) override;
};

