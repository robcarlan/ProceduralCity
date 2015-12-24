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
	
	//Just generate some random straight roads to render.

	boost::random::mt19937 rng;
	rng.seed();
	boost::random::uniform_int_distribution<> gen(-128, 128);
	
	int numRoads = 32;

	for (int itrNum = 0; itrNum < 32; itrNum++) {
		Point p1 = Point(gen(rng), gen(rng));
		Point p2 = Point(gen(rng), gen(rng));

		StraightRoad newRoad = StraightRoad(p1, p2);
		generatedRoads.push_back(newRoad);

		std::cout << newRoad.printRoad() << std::endl;
	}
	
}

StreetGen::StreetGen()
{
	topLeft = Point(-128.0f, -128.0f);
	bottomRight = Point(128.0f, 128.0f);
}


StreetGen::~StreetGen()
{

}
