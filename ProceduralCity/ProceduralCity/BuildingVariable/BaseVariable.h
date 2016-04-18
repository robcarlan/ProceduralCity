#pragma once
#include "Enums.h"
#include "GeometryVariable.h"

#include <QRect>

class LotVariable;
class BuildingVariable;

//Represent the base of a structure, i.e. the first few floors.
class BaseVariable : public GeometryVariable {
public:
	//Create the first iteration of the base
	BaseVariable(float heightStart, float heightEnd, Point origin, Point sizeEnd, float width, float depth, float rot);
	BaseVariable();
	~BaseVariable();

	// Inherited via GeometryVariable
	virtual bool getNextProductions(std::list<BuildingVariable>& out) override;
};

