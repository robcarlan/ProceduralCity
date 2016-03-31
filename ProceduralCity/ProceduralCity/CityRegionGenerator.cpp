#include "CityRegionGenerator.h"

std::vector<BuildingRegion> CityRegionGenerator::createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections) {
	//do some crazy algrothim
	//Loop left and right over every intersection. (Over every road. Follow a left / right path). Loop identified by (intersection, index). 
	// -> we can store where there aren't loops -> we can check these later?
	//Keep looping until max_edges, if we loop mark that as a loop and continue.

	leftVisited = std::map<roadPtr, bool>();
	rightVisited = std::map<roadPtr, bool>();
	std::vector<BuildingRegion> out = std::vector<BuildingRegion>();

	std::list<roadPtr> traversing = std::list<roadPtr>();
	std::list<bool> direction = std::list<bool>();
	std::list<bool> side = std::list<bool>();

	bool sideToStartSearch = true;

	//TODO :: SIMPLE STREETS HAVE TWO ROADS THE SAME AT AN INTERSECTION

	while (true) {
		BOOST_FOREACH(const roadPtr roadStart, roads) {
			if (roadStart->getStart()->getIntersectionPoint() == Point(900, 800))
				qDebug() << "The start";
			if (!hasVisited(sideToStartSearch, roadStart)) {
				int pathLength = 1;
				bool found = false;
				//true -> right
				bool currentSide = sideToStartSearch;
				bool followingRoadForwards = true;

				//This edge isn't associated with a region. Start following it left for MAX_EDGE runs
				roadPtr currentRoad = roadStart;
				intersectionPtr intersectionCrossed = currentRoad->getStart();
				intersectionPtr startPtr = intersectionCrossed;

				while (pathLength <= maxEdgeTraversal) {
					//Check to see if we have already searched this node, or size of road is 1 (implies a convex region, so we won't add it).
					if (hasVisited(currentSide, currentRoad) || intersectionCrossed->getNumIntersections() < 2) {
					//	assert(isValidRegion(currentSide, currentRoad) == false); 
						
						//Mark all connected as should not be searched
						pathLength = maxEdgeTraversal + 1;
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
							auto next = (currentSide ? nextIntersection->getRightMost(angleHeading, currentSide)
								: nextIntersection->getLeftMost(angleHeading, currentSide));
							traversing.push_back(currentRoad);
							direction.push_back(followingRoadForwards);
							side.push_back(currentSide);
							//TODO :: Fails because of two seperate roads with the same start and end vertices.
							currentRoad = next.first;
							currentSide = next.second;
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
					out.push_back(BuildingRegion(traversing, direction));

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

	//Need to 'push' in to allow for roads thickness
	//visited looping yeahh

	//Apply Street Widths
	return out;
}

bool CityRegionGenerator::hasVisited(bool side, roadPtr traversing) {
	if (side) return !(rightVisited.find(traversing) == rightVisited.end());
	else return !(leftVisited.find(traversing) == leftVisited.end());
}

bool CityRegionGenerator::isValidRegion(bool side, roadPtr traversing) {
	assert(hasVisited(side, traversing));
	if (side) return rightVisited[traversing];
	else return leftVisited[traversing];
}

void CityRegionGenerator::flagRoads(std::list<roadPtr> traversing, std::list<bool> side, bool flagFound) {
	std::list<bool>::iterator sideItr = side.begin();
	std::list<roadPtr>::iterator roadItr = traversing.begin();

	if (!flagFound) return;

	//Mark the explored edges as found or not
	while (sideItr != side.end()) {
		(*sideItr ? rightVisited : leftVisited)[*roadItr] = flagFound;
		sideItr++;
		roadItr++;
	}
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
