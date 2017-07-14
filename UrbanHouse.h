#pragma once
#include "BuildingVariable/GeometryVariable.h"
#include "Point.h"
#include "Enums.cpp"

#include <list>

class GeometryVariable;

///Represents a house in a commercial / high pop density area. I.e. a 
class UrbanHouse : public GeometryVariable {
public:
	UrbanHouse(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);
	UrbanHouse();
	~UrbanHouse();

	// Inherited via GeometryVariable
	virtual bool getNextProductions(std::list<BuildingVariable>& out) override;
};

