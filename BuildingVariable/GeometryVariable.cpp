#include "GeometryVariable.h"

GeometryVariable::GeometryVariable() {
	cubes = std::vector<Cube>();
	seed = 0;
}

GeometryVariable::~GeometryVariable() {
}

int GeometryVariable::getSeed() {
	return seed;
}

void GeometryVariable::addBoundingCube(float heightStart, float heightEnd, Point origin, float width, float depth, float rot) {

	QVector3D centrePos;
	centrePos.setZ(heightStart);
	centrePos.setX(origin.x());
	centrePos.setY(origin.y());

	QVector3D scale;
	scale.setZ(heightEnd - heightStart);
	scale.setY(depth);
	scale.setX(width);

	Cube Geometry = Cube(centrePos, scale, rot);
	this->rot = rot;

	cubes.push_back(Geometry);
}

void GeometryVariable::getGeometry(std::vector<Cube>& out) {
	for (auto itr = cubes.begin(); itr != cubes.end(); itr++) {
		out.push_back(*itr);
	}
}

buildingStyle GeometryVariable::getStyle() {
	return style;
}

void GeometryVariable::setSeed(int seed) {
	this->seed = seed;
}

int GeometryVariable::getLOD() {
	return LOD;
}