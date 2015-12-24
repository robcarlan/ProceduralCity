#include "Road.h"

Road::Road(Point start, Point end){
	this->start = start;
	this->end = end;
}


Road::~Road() {
}

StraightRoad::StraightRoad(Point start, Point end) : Road(start, end) {
}

std::string StraightRoad::printRoad() {
	return std::string("Road begins at " + this->getStart().printPoint() + " and ends at " + this->getEnd().printPoint());
}

Point Road::getStart() { return Road::start; }
Point Road::getEnd() { return Road::end; }



