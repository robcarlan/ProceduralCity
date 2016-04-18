#include "BuildingLot.h"
std::list<Point>& BuildingLot::getPoints() {
	return points;
}

void BuildingLot::setOwner(BuildingRegion * owner) {
	this->owner = owner;
}

void BuildingLot::setStyle(buildingStyle style) {
	this->style = style;
}

void BuildingLot::setPopDensity(float popDensity) {
	this->popDensity = popDensity;
}

void BuildingLot::setHeight(float height) {
	this->height = height;
}

void BuildingLot::setSeed(int seed) {
	this->seed = seed;
}

void BuildingLot::setSkyscraper(bool isSkyscraper) {
	useSkyscraper = isSkyscraper;
}

void BuildingLot::createBuilding() {
	//Create L-System here? 
	building = new Building(this);

	buildingMade = true;
}

float BuildingLot::getPopDensity() {
	return popDensity;
}

float BuildingLot::getHeight() {
	return height;
}

int BuildingLot::getSeed() {
	return seed;
}

bool BuildingLot::getIsTriangle() {
	return isTriangle;
}

bool BuildingLot::getIsQuad() {
	return isQuad;
}

bool BuildingLot::getIsComplexPoly() {
	return isComplexPoly;
}

bool BuildingLot::getIsSkyscraper() {
	return useSkyscraper;
}

Point & BuildingLot::getCentroid() {
	return centroid;
}

buildingStyle BuildingLot::getStyle() {
	return style;
}

BuildingRegion * BuildingLot::getOwner() {
	return owner;
}

Building* BuildingLot::getBuilding() {
	return building;
}

BuildingLot::BuildingLot(std::list<Point> points) 
: isComplexPoly(0), isQuad(false), isTriangle(false) {
	this->points = points;
	this->centroid = BuildingRegion::getCentroid(points);
	buildingMade = false;

	isQuad = points.size() == 4;
	isTriangle = points.size() == 3;
	isComplexPoly = points.size() > 4;
	setSkyscraper(false);

	seed = 0;
}

BuildingLot::BuildingLot() 
	:points() {
}


BuildingLot::~BuildingLot() {

	if (buildingMade) delete building;
}

QRect BuildingLot::maximumRectangle(std::list<Point>& bounds) {
	if (bounds.size() == 4) {
		//Check for rectangle
		auto pItr = bounds.begin();
		auto p1 = *(pItr++);
		auto p2 = *(pItr++);
		auto p3 = *(pItr++);
		auto p4 = *(pItr++);

		auto d1 = p2 - p1;
		auto d2 = p3 - p2;
		auto d3 = p4 - p3;
		auto d4 = p1 - p4;


	} 

	return QRect(0, 0, 0, 0);
}
