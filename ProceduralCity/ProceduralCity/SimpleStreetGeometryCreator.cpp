#include "SimpleStreetGeometryCreator.h"

std::pair<std::list<roadPtr>, std::list<intersectionPtr> > SimpleStreetGeometryCreator::toGeometry(std::list<RoadIntersection*> &intersections, std::list<Road*> &roads) {

	std::list<roadPtr> newRoads;
	std::list<intersectionPtr> newIntersections;

	std::map<RoadIntersection*, std::shared_ptr<IntersectionGeometry>> toNewIntersections;

	//Add a map between old intersections and the new ones, so it is easy to lookup when traversing roads.
	BOOST_FOREACH(RoadIntersection *key, intersections) {
		auto newIntersect = intersectionPtr(new IntersectionGeometry(*key));
		toNewIntersections.insert(std::make_pair(key, newIntersect));
		newIntersections.push_front(newIntersect);
	}

	BOOST_FOREACH(Road *roadItr, roads) {
		auto roadIntersections = roadItr->getIntersections();

		if (roadIntersections->size() == 2) {
			//Road is already simple, so we can simply copy over.
			auto start = roadItr->roadStartIntersection;
			auto end = roadItr->roadEndIntersection;
			auto startResult = toNewIntersections.find(start);
			auto endResult = toNewIntersections.find(end);

			if (!toNewIntersections.count(end)) {
				qDebug() << "KEY NOT FOUND";
				bool found = false;
				BOOST_FOREACH(RoadIntersection *key, intersections) {
					if (end == key) found = true;
				}
			}

			intersectionPtr newStart = startResult->second;
			intersectionPtr newEnd = endResult->second;
			createRoadFromIntersections(newStart, newEnd, roadItr, &newRoads);
		}
		else {
			//We need to split the road into several new ones - create by iterating through each intersection from start to end
			Point start = roadItr->getStart();
			int numToCreate = roadIntersections->size() - 1;
			std::vector<RoadIntersection*> sorted = *roadIntersections;
			std::sort(sorted.begin(), sorted.end(),
				[start](RoadIntersection* const& a, RoadIntersection* const& b) { return a->location.getDistanceSq(start) < b->location.getDistanceSq(start); });

			for (int i = 0; i < numToCreate; i++) {
				auto start = sorted[i];
				auto end = sorted[i + 1];
				createRoadFromIntersections(toNewIntersections[start], toNewIntersections[end], roadItr, &newRoads);
			}
		}
	}

	//Iterate over every road. If the road has 2 connected intersections, we are safe to copy this to our new format.
	//Otherwise, we need to split the road up into several parts.

	//Removes roads with dead ends
	bool changed = true;
	int roadsRemoved = 0;
	int intersectionsRemoved = 0;

	while (changed) {
		changed = false;

		for (auto iItr = newIntersections.begin(); iItr != newIntersections.end(); iItr++) {
			intersectionPtr ptr = *iItr;
			if (ptr->getNumIntersections() < 2) {
				if (ptr->getNumIntersections() == 1) {
					changed = true;
					//Remove this intersection, which in turn propogates to the attaching road.
					roadPtr toRemove = ptr->getIntersections().front();
					toRemove->disconnectRoad();
					toRemove->flagInvalid();
					ptr->flagInvalid();
				}
			}
		}
	}

	// TODO :: EFFICIENT REMOVAL OF CULLED ROADS FROM SET
	//Do we build the roads afterwards? Flag each road/intersection as valid, and return a new list? May be the best option.
	auto itrPtr = newIntersections.begin();

	while (itrPtr != newIntersections.end()) {
		if (itrPtr->get()->isValid()) {
			itrPtr++;
			intersectionsRemoved++;
		}
		else itrPtr = newIntersections.erase(itrPtr);
	}

	auto roadItr = newRoads.begin();

	while (roadItr != newRoads.end()) {
		if (roadItr->get()->isValid()) {
			roadItr++;
			roadsRemoved++;
		}
		else roadItr = newRoads.erase(roadItr);
	}

	qDebug() << roadsRemoved << " roads removed\n";
	qDebug() << intersectionsRemoved << " intersections removed\n";

//#ifdef DEBUG

	BOOST_FOREACH(roadPtr ptr, newRoads) {
		assert(ptr != nullptr);
		assert(ptr->getStart() != nullptr);
		assert(ptr->getEnd() != nullptr);
	}

	BOOST_FOREACH(intersectionPtr ptr, newIntersections) {
		assert(ptr != nullptr);
		assert(ptr->getNumIntersections() >= 2);
	}

	BOOST_FOREACH(roadPtr ptr, newRoads) {
		assert(ptr->getStart() != nullptr);
		assert(ptr->getEnd() != nullptr);
		assert(ptr->getOtherEnd(ptr->getOtherEnd(ptr->getStart()).first).first->getIntersectionPoint().getDistanceSq(ptr->getStart()->getIntersectionPoint()) < 0.1f);
	}

//#endif

	return std::make_pair(newRoads, newIntersections);
}

void SimpleStreetGeometryCreator::createRoadFromIntersections(intersectionPtr start, intersectionPtr end, Road * toCopy, std::list<roadPtr> *newRoads) {
	roadPtr newRoad;
	newRoad = roadPtr(new RoadGeometry(*toCopy));
	newRoad->setStart(start);
	newRoad->setEnd(end);
	start->connectRoad(newRoad);
	end->connectRoad(newRoad);
	newRoads->push_front(newRoad);
}

SimpleStreetGeometryCreator::SimpleStreetGeometryCreator() {
}

SimpleStreetGeometryCreator::~SimpleStreetGeometryCreator() {
}
