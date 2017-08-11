#include "Building.h"
#include "BuildingLot.h"
#include "LSystem/BuildingLSystem.h"

#include "Enums.cpp"

std::vector<Cube>& Building::getCubes() {
	return geometryCubes;
}

void Building::updateGeometry(int desiredLOD) {
	//Updates LSystem to desired state
	system->setDesiredLOD(desiredLOD);
	system->Run();

	//Now update cube buffer
	system->getGeometry(geometryCubes);
}

Building::Building(BuildingLot* lot) {
	this->style = lot->getStyle();
	system = new BuildingLSystem(lot);
	Point start = lot->getCentroid();

	system->initialiseLSystem();
	updateGeometry(1);
}

Building::Building() {
}

Building::~Building() {
	delete system;
}
