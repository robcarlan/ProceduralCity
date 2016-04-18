#pragma once
#include "RoadGeometry.h"
#include "Point.h"
#include <list>

class RoadGeometry;
typedef std::shared_ptr<RoadGeometry> roadPtr;

//Represents an intersection 
class IntersectionGeometry {
private:
	float pi, halfpi, twopi, piandahalf;
	int numIntersections;
	bool valid;

	Point intersectionPoint;
	std::list<roadPtr> connected;
	std::list<float> angles;
	std::pair<roadPtr, float> getBest(float angle, bool searchClockwise);
	bool getNextSide(float startAngle, float endAngle, bool isClockwise);
	bool isAbove(float angle);
	bool isBelow(float angle);
	float getAngleBetween(float angleStart, float angleEnd, bool clockwise);

public:
	int getNumIntersections();
	Point getIntersectionPoint();
	bool connectRoad(roadPtr toAdd);
	void removeRoad(RoadGeometry* toRemove);
	std::pair<roadPtr, bool> getAntiClockwise(float angle, bool clockwise);
	std::pair<roadPtr, bool> getClockwise(float angle, bool clockwise);
	std::list<roadPtr> getIntersections();
	void flagInvalid();
	bool isValid();

	IntersectionGeometry(const RoadIntersection &toCopy);
	IntersectionGeometry();
	~IntersectionGeometry();
};

typedef std::shared_ptr<IntersectionGeometry> intersectionPtr;