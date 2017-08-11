#pragma once
#include "QRect.h"

#include <vector>

class Cube;
class BuildingLot;
class BuildingLSystem;

enum buildingStyle;

//Represents a building made from various basic building pieces
class Building {
	buildingStyle style;
	BuildingLSystem *system;

	std::vector<Cube> geometryCubes;

public:

	QRect constructionArea;
	std::vector<Cube>& getCubes();
	void updateGeometry(int desiredLOD);
	Building(BuildingLot *lot);
	Building();
	~Building();
};

