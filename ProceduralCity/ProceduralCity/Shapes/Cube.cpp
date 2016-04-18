#include "Cube.h"



QVector3D Cube::getTop() {
	return baseOrigin + QVector3D(0, scale.y(), 0);
}

QVector3D Cube::getBase() {
	return baseOrigin;
}

QVector3D Cube::getScale() {
	return scale;
}

QVector3D Cube::getCentre() {
	return centre;
}

float Cube::getRot() {
	return rot;
}

Cube::Cube(const QVector3D & base, const QVector3D & scale, float rot) :
	baseOrigin(base), scale(scale), rot(rot) {
	centre = baseOrigin;
	centre.setZ(baseOrigin.z() + scale.z() / 2);
}

Cube::Cube() {
}


Cube::~Cube() {
}
