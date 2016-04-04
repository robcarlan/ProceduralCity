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
	std::list<float> angles = std::list<float>();
	std::list<bool> direction = std::list<bool>();
	std::list<bool> side = std::list<bool>();

	bool sideToStartSearch = true;
	bool followingRoadForwards = true;
	bool searchComplete = false;

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

				//This edge isn't associated with a region. Start following it for MAX_EDGE runs
				roadPtr currentRoad = roadStart;
				intersectionPtr intersectionCrossed = currentRoad->getStart();
				intersectionPtr startPtr = intersectionCrossed;

				while (pathLength <= maxEdgeTraversal && !searchComplete) {
					//Check to see if we have already searched this node, or size of road is 1 (implies a convex region, so we won't add it).
					auto next = currentRoad->getOtherEnd(intersectionCrossed);
					intersectionPtr nextIntersection = next.first;
					followingRoadForwards = next.second;
					float angleHeading = !followingRoadForwards ? currentRoad->getAngleToEnd() : currentRoad->getAngleToStart();

					if (hasVisited(clockwise, followingRoadForwards, currentRoad)) {
						//assert(isValidRegion(clockwise, followingRoadForwards, currentRoad) == false);
						
						//Mark all connected as should not be searched
						searchComplete = true;
					}
					else {
						//Keep searching, as these edges are unexplored

						if (startPtr == nextIntersection) {
							//Loop found, update l/r visited
							traversing.push_back(currentRoad);
							direction.push_back(followingRoadForwards);
							side.push_back(followingRoadForwards ? clockwise : !clockwise);
							angles.push_back(angleHeading);

							found = true;
							break;
						}
						else {
							//Keep searching, find the next road at the intersection
							//Take the OPPOSITE ANGLE OMG
							auto next = (clockwise ? nextIntersection->getClockwise(angleHeading, clockwise)
								: nextIntersection->getAntiClockwise(angleHeading, clockwise));
							traversing.push_back(currentRoad);
							direction.push_back(followingRoadForwards);
							side.push_back(followingRoadForwards ? clockwise : !clockwise);
							angles.push_back(angleHeading);
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
				
				if (found) flagRoads(traversing, side, found);
				if (found) {
					//Turn into a region
					auto bounds = toRegion(traversing, direction, side, angles);
					//Filter by size
					if (abs(BuildingRegion::getPolyArea(bounds)) > 150.0f)
						out.push_back(BuildingRegion(bounds, angles));
				}
				//Clear variables
				pathLength = 1;
				traversing.clear();
				direction.clear();
				side.clear();
				angles.clear();

				//Update followingForwards
				followingRoadForwards = true;
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

	//Mark the explored edges as found or not
	while (sideItr != side.end()) {
		(*sideItr ? clockwiseVisited : anticlockwiseVisited)[*roadItr] = flagFound;
		sideItr++;
		roadItr++;
	}
}

//'Pushes' in the point after road widths have been considered. I.e. the region vertex will not be exactly on the intersection itself.
Point CityRegionGenerator::getRegionPoint(roadPtr r1, roadPtr r2, intersectionPtr intersection, bool cw1, bool cw2, float angle1, float angle2) {
	bool r1Away = r1->getStart()->getIntersectionPoint() == intersection->getIntersectionPoint();
	bool r2Away = r2->getStart()->getIntersectionPoint() == intersection->getIntersectionPoint();
	float r1x = cosf(angle1);
	float r1y = sinf(angle1);
	float r2x = cosf(angle2);
	float r2y = sinf(angle2);
	float r1Width = r1->getWidth();
	float r2Width = r2->getWidth();
	r1x *= r1Width;
	r1y *= r1Width;
	r2x *= r2Width;
	r2y *= r2Width;

	assert(cw1 && !cw2 || !cw1 && cw2);

	//r1x = r1y = r2x = r2y = 0.0f;
	
	QLineF roadSide1 = QLineF(r1->getStart()->getIntersectionPoint().x(), r1->getStart()->getIntersectionPoint().y(),
		r1->getEnd()->getIntersectionPoint().x(), r1->getEnd()->getIntersectionPoint().y());
	if (cw1)
		roadSide1.translate(r1x, -r1y);
	else roadSide1.translate(-r1x, r1y);

	QLineF roadSide2 = QLineF(r2->getStart()->getIntersectionPoint().x(), r2->getStart()->getIntersectionPoint().y(),
		r2->getEnd()->getIntersectionPoint().x(), r2->getEnd()->getIntersectionPoint().y());

	if (!cw2)
		roadSide2.translate(r2x, -r2y);
	else roadSide2.translate(-r2x, r2y);

	//Check for nearby angle
	float dif = abs(angle1 - angle2);
	float tpi = boost::math::float_constants::two_pi;
	bool isParallel = ((abs(dif - tpi) < 0.01f) || (abs(tpi - dif) < 0.01f) || dif < 0.01f);

	if (isParallel) {
		//Too close
		//return intersection->getIntersectionPoint();
		return Point(intersection->getIntersectionPoint().x() + (cw1 ? r1x : -r1x), intersection->getIntersectionPoint().y() + (cw1 ? -r1y : r1y));
	}

	QPointF intersectPoint;
	auto intersectResult = roadSide1.intersect(roadSide2, &intersectPoint);

	//assert(intersectPoint.x() > 0 && intersectPoint.y() < 2048.0f);

	if (intersectResult == QLineF::IntersectType::NoIntersection) {
		//Shouldnt occur
		//return intersection->getIntersectionPoint();
	}
	return intersectPoint;
}

std::list<Point> CityRegionGenerator::toRegion(std::list<roadPtr>& roadList, std::list<bool>& travelDirection, std::list<bool>& side, std::list<float>& angles) {
	std::list<Point> bounds;
	bool clockwisePrev, clockwiseRoad;
	bool forwardPrev, forward;
	float anglePrev, angleNow;
	auto prevRoad = *roadList.rbegin();
	auto rItr = roadList.begin();
	auto dItr = travelDirection.begin();
	auto sItr = side.begin();
	auto aItr = angles.begin();
	auto intersection = *dItr ? (*rItr)->getEnd() : (*rItr)->getStart();

	//Start by using road[n-1], road[0]
	forwardPrev = travelDirection.back();
	clockwisePrev = side.back(); 
	clockwiseRoad = *sItr;
	anglePrev = angles.back();
	angleNow = *aItr;

	//Calculates points of intersection[n] using roads [n-1] and roads[n]
	bool first = true;
	while (rItr != roadList.end()) {
		auto intersection = *dItr ? (*rItr)->getStart() : (*rItr)->getEnd();

		forward = *dItr;
		clockwiseRoad = *sItr;
		angleNow = *aItr;

		//bounds.push_back(intersection->getIntersectionPoint());
		bounds.push_back(getRegionPoint(prevRoad, *rItr, intersection, forwardPrev ? !clockwisePrev : clockwisePrev, forward ? clockwiseRoad : !clockwiseRoad, anglePrev, angleNow));

		clockwisePrev = clockwiseRoad;
		forwardPrev = forward;
		prevRoad = *rItr;
		anglePrev = angleNow;

		rItr++;
		dItr++;
		sItr++;
		aItr++;
		first = false;
	}

	return bounds;
}

std::pair<std::list<BuildingRegion>&, std::list<BuildingRegion>&> CityRegionGenerator::filterRegions(std::list<BuildingRegion>& regions) {
	std::list<BuildingRegion> valid;
	std::list<BuildingRegion> invalid;
	return std::make_pair(valid, invalid);
}


void CityRegionGenerator::subdivideRegions(std::vector<BuildingRegion>& const buildings) {
	BOOST_FOREACH(BuildingRegion regionItr, buildings) {
		//Break into convex regions, if not already convex
		std::vector<BuildingLot> resultLots = std::vector<BuildingLot>();

		if (!regionItr.isConvex()) {
			auto convexLots = BuildingRegion::splitConvex(regionItr.getPoints());

			BOOST_FOREACH(std::list<Point> convexPoly, convexLots) {
				createLotsFromConvexPoly(convexPoly, resultLots);
			}
		}
		else {
			//Already convex
			createLotsFromConvexPoly(regionItr.getPoints(), resultLots);	
		}
		//Add to region
		regionItr.setLots(resultLots);
	}
}

//Recursive approach - split along edge w
void CityRegionGenerator::createLotsFromConvexPoly(std::list<Point>& bounds, std::vector<BuildingLot>& out) {
	auto result = std::vector<BuildingLot>();

	float buildingMinArea = 100.0f;
	float buildingMaxArea = 500.0f;

	float area = abs(BuildingRegion::getPolyArea(bounds));

	if (area < buildingMinArea)
		return;
	if (area < buildingMaxArea) {
		out.push_back(createLot(bounds));
		return;
	}
		
	//O.w. need to break down further. Find two edges to subdivide
	auto splitEdges = getLongestEdgePair(bounds);
	auto longestp1 = splitEdges.first.first;
	auto longestp2 = splitEdges.first.second;
	auto secondlongestp1 = splitEdges.second.first;
	auto secondlongestp2 = splitEdges.second.second;
	float lengthLongest = longestp1.getDistance(longestp2);
	float lengthSecondLongest = secondlongestp1.getDistance(secondlongestp2);

	//Calculate midpoint, with some offset
	static float randFactor = 0.4f;
	float offset = rand() * randFactor + (randFactor / 2.0f);
	Point midLongest;
	midLongest.setX(longestp1.x() + offset * (longestp2.x() - longestp1.x()));
	midLongest.setX(longestp1.y() + offset * (longestp2.y() - longestp1.y()));
	Point midSecondLongest;
	midLongest.setX(secondlongestp1.x() + offset * (secondlongestp2.x() - secondlongestp1.x()));
	midLongest.setX(secondlongestp1.y() + offset * (secondlongestp2.y() - secondlongestp1.y()));

	//Construct p1, p2
	auto polygon1 = std::list<Point>();
	polygon1.push_back(midLongest);

	auto pItr = bounds.begin();
	while (*pItr != longestp1) {
		pItr++;
	}

	//Keep adding to new polygon
	while (*pItr != secondlongestp2) {
		if (pItr == bounds.end())
			pItr = bounds.begin();

		polygon1.push_back(*pItr);
		pItr++;
	}

	polygon1.push_back(midSecondLongest);
	
	auto polygon2 = std::list<Point>();
	polygon2.push_back(midSecondLongest);

	//pItr starts at secondLongestp2
	while (*pItr != longestp2) {
		if (pItr == bounds.end())
			pItr = bounds.begin();

		polygon2.push_back(*pItr);
		pItr++;
	}

	polygon1.push_back(midLongest);

	//Recursively construct lots inside these polygons
	createLotsFromConvexPoly(polygon1, out);
	createLotsFromConvexPoly(polygon2, out);
}

std::pair<std::pair<Point, Point>, std::pair<Point, Point>> CityRegionGenerator::getLongestEdgePair(const std::list<Point>& bounds) {
	//Iterate through, keeping track of two max roads
	Point p1 = bounds.front();
	Point p1Prev = bounds.back();
	Point p2 = bounds.front();
	Point p2Prev = bounds.back();
	float d1 = 0.0f;
	float d2 = 0.0f;

	auto pPrev = bounds.end();
	auto pItr = bounds.begin();
	while (pItr != bounds.end()) {

		float dist = pItr->getDistanceSq(*pPrev);

		if (dist > d1) {
			//sift l1 to l2, update max
			p2 = p1;
			p2Prev = p1Prev;
			p1 = *pItr;
			p1Prev = *pPrev;
		}
		else if (dist > d2) {
			//Update d2
			p2 = *pItr;
			p2Prev = *pPrev;
		} 

		//Update 
		pPrev = pItr;
		pItr++;
	}

	//Calculate actual dist
	return std::make_pair(std::make_pair(p1, p1Prev), std::make_pair(p2, p2Prev));
}

BuildingLot CityRegionGenerator::createLot(std::list<Point> bounds) {
	//TODO :: Set required data, i.e. building params, pop density etc.
	return BuildingLot(bounds);
}

void CityRegionGenerator::setMaxEdges(int max) {
	maxEdgeTraversal = max;
}

CityRegionGenerator::CityRegionGenerator() {
	maxEdgeTraversal = 1024;
}


CityRegionGenerator::~CityRegionGenerator() {
}
