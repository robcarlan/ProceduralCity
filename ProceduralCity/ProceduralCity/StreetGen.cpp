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

void StreetGen::Run() {
	
	//Just generate some random straight roads to render

	boost::random::mt19937 rng;
	rng.seed(static_cast<unsigned int>(std::time(0)));
	boost::random::uniform_int_distribution<> genX(topLeft.getX(), bottomRight.getX());
	boost::random::uniform_int_distribution<> genY(bottomRight.getY(), topLeft.getY());
	
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
	Run();
	return generatedRoads;
}

StreetGen::StreetGen()
{
	generatedRoads = new std::vector<Road>();
	topLeft = Point(0.0f, 600.0f);
	bottomRight = Point(1200.0f, 0.0f);
}


StreetGen::~StreetGen()
{

}
