#pragma once
#include <map>
#include <list>
#include "IntersectionGeometry.h"
#include "RoadGeometry.h"
#include <qdebug.h>

#ifndef Q_MOC_RUN 
#include <boost\foreach.hpp>
#endif

class SimpleStreetGeometryCreator {
	void createRoadFromIntersections(intersectionPtr start, intersectionPtr end, Road * toCopy, std::list<roadPtr> *newRoads);
public:
	std::pair<std::list<roadPtr>, std::list<intersectionPtr>> toGeometry(std::list<RoadIntersection*> &intersections, std::list<Road*> &roads);

	SimpleStreetGeometryCreator();
	~SimpleStreetGeometryCreator();
};

