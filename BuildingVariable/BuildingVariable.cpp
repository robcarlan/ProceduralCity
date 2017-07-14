#include "BuildingVariable.h"
#include "BuildingLot.h"

void BuildingVariable::getGeometry(std::vector<Cube>& out) {
	variable->getGeometry(out);
}

void BuildingVariable::getNextProductions(std::list<BuildingVariable>& out) {
	variable->getNextProductions(out);
}

void BuildingVariable::setVariableData(GeometryVariable * newVar) {
	variable = newVar;
}

//Create LOT type
BuildingVariable::BuildingVariable(BuildingLot * lot) {
	this->variable = dynamic_cast<GeometryVariable*>(new LotVariable(lot));
	this->style = lot->getStyle();
	currentLOD = 0;
	this->vtype = BuildingVariableType::LOT;
}

BuildingVariable::BuildingVariable(BuildingVariableType type) {
	this->vtype = type; 
}

BuildingVariable::BuildingVariable(BuildingVariableType type, BuildingVariable * source) {
	this->vtype = type;

	//Create new variable from source, set parameters etc.
}

//TODO :: Empty
BuildingVariable::BuildingVariable() {
	this->vtype = BuildingVariableType::EMPTY;
}


BuildingVariable::~BuildingVariable() {
	switch (vtype) {
		//delete necessary pointers
	}
}
