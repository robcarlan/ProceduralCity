#include "RoadGeometry.h"

void RoadGeometry::calcAngles() {
	angle = std::atan2f(end.x() - start.x(), end.y() - start.y());
	angle += boost::math::float_constants::two_pi;
	float pi = boost::math::float_constants::pi;
	angle = std::fmodf(angle, boost::math::float_constants::two_pi);
	angleAway = std::fmodf(angle + pi, boost::math::float_constants::two_pi);
}

roadType RoadGeometry::getRoadType() {
	return rtype;
}

float RoadGeometry::getAngleToEnd() {
	return angle;
}

float RoadGeometry::getAngleToStart() {
	return angleAway;
}

float RoadGeometry::getLength() {
	return length;
}

QLineF & RoadGeometry::getNorm() {
	return norm;
}

std::pair<intersectionPtr, bool> RoadGeometry::getOtherEnd(intersectionPtr toCheck) const {
	if (toCheck->getIntersectionPoint().getDistanceSq(start) < 0.1f)
		return std::make_pair(endIntersect, true);
	else if (toCheck->getIntersectionPoint().getDistanceSq(end) < 0.1f)
		return std::make_pair(startIntersect, false);
	assert(false);
	return std::make_pair(nullptr, false);
}

intersectionPtr RoadGeometry::getStart() {
	return startIntersect;
}

intersectionPtr RoadGeometry::getEnd() {
	return endIntersect;
}

void RoadGeometry::setStart(intersectionPtr startPtr) {
	startIntersect = startPtr;
	this->start = startPtr->getIntersectionPoint();
	calcAngles();

	if (startIntersect != nullptr && endIntersect != nullptr) valid = true;
}

void RoadGeometry::setEnd(intersectionPtr endPtr) {
	endIntersect = endPtr;
	this->end = endPtr->getIntersectionPoint();
	calcAngles();

	if (startIntersect != nullptr && endIntersect != nullptr) valid = true;
}

void RoadGeometry::disconnectRoad() {
	removeIntersection(startIntersect);
	removeIntersection(endIntersect);
}

void RoadGeometry::flagInvalid() {
	valid = false;
}

bool RoadGeometry::isValid() {
	return valid;
}

void RoadGeometry::calculateGeometry() {
	this->length = startIntersect->getIntersectionPoint().getDistance(endIntersect->getIntersectionPoint());
	norm = QLineF(start, end).unitVector();	
}

void RoadGeometry::removeIntersection(intersectionPtr toRemove) {
	if (startIntersect == toRemove) {
		toRemove->removeRoad(this);
		startIntersect = nullptr;
		start = Point(0, 0);
		flagInvalid();
	}
	else if (endIntersect == toRemove) {
		toRemove->removeRoad(this);
		endIntersect = nullptr;
		end = Point(0, 0);
		flagInvalid();
	}
	else {
		//Else, we couldn't find the intersection
		assert(false);
	}
}

RoadGeometry::RoadGeometry(const Road & toCopy) {
	rtype = toCopy.rType;
	start = toCopy.getStart();
	end = toCopy.getEnd();
	angle = toCopy.getAngle();
}

RoadGeometry::RoadGeometry() {
	this->startIntersect = nullptr;
	this->endIntersect = nullptr;
	this->endElevation = 0.0f;
	this->startElevation = 0.0f;
	this->start = Point(0, 0);
	this->end = Point(0, 0);
	this->valid = false;
}


RoadGeometry::~RoadGeometry() {
}
