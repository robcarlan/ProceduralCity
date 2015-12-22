#include "Road.h"



Road::Road(Point start, Point end){
	this->start = start;
	this->end = end;
}


Road::~Road() {
}

StraightRoad::StraightRoad(Point start, Point end) : Road(start, end) {
}
