#pragma once
#include "Enums.h"
#include "Shapes/Cube.h"

#include <QRect>

class BuildingLot;
class BuildingLSystem;

//Represents a building made from various basic building pieces
class Building {
	buildingStyle style;
	BuildingLSystem *system;

	std::vector<Cube> geometryCubes;

public:

	QRect cosntructionArea;
	std::vector<Cube>& getCubes();
	void updateGeometry(int desiredLOD);
	Building(BuildingLot *lot);
	Building();
	~Building();
};

