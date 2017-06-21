#include "BuildingRegion.h"

void BuildingRegion::calculateArea() {
	typedef boost::geometry::model::d2::point_xy<float> point;
	boost::geometry::model::polygon<point> poly;

	BOOST_FOREACH(Point p, bounds) {
		boost::geometry::append(poly, point::point(p.x(), p.y()));
	}
	area = abs(boost::geometry::area(poly));
}

std::list<Point>& BuildingRegion::getPoints() {
	return bounds;
}

std::vector<BuildingLot>& BuildingRegion::getLots() {
	return lots;
}

bool BuildingRegion::isValid() {
	return valid;
}

bool BuildingRegion::isConvex() {
	return convex;
}

void BuildingRegion::flagInvalid() {
	valid = false;
}

void BuildingRegion::setLots(std::vector<BuildingLot> lots) {
	this->lots = lots;

	BOOST_FOREACH(BuildingLot& lot, this->lots) {
		lot.setOwner(this);
	}
}

Point & BuildingRegion::getCentroid() {
	return centroid;
}

BuildingRegion::BuildingRegion(std::list<Point> bounds, std::list<float> angles) {
	valid = true;
	this->bounds = bounds;
	this->roadBearing = angles;
	
	//calculateArea();
	area = abs(getPolyArea(bounds));
	convex = isPolyConvex(bounds, getConcave(bounds));
	centroid = getCentroid(bounds);
}

BuildingRegion::BuildingRegion() {
}


BuildingRegion::~BuildingRegion() {
}

Point BuildingRegion::getCentroid(const std::list<Point>& bounds) {
	float x = 0.0f;
	float y = 0.0f;
	float s = 1.0f / (6 * getPolyArea(bounds));
	auto prevItr = bounds.back();
	auto rItr = bounds.begin();

	while (rItr != bounds.end()) {
		x += (prevItr.x() + rItr->x()) * (prevItr.x() * rItr->y() - rItr->x() * prevItr.y());
		y += (prevItr.y() + rItr->y()) * (prevItr.x() * rItr->y() - rItr->x() * prevItr.y());

		prevItr = *rItr;
		rItr++;
	}
	x *= s;
	y *= s;
	return Point(x, y);
}

float BuildingRegion::getPolyArea(const std::list<Point>& bounds) {
	double sum = 0.0;
	auto prevItr = bounds.back();
	auto rItr = bounds.begin();

	while (rItr != bounds.end() ) {
		sum += prevItr.x() * rItr->y() - rItr->x() * prevItr.y();
		prevItr = *rItr;
		rItr++;
	}
	return 0.5f * sum;
}

//Recursive approach
void BuildingRegion::splitConvex(const std::list<Point>& bounds, std::vector<std::list<Point>>& convex) {
	//Test to see if already concave
	auto splitStart = getConcave(bounds);
	if (isPolyConvex(bounds, splitStart)) {
		//Base case - convex, so add this
		convex.push_back(bounds);
		return;
	}
	
	//Now search for an edge to split by
	auto eItr = splitStart;
	std::list<Point> p1 = std::list<Point>();
	std::list<Point> p2 = std::list<Point>();
	float bestLength = 10000.0f * 10000.0f;
	float area = BuildingRegion::getPolyArea(bounds);

	bool complete = false;
	while (!complete) {
		eItr++;
		if (eItr == bounds.end()) 
			eItr = bounds.begin();
		if (eItr == splitStart) {
			complete = true;
			continue;
		}

		//See how good this split will be
		auto split = splitPolygon(bounds, splitStart, eItr);
		auto temp_p1 = split.first;
		auto temp_p2 = split.second;

		//Ensure new polys are >= 3 sides each
		if (temp_p1.size() <= 2) continue;
		if (temp_p2.size() <= 2) continue;
		
		float p1Area = BuildingRegion::getPolyArea(temp_p1);
		float p2Area = BuildingRegion::getPolyArea(temp_p2);
		float sumArea = p1Area + p2Area;
		float lSquared = splitStart->getDistanceSq(*eItr);

		if (lSquared < bestLength) {
			//Ensure area adds up, so that the split actually preserved the shape of the polygon
			//I.e cut doesnt go outside
			if (sumArea < area + 0.01f && sumArea > area - 0.01f) {
				p1 = temp_p1;
				p2 = temp_p2;
				bestLength = lSquared;
			}
		}

	}

	//Recursive on two new halves
	splitConvex(p1, convex);
	splitConvex(p2, convex);

	return;
}

std::list<Point>::const_iterator BuildingRegion::getConcave(const std::list<Point>& bounds) {
	//assert(bounds.size() > 2);
	if (bounds.size() <= 3) return bounds.end();

	//Iterate over edges

	//Start at [0,1,2], end at [n-1, 0, 1]
	auto vert = bounds.end();
	auto prev = bounds.begin();
	auto itr = ++bounds.begin();
	auto next = ++(++bounds.begin());
	bool finished = false;
	//Use sign to get winding
	bool antiClockwise = (getPolyArea(bounds) >= 0.0f);
	while(!finished) {
		bool notPrecise = true;
		float ax = next->x() - itr->x();
		float ay = next->y() - itr->y();
		float bx = itr->x() - prev->x();
		float by = itr->y() - prev->y();

		static float threshold = 5.0f;
		if (notPrecise) {
			if (abs(ax) < threshold) ax = 0.0f;
			if (abs(ay) < threshold) ay = 0.0f;
			if (abs(bx) < threshold) bx = 0.0f;
			if (abs(by) < threshold) by = 0.0f;
		}

		float co = bx * ay - by * ax;

		if (antiClockwise) {
			if (co < 0.0f) {
				vert = itr;
				break;
			}
		}
		else if (co > 0.0f) {
			vert = itr;
			break;
		}

		//Iterator updates
		prev = itr;
		itr = next;
		next++;

		if (next == bounds.end())
			next = bounds.begin();

		//=> [n-1, 0, 1]
		if (itr == bounds.begin())
			finished = true;
	}

	return vert;
}

bool BuildingRegion::isPolyConvex(const std::list<Point>& bounds, const std::list<Point>::const_iterator itr) {
	return (itr == bounds.end());
}

std::pair<std::list<Point>, std::list<Point>> BuildingRegion::splitPolygon(const std::list<Point>& bounds, std::list<Point>::const_iterator splitStart, std::list<Point>::const_iterator splitEnd) {
	//Splits a plygon into 2, splitting along the two iterators.
	auto itr = splitStart;
	std::list<Point> polya = std::list<Point>();
	std::list<Point> polyb = std::list<Point>();
	//Poly 1 = [SplitEnd, ..., n-1, 0, ... , splitStart]
	//Poly 2 = [SplitStart, SplitStart + 1, ..., SplitEnd]
		
	//Poly a
	while (itr != splitEnd) {
		if (itr == bounds.end())
			itr = bounds.begin();
		polya.push_back(*itr);

		itr++;

		if (itr == bounds.end())
			itr = bounds.begin();
	}

	polya.push_back(*itr);
	polyb.push_back(*itr);
	itr++; // Now points to splitEnd

	if (itr == bounds.end())
		itr = bounds.begin();

	//Poly b
	while (*itr != *splitStart) {
		if (itr == bounds.end())
			itr = bounds.begin();
		polyb.push_back(*itr);
		itr++;
		if (itr == bounds.end())
			itr = bounds.begin();
	}

	polyb.push_back(*itr);
	return std::make_pair(polya, polyb);
}
