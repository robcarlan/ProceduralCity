#include "BuildingRegion.h"

void BuildingRegion::calculateArea() {
	typedef boost::geometry::model::d2::point_xy<float> point;
	boost::geometry::model::polygon<point> poly;

	BOOST_FOREACH(Point p, bounds) {
		boost::geometry::append(poly, point::point(p.x(), p.y()));
	}
	area = abs(boost::geometry::area(poly));
}

std::list<Point> BuildingRegion::getPoints() {
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

void BuildingRegion::setLots(std::vector<BuildingLot>& lots) {
	this->lots = lots;
}

BuildingRegion::BuildingRegion(std::list<Point> bounds, std::list<float> angles) {
	valid = true;
	this->bounds = bounds;
	this->roadBearing = angles;
	
	//calculateArea();
	area = abs(getPolyArea(bounds));
	convex = getConcave(bounds) == bounds.end();

	//if (area < 1000.0f) flagInvalid();
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
std::vector<std::list<Point>> BuildingRegion::splitConvex(const std::list<Point>& bounds) {
	//Test to see if already concave

	return std::vector<std::list<Point>>();
}

std::list<Point>::const_iterator BuildingRegion::getConcave(const std::list<Point>& bounds) {
	if (bounds.size() == 3) return bounds.end();

	//Iterate over edges
	auto vert = bounds.end();
	auto prev = bounds.end()--;
	auto itr = bounds.begin();
	auto next = bounds.begin()++;
	bool finished = false;
	//Use sign to get winding
	bool antiClockwise = (getPolyArea(bounds) >= 0.0);
	while(!finished) {
		float ax = next->x() - itr->x();
		float ay = next->y() - itr->y();
		float bx = itr->x() - prev->x();
		float by = itr->y() - prev->y();
		float co = bx * ay - by * ax;

		if (antiClockwise && co < 0.0f) {
			vert = itr;
			break;
		}
		else if (co > 0.0) {
			vert = itr;
			break;
		}

		//Iterator updates
		prev = itr;
		itr = next;
		next++;
		if (next == bounds.end())
			next = bounds.begin();
		else if (next == bounds.begin())
			finished = true;
	}

	return vert;
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
	}

	polya.push_back(*itr);
	//Poly b
	while (itr != splitStart) {
		if (itr == bounds.end())
			itr = bounds.begin();
		polyb.push_back(*itr);
	}

	polya.push_back(*itr);
	return std::make_pair(polya, polyb);
}
