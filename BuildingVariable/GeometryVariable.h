#pragma once
#include "Point.h"

#include <vector>
#include <list>

class Cube;
class BuildingVariable;

enum buildingStyle;

class GeometryVariable {
protected:
	buildingStyle style;
	float rot;
	int seed;
	int LOD;

	float heightStart, heightEnd, width, depth;
	Point origin;

	std::vector<Cube> cubes;
	void addBoundingCube(float heightStart, float heightEnd, Point origin, float width, float depth, float rot);

public:

	virtual bool getNextProductions(std::list<BuildingVariable> &out) = 0;
	virtual void getGeometry(std::vector<Cube> &out);

	buildingStyle getStyle();
	void setSeed(int seed);
	int getSeed();
	int getLOD();

	GeometryVariable();
	~GeometryVariable();
};

