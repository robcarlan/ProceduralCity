#include "CityRegionGenerator.h"

//This function creates all of the city regions by walking through each road, and marking each side of each road as visited or not. The algorithm finishes once all sides of all roads are visited.
std::vector<BuildingRegion> CityRegionGenerator::createRegions(std::list<roadPtr> const roads, std::list<intersectionPtr> const intersections) {
	//Loop left and right over every intersection. (Over every road. Follow a left / right path). Loop identified by (intersection, index).
	//Keep looping until max_edges, if we loop mark that as a loop and continue.
	QElapsedTimer timer;
	timer.start();

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
	int numRegions = 0;
	int numCulled = 0;

	while (true) {
		BOOST_FOREACH(const roadPtr roadStart, roads) {
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

					if (false) {
					//if (hasVisited(clockwise, followingRoadForwards, currentRoad)) {
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
						} else {
							//Keep following, find the next road at the intersection
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
					numRegions++;

					//Turn into a region
					auto bounds = toRegion(traversing, direction, side, angles);
					BuildingRegion tmp = BuildingRegion(bounds, angles);

					if (coversIllegalTerritory(bounds)) {
						tmp.flagInvalid();
						numCulled++;
					} else if (abs(BuildingRegion::getPolyArea(bounds)) < minBuildArea) {
						tmp.flagInvalid();
						numCulled++;
					}

					//TODO :: Improve from this method
					QPolygon enclosed;
					QPolygon larger;
					auto rItr = traversing.begin();
					auto dItr = direction.begin();

					while (rItr != traversing.end()) {
						Point pt = (*dItr) ? (*rItr)->getStart()->getIntersectionPoint() : (*rItr)->getEnd()->getIntersectionPoint();
						larger.append(QPoint(pt.x(), pt.y()));

						rItr++;
						dItr++;
					}

					BOOST_FOREACH(Point& pt, bounds) {
						QPoint qpt = QPoint(pt.x(), pt.y());
						enclosed.append(qpt);
						if (!larger.containsPoint(qpt, Qt::WindingFill)) {
							//Todo :: False negatives?
							tmp.flagInvalid();
							numCulled++;
							break;
						}
					}

					out.push_back(tmp);
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

	qDebug() << "Created : " << numRegions << " regions";
	qDebug() << "Culled : " << numCulled << " regions";
	qDebug() << "Elapsed : " << timer.elapsed() / 1000.0f << " seconds";

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

	return out;
}

bool CityRegionGenerator::hasVisited(bool side, bool forwards, roadPtr traversing) {
	//Clockwise + Forward or AntiCW + Backwards = search clockwise
	if ((side && forwards) || (!side && !forwards))
		return !(clockwiseVisited.find(traversing) == clockwiseVisited.end());
	else
		return !(anticlockwiseVisited.find(traversing)
				 == anticlockwiseVisited.end());
}

bool CityRegionGenerator::isValidRegion(bool side, bool forwards, roadPtr traversing) {
	assert(hasVisited(side, forwards, traversing));
	if ((side && forwards) || (!side && !forwards)) return clockwiseVisited[traversing];
	else return anticlockwiseVisited[traversing];
}

bool CityRegionGenerator::coversIllegalTerritory(std::list<Point>& bounds) {
	//Simple test - see if centroid covers illegal
	auto p_centroid = BuildingRegion::getCentroid(bounds);
	if (getHeightValue(getHeight(p_centroid)) < 0.0f) return true;
	return false;

	//Else sample every point in the bounding rectangle - expensive

	QRect toCheck = smallestEnclosingRectangle(bounds);

	for (int x = toCheck.left(); x < toCheck.right(); x++) {
		for (int y = toCheck.top(); y < toCheck.bottom(); y++) {
			if (sampleGeog(x, y, *geogSampler) == geogType::PARK ||
				sampleGeog(x, y, *geogSampler) == geogType::WATER)
				return true;
		}
	}

	return false;
}

void CityRegionGenerator::flagRoads(std::list<roadPtr> traversing,
									std::list<bool> side,
									bool flagFound) {
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
Point CityRegionGenerator::getRegionPoint(roadPtr r1, roadPtr r2,
										  intersectionPtr intersection,
										  bool cw1, bool cw2, float angle1,
										  float angle2) {
	bool r1Away = r1->getStart()->getIntersectionPoint()
		== intersection->getIntersectionPoint();
	bool r2Away = r2->getStart()->getIntersectionPoint()
		== intersection->getIntersectionPoint();
	float r1x = cosf(angle1);
	float r1y = sinf(angle1);
	float r2x = cosf(angle2);
	float r2y = sinf(angle2);
	float r1Width = getRoadWidth(r1->getRoadType());
	float r2Width = getRoadWidth(r2->getRoadType());
	r1x *= r1Width;
	r1y *= r1Width;
	r2x *= r2Width;
	r2y *= r2Width;

	assert(cw1 && !cw2 || !cw1 && cw2);

	//r1x = r1y = r2x = r2y = 0.0f;

	QLineF roadSide1 = QLineF(r1->getStart()->getIntersectionPoint().x(),
							  r1->getStart()->getIntersectionPoint().y(),
							  r1->getEnd()->getIntersectionPoint().x(),
							  r1->getEnd()->getIntersectionPoint().y());
	if (cw1)
		roadSide1.translate(r1x, -r1y);
	else roadSide1.translate(-r1x, r1y);

	QLineF roadSide2 = QLineF(r2->getStart()->getIntersectionPoint().x(),
							  r2->getStart()->getIntersectionPoint().y(),
							  r2->getEnd()->getIntersectionPoint().x(),
							  r2->getEnd()->getIntersectionPoint().y());

	if (!cw2)
		roadSide2.translate(r2x, -r2y);
	else roadSide2.translate(-r2x, r2y);

	//Check for nearby angle
	float dif = abs(angle1 - angle2);
	float tpi = boost::math::float_constants::two_pi;
	bool isParallel = (
		(abs(dif - tpi) < 0.01f) ||
		(abs(tpi - dif) < 0.01f) ||
		dif < 0.01f);

	if (isParallel) {
		//Too close
		//return intersection->getIntersectionPoint();
		return Point(intersection->getIntersectionPoint().x()
					 + (cw1 ? r1x : -r1x),
					 intersection->getIntersectionPoint().y()
					 + (cw1 ? -r1y : r1y));
	}

	QPointF intersectPoint;
	auto intersectResult = roadSide1.intersect(roadSide2, &intersectPoint);

	if (intersectResult == QLineF::IntersectType::NoIntersection) {
		//Shouldnt occur
		//return intersection->getIntersectionPoint();
	}
	return intersectPoint;
}

std::list<Point> CityRegionGenerator::toRegion(std::list<roadPtr>& roadList,
											   std::list<bool>& travelDirection,
											   std::list<bool>& side,
											   std::list<float>& angles) {
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

		bounds.push_back(
			getRegionPoint(prevRoad,
						   *rItr,
						   intersection,
						   forwardPrev ? !clockwisePrev : clockwisePrev,
						   forward ? clockwiseRoad : !clockwiseRoad,
						   anglePrev,
						   angleNow)
			);

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
buildingStyle CityRegionGenerator::getBuildingStyle(Point & pos) {
	QColor val = QColor(buildingTypeSampler->pixel(pos.x(), pos.y()));

	if (val == RESIDENTIAL_COL)
		return RESIDENTIAL;
	else if (val == COMMERCIAL_COL)
		return COMMERCIAL;
	else if (val == INDUSTRIAL_COL)
		return INDUSTRIAL;

	else return NONE;
}

float CityRegionGenerator::getPopDensity(Point & pos) {
	return (densitySampler->pixel(pos.x(), pos.y()) % 256) / 256.0f;
}

float CityRegionGenerator::getHeight(Point & pos) {
	return (heightSampler->pixel(pos.x(), pos.y()) % 256) / 256.0f;
}

float CityRegionGenerator::getHeightValue(float factor) {
	return (1 - factor) * heightScale + minHeight;
}

float CityRegionGenerator::getRoadWidth(const roadType &road) {
	switch (road) {
	case roadType::MAINROAD:
		return mainRoadWidth;
	case roadType::STREET:
		return streetWidth;
	default:
		return defaultRoadWidth;
	}
}
void CityRegionGenerator::subdivideRegions(std::vector<BuildingRegion>& buildings) {
	srand(0);

	BOOST_FOREACH(BuildingRegion &regionItr, buildings) {
		//Invalid regions aren't split into lots - i.e. those with small area.
		if (!regionItr.isValid()) continue;

		//Break into convex regions, if not already convex
		std::unique_ptr<std::vector<BuildingLot>> resultLots =
			std::unique_ptr<std::vector<BuildingLot>>
			(new std::vector<BuildingLot>());

		if (!regionItr.isConvex()) {
			std::vector<std::list<Point>> convexLots =
				std::vector<std::list<Point>>();
			BuildingRegion::splitConvex(regionItr.getPoints(), convexLots);

			BOOST_FOREACH(std::list<Point> convexPoly, convexLots) {
				std::list<Point> toTest = convexPoly;

				//Add more points if necessary
				if (shouldSplitBoundsFurther(convexPoly))
					toTest = subdivideBounds(convexPoly);

				std::list<bool> roadAccess = std::list<bool>();
				for (int i = 0; i < toTest.size(); i++)
					roadAccess.push_back(true);

				createLotsFromConvexPoly(regionItr, toTest, roadAccess, resultLots);
			}
		}
		else {

			//Already convex
			std::list<Point> toTest = regionItr.getPoints();

			//Add more points if necessary
			if (shouldSplitBoundsFurther(regionItr.getPoints()))
				toTest = subdivideBounds(regionItr.getPoints());

			std::list<bool> roadAccess = std::list<bool>();
			for (int i = 0; i < toTest.size(); i++)
				roadAccess.push_back(true);

			createLotsFromConvexPoly(regionItr, toTest, roadAccess, resultLots);
		}
		//Add to region
		regionItr.setLots(*resultLots);
	}
}

//Recursive approach - split along edge w

//Todo :: track road access, prioritise those roads in the split.
//Can cull lots with no road access here.
void CityRegionGenerator::createLotsFromConvexPoly(BuildingRegion& owner,
												   std::list<Point>& bounds,
												   std::list<bool>& roadAccess,
												   std::unique_ptr<std::vector<BuildingLot>>& out) {
	auto result = std::vector<BuildingLot>();

	float area = abs(BuildingRegion::getPolyArea(bounds));

	bool hasRoadAccess = false;

	for (auto it = roadAccess.begin(); it != roadAccess.end(); it++) {
		if (*it) hasRoadAccess = true;
	}

	if (!hasRoadAccess) return;

	if (area < minBuildArea)
		return;
	if (area < maxBuildArea) {
		//Still has road access, so this lot is next to a road! Great!
		out->push_back(createLot(bounds, owner));
		return;
	}

	//O.w. need to break down further. Find two edges to subdivide
	auto splitEdges = getLongestEdgePair(bounds, roadAccess); //return longest w/ road access, longest w/o.
	auto longestp1 = splitEdges.first.first;
	auto longestp2 = splitEdges.first.second;
	auto secondlongestp1 = splitEdges.second.first;
	auto secondlongestp2 = splitEdges.second.second;
	float lengthLongest = longestp1.getDistance(longestp2);
	float lengthSecondLongest = secondlongestp1.getDistance(secondlongestp2);

	//Calculate midpoint, with some offset
	float uniform = (static_cast<float> (rand()) / static_cast<float>(RAND_MAX));
	float offset = 0.5f + (uniform) * randOffset - (randOffset/ 2.0f);

	Point midLongest;
	midLongest.setX(longestp1.x() + offset * (longestp2.x() - longestp1.x()));
	midLongest.setY(longestp1.y() + offset * (longestp2.y() - longestp1.y()));

	Point midSecondLongest;
	midSecondLongest.setX(
		secondlongestp1.x() +
		(1 - offset) * (secondlongestp2.x() - secondlongestp1.x()));
	midSecondLongest.setY(
		secondlongestp1.y() +
		(1 - offset) * (secondlongestp2.y() - secondlongestp1.y()));

	//Construct p1, p2
	auto polygon1 = std::list<Point>();
	auto roadAccess1 = std::list<bool>();

	polygon1.push_back(midLongest);
	auto rItr = roadAccess.begin();

	auto pItr = bounds.begin();
	while (*pItr != longestp2) {
		pItr++;
		rItr++;
	}

	//pItr == longestp2
	bool before;

	//Keep adding to new polygon
	while (*pItr != secondlongestp2) {
		polygon1.push_back(*pItr);
		roadAccess1.push_back(*rItr);

		before = *rItr;
		pItr++;
		rItr++;
		if (rItr == roadAccess.end())
			rItr = roadAccess.begin();
		if (pItr == bounds.end())
			pItr = bounds.begin();
	}

	//pItr == secondLongestp2
	polygon1.push_back(midSecondLongest);

	auto polygon2 = std::list<Point>();
	auto roadAccess2 = std::list<bool>();
	polygon2.push_back(midSecondLongest);

	if (pItr == bounds.end())
		pItr = bounds.begin();
	if (rItr == roadAccess.end())
		rItr = roadAccess.begin();

	//roadAccess2.push_back(*rItr);
	//roadAccess1.push_back(*rItr);
	roadAccess1.push_back(false);
	roadAccess2.push_back(before);

	//pItr starts at secondLongestp2
	while (*pItr != longestp2) {
		polygon2.push_back(*pItr);
		roadAccess2.push_back(*rItr);

		before = *rItr;
		pItr++;
		rItr++;

		if (pItr == bounds.end())
			pItr = bounds.begin();
		if (rItr == roadAccess.end())
			rItr = roadAccess.begin();
	}

	if (rItr == roadAccess.end())
		rItr = roadAccess.begin();
	//roadAccess2.push_back(*rItr);
	roadAccess1.push_front(before);
	polygon2.push_back(midLongest);
	roadAccess2.push_back(false);

	assert(roadAccess2.size() == polygon2.size());
	assert(roadAccess1.size() == polygon1.size());

	//Recursively construct lots inside these polygons
	createLotsFromConvexPoly(owner, polygon1, roadAccess1, out);
	createLotsFromConvexPoly(owner, polygon2, roadAccess2, out);
}

std::list<Point> CityRegionGenerator::subdivideBounds(std::list<Point>& bounds) {
	std::list<Point> out;

	int numIterations = bounds.size();
	int i = 0;

	auto pItr = bounds.begin();
	pItr++;
	auto before = bounds.begin();

	while (i < numIterations) {
		out.push_back(*before);

		if (pItr->getDistanceSq(*before) > 40 * 40) {
			//Create more points along this line
			QLineF toSubdivide = QLineF(*before, *pItr);
			float length = toSubdivide.length();
			toSubdivide = toSubdivide.unitVector();

			int numDivisions = length / 10.0f; // Every 10
			numDivisions = 2;

			for (int i = 1; i < numDivisions; i++ ) {
				toSubdivide.setLength(i * 10.0f);
				out.push_back(toSubdivide.p2());
			}
		}

		i++;
		before = pItr;
		pItr++;
		if (pItr == bounds.end()) pItr = bounds.begin();
	}

	return out;
}

bool CityRegionGenerator::shouldSplitBoundsFurther(std::list<Point>& bounds) {
	return false;
}

std::pair<std::pair<Point, Point>, std::pair<Point, Point>> CityRegionGenerator::getLongestEdgePair(const std::list<Point>& bounds,
	const std::list<bool>& hasRoadAccess) {
	//Iterate through, keeping track of two max roads
	Point p1 = bounds.front();
	Point p1Prev = bounds.back();
	Point p2 = bounds.front();
	Point p2Prev = bounds.back();
	float d1 = 0.0f;
	float d2 = 0.0f;

	auto pPrev = bounds.back();
	auto pItr = bounds.begin();
	auto rItr = hasRoadAccess.begin();
	while (pItr != bounds.end()) {
		float dist = pItr->getDistance(pPrev);

		if (dist > d1 && *rItr) {
			//Update best distances found
			p1 = *pItr;
			p1Prev = pPrev;
			p2 = p1;
			p2Prev = p1Prev;

			d2 = d1;
			d1 = dist;
		}
		//Update
		pPrev = *pItr;
		pItr++;
		rItr++;
	}

	//Direction vector of first line
	float p1X = p1.x() - p1Prev.x();
	float p1Y = p1.y() - p1Prev.y();

	//Now look for roughly parallel
	pItr = bounds.begin();
	pPrev = bounds.back();
	p2 = *pItr;
	p2Prev = pPrev;

	float product = 0.0f;
	bool found = false;
	bool before = true;
	bool foundBefore = false;

	//Find roughly parallel edge
	while (pItr != bounds.end()) {
		if (*pItr == p1) {
			//Iterating over self
			pPrev = *pItr;
			pItr++;
			before = false;
		}
		else {
			//Calculate dot
			float difX = pItr->x() - pPrev.x();
			float difY = pItr->y() - pPrev.y();
			float len = pItr->getDistance(pPrev);
			float dot = ((p1X * difX + p1Y * difY) / (len * d1));

			if (dot < product) {
				//New best found
				product = dot;
				p2 = *pItr;
				p2Prev = pPrev;
				found = true;
				foundBefore = before;
			}

			pPrev = *pItr;
			pItr++;
		}
	}

	if (!found) {
		//Try something else

		//Resorts to second longest line
	}

	if (foundBefore)
		return std::make_pair(
			std::make_pair(p2Prev, p2), std::make_pair(p1Prev, p1));
	else
		return std::make_pair(
			std::make_pair(p1Prev, p1), std::make_pair(p2Prev, p2));
}

BuildingLot CityRegionGenerator::createLot(std::list<Point> bounds,
										   BuildingRegion& owner) {

	//Initialises data from image maps
	assert(bounds.size() > 0);
	BuildingLot newBuilding(bounds);
	newBuilding.setOwner(&owner);
	newBuilding.setStyle(getBuildingStyle(newBuilding.getCentroid()));
	newBuilding.setPopDensity(getPopDensity(newBuilding.getCentroid()));
	newBuilding.setHeight(getHeightValue(getHeight(newBuilding.getCentroid())));

	return newBuilding;
}

void CityRegionGenerator::setMaxEdges(int max) {
	maxEdgeTraversal = max;
}

void CityRegionGenerator::setImageData(QImage & density,
									   QImage & buildingType,
									   QImage &height,
									   QImage &geog) {
	densitySampler = &density;
	buildingTypeSampler = &buildingType;
	heightSampler = &height;
	geogSampler = &height;
}

void CityRegionGenerator::setParams(float minBuildArea, float maxBuildArea,
									float randomOffset, float minLotDim,
									float maxLotDim, float mainRoadWidth,
									float streetWidth, bool allowLotMerge,
									float minHeight, float heightScale,
									int seed) {
	this->mainRoadWidth = mainRoadWidth;
	this->streetWidth = streetWidth;
	this->minBuildArea = minBuildArea;
	this->maxBuildArea = maxBuildArea;
	this->minLotDim = minLotDim;
	this->maxLotDim = maxLotDim;
	this->randOffset = randomOffset;
	this->allowLotMerge = allowLotMerge;
	this->minHeight = minHeight;
	this->heightScale = heightScale;
	this->seed = seed;
	srand(seed);
}

CityRegionGenerator::CityRegionGenerator() {
	maxEdgeTraversal = 1024;

	mainRoadWidth = 10.0f;
	streetWidth = 5.0f;
	minBuildArea = 100.0f;
	maxBuildArea = 1000.0f;
	minLotDim = 0.0f;
	maxLotDim = 200.0f;
	randOffset = 0.0f;
	allowLotMerge = false;
	seed = 0;
	srand(seed);
}


CityRegionGenerator::~CityRegionGenerator() {
}
