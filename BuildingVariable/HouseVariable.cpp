#include "HouseVariable.h"
#include "CubeVariable.h"
#include "BuildingVariable.h"

#include "Parameters.h"

HouseVariable::HouseVariable() {
}

HouseVariable::HouseVariable(float heightStart, float heightEnd, Point origin, float width, float depth, float rot) {
	LOD = 1;
	addBoundingCube(heightStart, heightEnd, origin, width, depth, rot);

	this->heightEnd = heightEnd;
	this->heightStart = heightStart;
	this->origin = origin;
	this->width = width;
	this->depth = depth;
	this->rot = rot;
}

HouseVariable::~HouseVariable() {
}

bool HouseVariable::getNextProductions(std::list<BuildingVariable> &out) {
	//Subdivide house into a few cubes

	int maxDivisions = floor<int>(this->width / minHousePieceSize);
	if (maxDivisions > 3) maxDivisions = 3;
	int numDivisions = rand() % maxDivisions;


	if (maxDivisions == 1) {
		CubeVariable * cube = new CubeVariable(heightStart, heightEnd, origin, width * 0.95, depth, rot);
		BuildingVariable base = BuildingVariable(BuildingVariableType::CUBE);
		base.setVariableData(dynamic_cast<GeometryVariable*>(cube));
		cube->setSeed(seed);
		out.push_back(base);
	}
	else if (maxDivisions == 2) {

		float randSplit = getBetween(rand(), 0.4f, 0.6f);
		float depth1 = getBetween(rand(), depth * 0.7f, depth);
		float depth2 = getBetween(rand(), depth * 0.7f, depth);
		float w1 = width * randSplit;
		float w2 = width * (1.0f - randSplit);

		Point origin1 = origin;
		origin1.setX(origin.x() - width / 2); //Now origin x is at the leftmost boundary
		Point origin2 = origin1;
		origin2.setX(origin1.x() + w1 + w2 / 2);
		origin1.setX(origin1.x() + w1 / 2); //Move origin 1 to correct position

		CubeVariable * cube1 = new CubeVariable(heightStart, heightEnd, origin1, w1, depth1, rot);
		CubeVariable * cube2 = new CubeVariable(heightStart, heightEnd, origin2, w2, depth2, rot);
		BuildingVariable base1 = BuildingVariable(BuildingVariableType::CUBE);
		BuildingVariable base2 = BuildingVariable(BuildingVariableType::CUBE);
		base1.setVariableData(dynamic_cast<GeometryVariable*>(cube1));
		base2.setVariableData(dynamic_cast<GeometryVariable*>(cube2));
		cube1->setSeed(seed);
		cube2->setSeed(seed);
		out.push_back(base1);
		out.push_back(base2);
	}
	else if (maxDivisions == 3) {

		float randSplit1 = getBetween(rand(), 0.3f, 0.4f);
		float randSplit2 = getBetween(rand(), 0.6f, 0.8f);
		float w1 = width * randSplit1;
		float w2 = width * (randSplit2 - randSplit1);
		float w3 = width * (1.0f - randSplit2);
		float depth1 = getBetween(rand(), depth * 0.7f, depth);
		float depth2 = getBetween(rand(), depth * 0.7f, depth);
		float depth3 = getBetween(rand(), depth * 0.7f, depth);

		//Create 3 boxes given by these params
		Point origin1 = origin;
		origin1.setX(origin.x() - width / 2); //Now origin x is at the leftmost boundary
		Point origin2 = origin1;
		origin2.setX(origin1.x() + w1 + w2 / 2);
		Point origin3 = origin1;
		origin3.setX(origin1.x() + w1 + w2 + w3 / 2);
		origin1.setX(origin1.x() + w1 / 2); //Move origin 1 to correct position

		CubeVariable * cube1 = new CubeVariable(heightStart, heightEnd, origin1, w1, depth1, rot);
		CubeVariable * cube2 = new CubeVariable(heightStart, heightEnd, origin2, w2, depth2, rot);
		CubeVariable * cube3 = new CubeVariable(heightStart, heightEnd, origin3, w3, depth3, rot);
		BuildingVariable base1 = BuildingVariable(BuildingVariableType::CUBE);
		BuildingVariable base2 = BuildingVariable(BuildingVariableType::CUBE);
		BuildingVariable base3 = BuildingVariable(BuildingVariableType::CUBE);
		base1.setVariableData(dynamic_cast<GeometryVariable*>(cube1));
		base2.setVariableData(dynamic_cast<GeometryVariable*>(cube2));
		base3.setVariableData(dynamic_cast<GeometryVariable*>(cube3));
		cube1->setSeed(seed);
		cube2->setSeed(seed);
		cube3->setSeed(seed);
		out.push_back(base1);
		out.push_back(base2);
		out.push_back(base3);
	}

	return true;
}
