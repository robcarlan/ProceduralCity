#pragma once
#include <list>
#include <math.h>
#include "RoadVariable.h"
#include "IntersectionGeometry.h"
#include "Road.h"
#include "Point.h"
#include "Enums.h"

class IntersectionGeometry;
typedef std::shared_ptr<IntersectionGeometry> intersectionPtr;

//Represents a simple road - i.e. a line going from one intersection to another
class RoadGeometry {
protected:
	float startElevation;
	float endElevation;
	float angle;
	float angleAway;
	Point start, end;
	intersectionPtr startIntersect, endIntersect;
	bool valid;

	void calcAngles();
	void removeIntersection(intersectionPtr toRemove);
public:
	roadType rtype;
	roadType getRoadType();

	float getAngleToEnd();
	float getAngleToStart();

	std::pair<intersectionPtr, bool> getOtherEnd(intersectionPtr toCheck) const;
	intersectionPtr getStart();
	intersectionPtr getEnd();
	void setStart(intersectionPtr start);
	void setEnd(intersectionPtr end);
	void disconnectRoad();
	void flagInvalid();
	bool isValid();

	RoadGeometry(const Road &toCopy);
	RoadGeometry();
	~RoadGeometry();
};

typedef std::shared_ptr<RoadGeometry> roadPtr;