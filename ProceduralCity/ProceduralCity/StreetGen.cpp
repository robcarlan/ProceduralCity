#include "StreetGen.h"

using namespace boost::geometry;

//int main(int argc, char* argv) {
//	int i;
//
//	StreetGen sg;
//	sg.Run();
//
//	std::cin >> i;
//	return 0;
//}


void StreetGen::setSeed(int seed) {
	rng.seed(seed);
}

void StreetGen::setSeed() {
	rng.seed(static_cast<unsigned int>(std::time(0)));
}

void StreetGen::Run() {
	
	//Just generate some random straight roads to render	
	generatedRoads->clear();

	int numRoads = 32;
	for (int itrNum = 0; itrNum < 32; itrNum++) {
		Point* p1 = new Point(genX(rng), genY(rng));
		Point* p2 = new Point(genX(rng), genY(rng));

		StraightRoad *newRoad = new StraightRoad(*p1, *p2);
		generatedRoads->push_back(*newRoad);
	}
	
}

std::vector<Road>* StreetGen::getGenerated() {
	return generatedRoads;
}

StreetGen::StreetGen() {
	generatedRoads = new std::vector<Road>();
	setSize(Point(2048.0f, 2048.0f));
}


StreetGen::~StreetGen()
{

}

void StreetGen::setHeightMap(QImage & hMap) {
	this->hMap = QImage(hMap);
}

void StreetGen::setPopMap(QImage & pMap) {
	this->pMap = QImage(pMap);
}

void StreetGen::setGeogMap(QImage & gMap) {
	this->gMap = QImage(gMap);
}

void StreetGen::setSize(Point newSize) {
	size = newSize;
	genX = boost::random::uniform_int_distribution<>(0, size.getX());
	genY = boost::random::uniform_int_distribution<>(0, size.getY());
}
