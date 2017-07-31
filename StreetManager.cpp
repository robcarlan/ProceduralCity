#include "StreetManager.h"

const int StreetManager::numSubdivide = 5;
const float StreetManager::nearbyIntersectionRange = 2.0f;
const float StreetManager::angleCurveThreshold = 1.0f;

StreetManager::BOOST_POINT StreetManager::toBoostPoint(Point val) {
	return  boost::geometry::model::d2::point_xy<float>(val.x(), val.y());
}

StreetManager::BOOST_SEGMENT StreetManager::toBoostSegment(Road *val) {
	return boost::geometry::model::segment<BOOST_POINT>(toBoostPoint(val->getStart()), toBoostPoint(val->getEnd()));
}

int StreetManager::roadCount() {
	return roadTree.size();
}

int StreetManager::vertCount() {
	return intersectionTree.size();
}

StreetManager::StreetManager(Point bounds, CityView2D *view) {
	roadList = std::list<Road*>();
	intersectionsList = std::list<RoadIntersection*>();

	this->bounds = bounds;
	this->view = view;
}

StreetManager::StreetManager() {
}

StreetManager::~StreetManager() {
}

void StreetManager::reset() {
	roadTree.clear();
	intersectionTree.clear();

	view->Reset(bounds);

	// Clear memory
	while (!roadList.empty()) {
		delete roadList.front();
		roadList.pop_front();
	}

	while (!intersectionsList.empty()) {
		delete intersectionsList.front();
		intersectionsList.pop_front();
	}

}

void StreetManager::setView(CityView2D * view) {
	this->view = view;
}

//Simple search on intersections to find the closest intersection
StreetManager::intersectionRec* StreetManager::getClosest(Point queryPoint, std::vector<intersectionRec>& nearby) {
	StreetManager::intersectionRec* smallest = nearby.begin().operator->();

	for (std::vector<intersectionRec>::iterator it = nearby.begin(); it != nearby.end(); it++) {
		if (queryPoint.getDistanceSq(it->first) < queryPoint.getDistanceSq(smallest->first) && !queryPoint.isWithinBounds(it->first, 1.0f))
			smallest = it.operator->();
	}

	return smallest;
}

void StreetManager::getNearbyVertices(Point queryPoint, double radius, std::vector<RoadIntersection*>& nearby) {

	std::vector<intersectionIndex> result;

	//Perform knn to find 3 nearest neighbours.
	intersectionTree.query(boost::geometry::index::nearest(toBoostPoint(queryPoint), 5), std::back_inserter(result));

	for (std::vector<intersectionIndex>::iterator it = result.begin(); it != result.end(); it++) {
		Point pt = it->second->location;

		//The result is only knn, so may not be within desired bounds at all.
		if (queryPoint.isWithinBounds(pt, radius))
			nearby.push_back(it->second);
	}
}

void StreetManager::getIntersectingEdges(Road edge, std::vector<intersectionRec>& nearby) {

	std::vector<roadIndex> result;

	//Query road tree to find intersects with edge. Store in result.
	roadTree.query(boost::geometry::index::intersects(toBoostSegment(&edge)), std::back_inserter(result));

	for (std::vector<roadIndex>::iterator it = result.begin(); it != result.end(); it++) {
		std::unique_ptr<QPointF> intersectPoint(new QPointF);
		QLineF::IntersectType intersectType = it->second->intersect(edge, intersectPoint.get());

		//Find all intersect points, check whether they are bounded and at least some distance away from the start.
		if (intersectType == QLineF::IntersectType::BoundedIntersection)
			if (!edge.getStart().isWithinBounds(*intersectPoint, 1.0f))
				nearby.push_back(intersectionRec(Point(*intersectPoint), it->second));
	}
}

RoadIntersection * StreetManager::getExistingIntersections(Road * query, Road * target) {
	Point roadEnd = query->getEnd();

	for (std::vector<RoadIntersection*>::iterator it = target->intersections.begin(); it != target->intersections.end(); it++) {
		//Look for intersection within a threshold.
		Point intersectionVal = (*it)->location;

		if (abs(roadEnd.x() - intersectionVal.x()) <= nearbyIntersectionRange && abs(roadEnd.y() - intersectionVal.y()) <= nearbyIntersectionRange) {
			return (*it);
		}

	}

	return nullptr;
}

bool StreetManager::shouldSubdivide(Road * road) {

	//TODO ::
	return false;
	//Subdivide either the whole road, or the ends.
	//Ensure subdivided still is a legal road
	//Test angle diference (plus if it is a manhattan?) between source and target

	//Determines if we should subdivide current road based on curvature. 
	//Currently only main roads are curved
	if (road->parent == nullptr || road->rType != roadType::MAINROAD) return false;
	//if (road->isBranch) return false;
	float angleDif = abs(road->parent->getAngle() - road->getAngle());
	
	//Close to 90 -> not worth culling
	if (abs(angleDif - 90.0f * (M_PI / 180.0f)) < 0.01f) return true;

	return (angleDif > angleCurveThreshold);
}

