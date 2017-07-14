#pragma once
#include <QVector3D>

class Cube {

	QVector3D baseOrigin;
	QVector3D scale;
	QVector3D centre;
	float rot;

public:
	QVector3D getTop();
	QVector3D getBase();
	QVector3D getScale();
	QVector3D getCentre();
	float getRot();

	Cube(const QVector3D &base, const QVector3D &scale, float rot);

	Cube();
	~Cube();
};

