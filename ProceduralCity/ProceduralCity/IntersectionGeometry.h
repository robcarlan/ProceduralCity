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
	std::pair<roadPtr, float> getBest(float angle, bool scanRight);
	bool getNextSide(float startAngle, float endAngle, bool sideBefore);
	bool isAbove(float angle);
	bool isBelow(float angle);
	float getAngleBetween(float angleStart, float angleEnd, bool clockwise);

public:
	int getNumIntersections();
	Point getIntersectionPoint();
	void connectRoad(roadPtr toAdd);
	void removeRoad(RoadGeometry* toRemove);
	std::pair<roadPtr, bool> getLeftMost(float angle, bool side);
	std::pair<roadPtr, bool> getRightMost(float angle, bool side);
	std::list<roadPtr> getIntersections();
	void flagInvalid();
	bool isValid();

	IntersectionGeometry(const RoadIntersection &toCopy);
	IntersectionGeometry();
	~IntersectionGeometry();
};

typedef std::shared_ptr<IntersectionGeometry> intersectionPtr;