void StreetManager::subdivideRoad(Road * road) {
	//Link new generated roads together, make sure final road links to incident, if it exists.
	bool isIncident = road->roadEndIntersection != nullptr;
	RoadIntersection *final = road->roadEndIntersection;
	//Divide road every set length. 
	int numSegments = road->length() / (float)numSubdivide;
	float xPos[numSubdivide];
	float yPos[numSubdivide];

	for (int i = 0; i < numSubdivide; i++) {
		//Constant angle change
		float factor = i * (1.0f / (float)numSubdivide);
		xPos[i] = (road->x2() - road->x1()) * factor * factor + road->x1();
		yPos[i] = (road->y2() - road->y1()) * factor + road->y1();
	}

	Road *prev = road->parent;
	//Link up each segment, add to structure
	for (int i = 0; i < numSubdivide; i++) {
		Road * segment; 
		RoadIntersection * end;
		if (i == numSubdivide - 1 ) {
			//Final index
			//Create such that road ends at originalo end destination
			segment = new Road(Point(xPos[i], yPos[i]), final->pos(), prev, road->rType);
			end = road->roadEndIntersection;
			road->roadEndIntersection->removeRoad(road);
			road->roadEndIntersection->attachRoad(segment);
		}
		else {
			//Intermediate index
			//Create road to join onto next determined position
			segment = new Road(Point(xPos[i], yPos[i]), Point(xPos[i + 1], yPos[i + 1]), prev, road->rType);
			end = new RoadIntersection(Point(xPos[i + 1], yPos[i + 1]), segment);
		}

		RoadIntersection * start;
		if (i == 0) start = road->roadStartIntersection;
		else start = prev->roadEndIntersection;

		segment->addStartIntersection(start);
		segment->addEndIntersection(end);
		end->attachRoad(segment);
		start->attachRoad(segment);

		insertRoad(segment);
		if (i < numSubdivide - 1) insertIntersection(end); //Maybe not on last!
		prev = segment;
	}

	//Paint subdivided red please

	
}

void StreetManager::branchRoad(RoadIntersection *start, Road *newRoad) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);
	
	//Create the new intersection, add to structure
	//Automatically connects correctly to road
	RoadIntersection *newIntersection = new RoadIntersection(newRoad->getEnd(), newRoad);
	newRoad->setIsBranch(true);

	if (shouldSubdivide(newRoad)) {
		subdivideRoad(newRoad);
		insertIntersection(newRoad->roadEndIntersection);
	} else {
		//Add new road into the structure
		insertRoad(newRoad);
		insertIntersection(newIntersection);
	}
}

void StreetManager::connectToExistingIntersection(RoadIntersection * start, Road * newRoad, RoadIntersection * crossing) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);
	assert(newRoad->getEnd().getDistance(crossing->location) < 6.0f);

//	newRoad->setEnd(crossing->location);
	//newRoad->addStartIntersection(start);
	newRoad->addEndIntersection(crossing);
	crossing->attachRoad(newRoad);

	//Add new road into the structure, connect to existing crossing
	if (shouldSubdivide(newRoad)) {
		subdivideRoad(newRoad);
	} else {
		insertRoad(newRoad);
	}
}

void StreetManager::connectToNewIntersection(RoadIntersection * start, Road * newRoad, Road * target) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);

	//Add new road into structure - already added in streetgen lol
	//newRoad->addStartIntersection(start);

	//Create new Intersection, attaches to road in constructor
	RoadIntersection *crossing = new RoadIntersection(Point(newRoad->getEnd()), newRoad, target);

	if (shouldSubdivide(newRoad)) {
		subdivideRoad(newRoad);
	}
	else {
		insertIntersection(crossing);
		insertRoad(newRoad);
	}

}

void StreetManager::connectToRoad(RoadIntersection * start, Road * toAdd, Road * target) {
	RoadIntersection *intersection = getExistingIntersections(toAdd, target);

	if (intersection != nullptr)
		connectToExistingIntersection(start, toAdd, intersection);
	else	//No nearby crossings exist, so lets add a new one
		connectToNewIntersection(start, toAdd, target);
}

void StreetManager::insertRoad(Road * toAdd) {
	//Rtree
	roadTree.insert(std::make_pair(toBoostSegment(toAdd), toAdd));
	//To manage memory afterwards
	roadList.push_front(toAdd);
	view->addRoad(toAdd);
}

void StreetManager::insertIntersection(RoadIntersection * toAdd) {
	//Rtree
	intersectionTree.insert(std::make_pair(toBoostPoint(toAdd->location), toAdd));	
	//In order to manage memory afterwards
	intersectionsList.push_front(toAdd);
	view->addIntersection(toAdd);
}

std::list<Road*> StreetManager::getRoads() {
	return roadList;
}

std::list<RoadIntersection*> StreetManager::getIntersections() {
	return intersectionsList;
}
