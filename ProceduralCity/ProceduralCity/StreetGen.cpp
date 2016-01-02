#include "StreetGen.h"

using namespace boost::geometry;

void StreetGen::setSeed(int seed) {
	rng.seed(seed);
}

void StreetGen::setSeed() {
	rng.seed(static_cast<unsigned int>(std::time(0)));
}

void StreetGen::Run() {
	
	while (!finished) {
		nextIteration();
	}
}

void StreetGen::nextIteration() {
	if (finished) return;

	for (int itrNum = 0; itrNum < 4; itrNum++) {
		Point* p1 = new Point(genX(rng), genY(rng));
		Point* p2 = new Point(genX(rng), genY(rng));

		StraightRoad *newRoad = new StraightRoad(*p1, *p2);
		getScene()->addItem(newRoad);
	}

	finished = getScene()->items().count() > 32;
}

//
void StreetGen::initialise() {
	streets.clearRoads();

	ready = true;
	finished = false;
}

bool StreetGen::isReady() {
	return ready;
}

bool StreetGen::isFinished() {
	return finished;
}

std::vector<Road>* StreetGen::getGenerated() {
	return nullptr;
}

StreetGen::StreetGen() {
	ready = finished = false;
	setSize(Point(2048.0f, 2048.0f));
}


StreetGen::~StreetGen()
{

}

void StreetGen::setHeightMap(QImage & hMap, bool use) {
	this->hMap = QImage(hMap);
	streets.setHeight(&hMap);
	useHeight = use;
}

void StreetGen::setPopMap(QImage & pMap, bool use) {
	this->pMap = QImage(pMap);
	streets.setPop(&pMap);
	usePop = use;
}

void StreetGen::setGeogMap(QImage & gMap, bool use) {
	this->gMap = QImage(gMap);
	streets.setGeog(&gMap);
	useGeog = use;
}

void StreetGen::setSize(Point newSize) {
	size = newSize;
	genX = boost::random::uniform_int_distribution<>(0, size.getX());
	genY = boost::random::uniform_int_distribution<>(0, size.getY());
}

QGraphicsScene *StreetGen::getScene() {
	return streets.getScene();
}
