#pragma once
#include <list>

#include "RoofVariable.h"
#include "TowerVariable.h"
#include "LotVariable.h"
#include "BaseVariable.h"
#include "GeometryVariable.h"

#include "Enums.h"
#include "Shapes/Cube.h"

enum BuildingVariableType {
	EMPTY,
	LOT,
	BASE,
	TOWER,
	ROOF,
	BLOCK,
	CUBE,
	HOUSE,
	URBANHOUSE,
	INDUSTRIALBUILDING
};

//class GeometryVariable;
class BuildingLot;

class BuildingVariable {
public:

	BuildingVariableType vtype;
	buildingStyle style;

	GeometryVariable *variable;

	int currentLOD;
	
	void getGeometry(std::vector<Cube> &out);
	virtual void getNextProductions(std::list<BuildingVariable> &out);
	virtual void setVariableData(GeometryVariable *newVar);

	BuildingVariable(BuildingLot *lot);
	BuildingVariable(BuildingVariableType type);
	BuildingVariable(BuildingVariableType type, BuildingVariable *source);
	BuildingVariable();
	~BuildingVariable();
};

