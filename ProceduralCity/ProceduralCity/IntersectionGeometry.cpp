#include "IntersectionGeometry.h"

std::pair<roadPtr, float> IntersectionGeometry::getBest(float angle, bool searchClockwise) {
	//Find angle with largest -ve change
	std::list<float>::iterator angleItr = angles.begin();
	std::list<roadPtr>::iterator roadItr = connected.begin();

	float best = boost::math::float_constants::two_pi;
	float bestAngle = 0.0f;
	roadPtr bestRoad;

	while (angleItr != angles.end()) {
		if (abs(angle - *angleItr) < 0.001f) {
			angleItr++;
			roadItr++;
			continue; //Iterating over ourself!
		}

		//bool searchClockwise = false;
		//bool isAngleAbove = isAbove(angle);
		//if (scanRight && isAngleAbove) searchClockwise = true;
		//else if (!scanRight && !isAngleAbove) searchClockwise = true;
		float dif = getAngleBetween(angle, *angleItr, searchClockwise);
		//float value = scanLeft ? *angleItr - angle : angle - *angleItr;
		//float dif = fmodf(value, boost::math::float_constants::two_pi);
		//Wrap to +ve value
		if (dif < 0.0f) dif += boost::math::float_constants::two_pi;
		if (dif < best) {
			best = dif;
			bestAngle = *angleItr;
			bestRoad = *roadItr;
		}

		angleItr++;
		roadItr++;
	}

	return std::make_pair(bestRoad, bestAngle);
}

float IntersectionGeometry::getAngleBetween(float angleStart, float angleEnd, bool clockwise) {
	float value = clockwise ? angleStart - angleEnd : angleEnd - angleStart;
	if (value < 0.0f) value += boost::math::float_constants::two_pi;
	return fmodf(value, boost::math::float_constants::two_pi);
}

int IntersectionGeometry::getNumIntersections() {
	return numIntersections;
}

Point IntersectionGeometry::getIntersectionPoint() {
	return intersectionPoint;
}

void IntersectionGeometry::connectRoad(roadPtr toAdd) {
	//Add road, then add angle
	connected.push_back(toAdd);
	numIntersections++;
	bool connectedToStart = toAdd->getStart()->intersectionPoint.getDistanceSq(intersectionPoint) < 0.1f;
	angles.push_back(connectedToStart ? toAdd->getAngleToEnd() : toAdd->getAngleToStart());

	if (numIntersections >= 2) valid = true;
}

void IntersectionGeometry::removeRoad(RoadGeometry *toRemove) {
	auto roadPtr = connected.begin();
	auto angleItr = angles.begin();

	while (roadPtr != connected.end()) {
		if (toRemove == &(**roadPtr)) {
			connected.erase(roadPtr);
			angles.erase(angleItr);
			numIntersections--;

			if (numIntersections < 2) valid = false;

			return;
		}

		roadPtr++;
		angleItr++;
	}

	//Shouldn't get here - we couldn't find the road to remove
	assert(false);
}

std::pair<roadPtr, bool> IntersectionGeometry::getAntiClockwise(float angle, bool clockwise) {
	//Travelling on the right and looking for left most, is the same as looking for the right most(i.e. first)
	if (clockwise == true) return getClockwise(angle, clockwise);
	auto next = getBest(angle, clockwise);
	float outgoingAngle = next.second;
	//Switch sides if travel vertically
	return std::make_pair(next.first, getNextSide(angle, outgoingAngle, clockwise));
}

std::pair<roadPtr, bool> IntersectionGeometry::getClockwise(float angle, bool clockwise) {
	if (clockwise == false) return getAntiClockwise(angle, clockwise);
	//Find angle with smalles +ve change
	auto next = getBest(angle, clockwise);
	float outgoingAngle = next.second;
	//Switch sides if travel vertically
	return std::make_pair(next.first, getNextSide(angle, outgoingAngle, clockwise));
}

bool IntersectionGeometry::getNextSide(float startAngle, float endAngle, bool isClockwise) {
	//Sides switch whenever we cross a horizontal boundary
	// 0 angle is bottom, 1/2 pi is rhs ... 
	//it is opposite : getNextSide(sideBefore) = !getNextSide(!sideBefore)

	return !isClockwise;

	//assert(startAngle >= 0 && startAngle < boost::math::float_constants::two_pi);
	//assert(endAngle >= 0 && endAngle < boost::math::float_constants::two_pi);
	//bool inBottomHalfStart = isBelow(startAngle);
	//bool inBottomHalfEnd = isBelow(endAngle);

	////If we are on the same half (horizontal plane) we take the opposite. Otherwise, stay the same
	//if ((inBottomHalfEnd && inBottomHalfStart) || (!inBottomHalfEnd && !inBottomHalfStart)) return (!sideBefore);
	//else return sideBefore;
}

bool IntersectionGeometry::isAbove(float angle) {
	return (angle <= piandahalf && angle >= halfpi);
}

bool IntersectionGeometry::isBelow(float angle) {
	return !isAbove(angle);
}

std::list<roadPtr> IntersectionGeometry::getIntersections() {
	return connected;
}

void IntersectionGeometry::flagInvalid() {
	valid = false;
}

bool IntersectionGeometry::isValid() {
	return valid;
}

IntersectionGeometry::IntersectionGeometry(const RoadIntersection &toCopy) {
	intersectionPoint = toCopy.location;
	this->numIntersections = 0;
	this->connected = std::list<roadPtr>();
	valid = false;
	pi = boost::math::float_constants::pi;
	twopi = boost::math::float_constants::two_pi;
	halfpi = boost::math::float_constants::half_pi;
	piandahalf = pi + halfpi;
}

IntersectionGeometry::IntersectionGeometry() {
	this->intersectionPoint = Point(0, 0);
	this->numIntersections = 0;
	this->connected = std::list<roadPtr>();
	valid = false;
	pi = boost::math::float_constants::pi;
	twopi = boost::math::float_constants::two_pi;
	halfpi = boost::math::float_constants::half_pi;
	piandahalf = pi + halfpi;
}


IntersectionGeometry::~IntersectionGeometry() {
}
