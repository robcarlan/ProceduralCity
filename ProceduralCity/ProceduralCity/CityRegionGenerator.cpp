#include "CityRegionGenerator.h"

std::vector<BuildingRegion> CityRegionGenerator::createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections) {
	//do some crazy algrothim
	//Loop left and right over every intersection. (Over every road. Follow a left / right path). Loop identified by (intersection, index). 
	// -> we can store where there aren't loops -> we can check these later?
	//Keep looping until max_edges, if we loop mark that as a loop and continue.


	anticlockwiseVisited = std::map<roadPtr, bool>();
	clockwiseVisited = std::map<roadPtr, bool>();
	std::vector<BuildingRegion> out = std::vector<BuildingRegion>();

	std::list<roadPtr> traversing = std::list<roadPtr>();
	std::list<bool> direction = std::list<bool>();
	std::list<bool> side = std::list<bool>();

	bool sideToStartSearch = true;
	bool followingRoadForwards = true;
	bool searchComplete = false;

	//TODO :: SIMPLE STREETS HAVE TWO ROADS THE SAME AT AN INTERSECTION

	while (true) {
		BOOST_FOREACH(const roadPtr roadStart, roads) {
			if (roadStart->getStart()->getIntersectionPoint() == Point(900, 800))
				qDebug() << "The start";
			if (!hasVisited(sideToStartSearch, followingRoadForwards, roadStart)) {
				int pathLength = 1;
				bool found = false;
				searchComplete = false;
				//true -> clockwise
				bool clockwise = sideToStartSearch;

				//This edge isn't associated with a region. Start following it left for MAX_EDGE runs
				roadPtr currentRoad = roadStart;
				intersectionPtr intersectionCrossed = currentRoad->getStart();
				intersectionPtr startPtr = intersectionCrossed;

				while (pathLength <= maxEdgeTraversal && !searchComplete) {
					//Check to see if we have already searched this node, or size of road is 1 (implies a convex region, so we won't add it).
					if (hasVisited(clockwise, followingRoadForwards, currentRoad) || intersectionCrossed->getNumIntersections() < 2) {
					//	assert(isValidRegion(currentSide, currentRoad) == false); 
						
						//Mark all connected as should not be searched
						searchComplete = true;
					}
					else {
						//Keep searching, as these edges are unexplored
						auto next = currentRoad->getOtherEnd(intersectionCrossed);
						intersectionPtr nextIntersection = next.first;
						followingRoadForwards = next.second;

						if (startPtr == nextIntersection) {
							//Loop found, update l/r visited
							found = true;
							break;
						}
						else {
							//Keep searching, find the next road at the intersection
							//Take the OPPOSITE ANGLE OMG
							float angleHeading = !followingRoadForwards ? currentRoad->getAngleToEnd() : currentRoad->getAngleToStart();
							auto next = (clockwise ? nextIntersection->getClockwise(angleHeading, clockwise)
								: nextIntersection->getAntiClockwise(angleHeading, clockwise));
							traversing.push_back(currentRoad);
							direction.push_back(followingRoadForwards);
							side.push_back(followingRoadForwards ? clockwise : !clockwise);
							//TODO :: Fails because of two seperate roads with the same start and end vertices.
							currentRoad = next.first;
							//switches every iteration.
							//clockwise = next.second;
							assert(currentRoad != nullptr);
							intersectionCrossed = nextIntersection;
							pathLength++;
						}
					}
				}

				if (pathLength > maxEdgeTraversal) {
					found = false;
				}

				flagRoads(traversing, side, found);

				if (found)
					out.push_back(BuildingRegion(traversing, direction, side));

				//Clear variables
				pathLength = 1;
				traversing.clear();
				direction.clear();
				side.clear();
			}
		}

		if (sideToStartSearch) sideToStartSearch = false;
		else break;
	}

	//We have to remove the longest item, as this iterates over the outside
	auto itr = out.begin();
	auto toRemove = out.begin();
	int maxLength = 0;
	for (itr; itr != out.end(); itr++) {
		if (itr->getPoints().size() > maxLength) {
			maxLength = itr->getPoints().size();
			toRemove = itr;
		}
	}

	out.erase(toRemove);

	//Need to 'push' in to allow for roads thickness
	//visited looping yeahh

	//Apply Street Widths
	return out;
}

bool CityRegionGenerator::hasVisited(bool side, bool forwards, roadPtr traversing) {
	//Clockwise + Forward or AntiCW + Backwards = search clockwise
	if ((side && forwards) || (!side && !forwards)) return !(clockwiseVisited.find(traversing) == clockwiseVisited.end());
	else return !(anticlockwiseVisited.find(traversing) == anticlockwiseVisited.end());
}

bool CityRegionGenerator::isValidRegion(bool side, bool forwards, roadPtr traversing) {
	assert(hasVisited(side, forwards, traversing));
	if ((side && forwards) || (!side && !forwards)) return clockwiseVisited[traversing];
	else return anticlockwiseVisited[traversing];
}

void CityRegionGenerator::flagRoads(std::list<roadPtr> traversing, std::list<bool> side, bool flagFound) {
	std::list<bool>::iterator sideItr = side.begin();
	std::list<roadPtr>::iterator roadItr = traversing.begin();

	if (!flagFound) return;

	//Mark the explored edges as found or not
	while (sideItr != side.end()) {
		(*sideItr ? clockwiseVisited : anticlockwiseVisited)[*roadItr] = flagFound;
		sideItr++;
		roadItr++;
	}
}

Point CityRegionGenerator::getRegionPoint(roadPtr r1, roadPtr r2, intersectionPtr intersection, bool cw1, bool cw2) {
	return Point();
}

std::vector<BuildingLot> CityRegionGenerator::subdivideRegions(std::list<BuildingRegion> const buildings) {
	return std::vector<BuildingLot>();
}

void CityRegionGenerator::setMaxEdges(int max) {
	maxEdgeTraversal = max;
}

CityRegionGenerator::CityRegionGenerator() {
	maxEdgeTraversal = 1024;
}


CityRegionGenerator::~CityRegionGenerator() {
}
