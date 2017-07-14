#pragma once

#include "Shapes\Cube.h"
#include "GeometryVariable.h"
#include "Enums.cpp"

#include <QRect>
#include <QLineF>
#include <random>

class BuildingLot;

//First variable for a building - represents the actual 'constructed' land on which a building is constructed
class LotVariable : public GeometryVariable {
	//Assumes a square construction
	QRect bounds;
	float height;
	float popDensity;
	Point origin;

	bool isSkyscraper;

	//Todo :: add special buildings.
	Cube boundBox;

public:
	QRect& getBounds();
	float getHeight();

	LotVariable(BuildingLot *base);
	LotVariable();
	~LotVariable();


	// Inherited via GeometryVariable
	virtual bool getNextProductions(std::list<BuildingVariable>& out) override;
	virtual void createIndustrial(std::list<BuildingVariable>& out);
	virtual void createResidential(std::list<BuildingVariable>& out);
	virtual void createCommercial(std::list<BuildingVariable>& out);

	virtual void getGeometry(std::vector<Cube>& out) override;

};

