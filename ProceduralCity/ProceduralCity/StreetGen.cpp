#include "StreetGen.h"

using namespace boost::geometry;

void StreetGen::setSeed(int seed) {
	rng.seed(seed);
}

void StreetGen::setSeed() {
	rng.seed(static_cast<unsigned int>(std::time(0)));
}

void StreetGen::applyLocalConstraints(Variable *toCheck) {
	const float extendRadius = 10.0f;
	const float minDistanceSq = 10.0f;

	std::vector<StreetManager::intersectionRec> intersections;
	Road temp = Road(toCheck->road.start, toCheck->road.end);
	streets.getIntersectingEdges(temp, intersections);

	bool culled = false;

	if (intersections.size() > 0) {
		//We have found some intersections, get the closest.
		auto minIntersect = streets.getClosest(toCheck->road.start, intersections);
		toCheck->road.end = minIntersect->first;
		//toCheck->state = solutionState::FAILED;
		//Add a crossing, leave target as null as we need to create this
		toCheck->road.targetRoad = minIntersect->second;
		toCheck->state = solutionState::SUCCEED;
		culled = true;
	} else { //Road does not intersect, so lets see if it is near to an existing crossing.

		//TEMPORARY
		toCheck->state = solutionState::SUCCEED;
		return;

		std::vector<RoadIntersection *> existingIntersections;
		streets.getNearbyVertices(toCheck->road.end, extendRadius, existingIntersections);
		bool connectToIntersection = false;

		if (existingIntersections.size() > 0) {
			//we may be able to extend the road to join an intersection

			//sort by closest, then attach to the first intersection which doesn't intersect other lines
			BOOST_FOREACH(RoadIntersection *it, existingIntersections) {
				streets.getIntersectingEdges(Road(toCheck->road.start, it->location), intersections);
				if (intersections.size() == 0 && toCheck->road.start.getDistanceSq(it->location) > minDistanceSq) {
					toCheck->road.end = it->location;
					connectToIntersection = true; culled = true;
					break;
				}
			}
		}

		//Extend the road by a maximum radius, then see if it intersects.
		if (!connectToIntersection) {
			QLineF norm = temp.normalVector();
			norm.setLength(extendRadius);
			Point extended = Point(toCheck->road.start.x() + norm.dx(), toCheck->road.start.y() + norm.dy());
			streets.getIntersectingEdges(Road(toCheck->road.start, extended), intersections);

			if (intersections.size() > 0 && toCheck->road.start.getDistanceSq(extended) > minDistanceSq) {
				toCheck->road.end = extended;
				connectToIntersection = true;
				culled = true;
			}
		}
	}

	if (culled) toCheck->state = solutionState::SUCCEED;
	else toCheck->state = solutionState::SUCCEED;
}

StreetGen::VarList* StreetGen::applyGlobalConstraints(ruleAttr rules, roadAttr roads) {
	//Create 3 sets of new variables
	int BRANCH1 = 0;
	int BRANCH2 = 1;
	int ROAD = 2;

	//Initialise the new set of variables
	ruleAttr nrules[3];
	roadAttr nroads[3];
	int delay[3];
	boost::random::uniform_real_distribution<float> uniform = boost::random::uniform_real_distribution<float>();

	QPointF distance = roads.end - roads.start;

	for (int i = 0; i < 3; i++) {
		nrules[i] = rules;
		//Increment depth
		nrules[i].depth++;
		nroads[i] = roads;
		float scale = uniform(rng);
		nroads[i].start += distance * scale;
		delay[i] = i;
		//nroads[i].branchFromParent(roads.);
	}

	//Calculate the new values

	//Set branches to point at right angle
	nroads[BRANCH1].angle += math::half_pi<float>();// +uniform(rng);
	nroads[BRANCH2].angle -= math::half_pi<float>(); // +uniform(rng);
	nroads[ROAD].angle -= math::half_pi<float>();// +uniform(rng);

	//Set all roads to have some new length
	float newLength = 30.0f + uniform(rng) * 50.0f;
	nroads[BRANCH1].length += newLength;
	newLength = 30.0f + uniform(rng) * 50.0f;
	nroads[BRANCH2].length += newLength;
	newLength = 30.0f + uniform(rng) * 50.0f;
	nroads[ROAD].length += newLength;

	//Calculate new end positions
	for (int i = 0; i < 3; i++) {
		//Create unit vector in direction
		nroads[i].angle = math::mod<float>(nroads[i].angle, math::two_pi<float>());
		Point unit = Point(cosf(nroads[i].angle), sinf(nroads[i].angle));
		unit *= nroads[i].length;
		nroads[i].end = nroads[i].start + unit;
	}

	//Create the new variables, and return them.

	Variable *branch1 = new Variable(variableType::BRANCH, delay[BRANCH1], nrules[BRANCH1], nroads[BRANCH1]);
	Variable *branch2 = new Variable(variableType::BRANCH, delay[BRANCH2], nrules[BRANCH2], nroads[BRANCH2]);
	Variable *road = new Variable(variableType::ROAD, delay[ROAD], nrules[ROAD], nroads[ROAD]);
	Variable *insertion = new Variable(variableType::INSERTION, delay[ROAD], nrules[ROAD], nroads[ROAD]);
	insertion->state = solutionState::UNASSIGNED;

	VarList* newList = new VarList();
	newList->push_back(*branch1);
	newList->push_back(*branch2);
	newList->push_back(*road);
	newList->push_back(*insertion);

	return newList;
}

void StreetGen::addRoadToSystem(roadAttr roads) {
	//Create first intersection if necessary
	std::vector<RoadIntersection*> nearby;

	//If nearby just attach to existing one
	//TODO ie when starting at an existing intersection
	RoadIntersection *start = new RoadIntersection(roads.start, roads.parentRoad);
	//streets.getNearbyVertices(*start, 5.0f, nearby); 
	Road *road = new Road(roads.start, roads.end);

	//Create second intersection if necessary
	if (roads.targetRoad != nullptr) {
		//Check to see if we need to connect to an intersection / use old

		streets.connectToNewIntersection(start, road, roads.targetRoad);
	}
	else {
		//Create the new empty crossing
		streets.branchRoad(start, road);
	}
}

//Splice new variables into position, then remove all empty variables
void StreetGen::afterIteration() {
	//Only replace if toInsert isn't empty
	if (toInsert.empty()) return;

	VarIterator it = current.begin();
	std::list<std::pair<VarIterator, VarList>>::iterator replaceItr = toInsert.begin();

	while (it != current.end()) {
		//Delete all e variables
		//TODO :: hopefully redundant
		if (it->varType == variableType::EMPTY)
			it = current.erase(it);

		//Check if there are any productions to replace
		if (replaceItr == toInsert.end()) break;

		//Insert all new productions
		if (it == replaceItr->first) {
			//Empty production -> delete current variable
			if (replaceItr->second.size() == 0) {
				it = current.erase(it);
				replaceItr++;
				if (it == current.end()) break;
				else continue;
			} else {
				//Replace current variable with new productions
				current.splice(it, replaceItr->second);
				it = current.erase(it);
				it--;
			}

			replaceItr++;
		}

		it++;
	}

	//We should have reached the end of all productions
	assert(replaceItr == toInsert.end());

	toInsert.clear();
}

void StreetGen::insertProduction(VarIterator before, VarList after) {
	//toInsert.emplace_front(std::pair<VarIterator, VarList>(before, after));
}

StreetGen::VarList StreetGen::getInitialProduction() {
	VarList initial;
	ruleAttr emptyRule;
	roadAttr emptyRoad;
	initial.push_front(Variable(variableType::INSERTION, 0, emptyRule, getInitialRoadAttr()));
	initial.push_front(Variable(variableType::ROAD, 0, getInitialRuleAttr(), emptyRoad));

	return initial;
}

ruleAttr StreetGen::getInitialRuleAttr() {
	//eventually pass these as parameters
	ruleAttr rules;
	rules.depth = 0;

	return rules;
}

roadAttr StreetGen::getInitialRoadAttr() {
	//eventually pass these as paramters
	roadAttr road;
	road.angle = 0.0f;
	road.start = Point(500, 500);
	road.end = Point(600, 700);
	road.length = math::sqrt(road.start.getDistanceSq(road.end));

	return road;
}

//Applies the L System rules to the current variable. Returns the productions in VarList productions. Specific road creation behaviour
//is obtained by calls to LocalConstraints and GlobalConstraints
bool StreetGen::applyRule(VarIterator currentVar, VarList *productions) {
	//VarList productions;
	assert(productions->empty());
	VarIterator left;
	VarIterator right;

	switch (currentVar->varType) {
	case variableType::BRANCH : 
		//Production 6
		if (currentVar->delay < 0) return true;
		else if (currentVar->delay > 0) {
			//Production 4, reduce delay
			currentVar->delay--;
			return false;
		}
		else {
			//Production 5. Create new Road and Insertion module
			productions->push_front(Variable(variableType::INSERTION, 0, currentVar->rules, currentVar->road));
			productions->push_front(Variable(variableType::ROAD, 0, currentVar->rules, currentVar->road));
			return true;
		}
		break;
	case variableType::INSERTION:
		//Production 7 - If a road with delay < 0 preceeds this, delete the variable.
		left = currentVar;
		left--;
		if (left->varType == variableType::ROAD && left->delay < 0) {
			return true;
		}
		else if (currentVar->state == solutionState::UNASSIGNED) {
			//Production 8. Unassigned insertion. Call local constraints to attempt to adjust the parameters.
			applyLocalConstraints(&(*currentVar));

			//Succeed => Safe to add to system
			if (currentVar->state == solutionState::SUCCEED)
				addRoadToSystem(currentVar->road);

		}
		else {
			//Production 9. If successful, the Road variable at production 2 will obtain the data needed from this variable to extend the system.
			return true;
		}
		break;
	case variableType::ROAD :
		right = currentVar;
		right++;

		//Production 1 - delete roads which have expired
		if (currentVar->delay < 0) {
			return true;
		}
		//Production 3, delete roads next to failed insertions.
		else if (right->state == solutionState::FAILED) {
			return true;
		} else {
			//Production 2 - right iterator is a succeeded insertion. Create new branches / road / insertion module with parameters set by
			//global goals. Add this road and vertices to the system.
			if (right->varType == variableType::INSERTION && right->state == solutionState::SUCCEED) {
				BOOST_FOREACH(Variable lol, *applyGlobalConstraints(currentVar->rules, right->road) ) {
					productions->push_back(lol);
				}

				//TODO :: Properly insert into new system, with roac crossings
				//addRoadToSystem(currentVar->rules, right->road);

				return true;
			}
		}
			
		break;
	default: break;
	}

	return false;
}

void StreetGen::Run() {
	
	while (!finished) {
		nextIteration();
	}
}

void StreetGen::nextIteration() {
	assert(toInsert.empty());

	printProductions();

	bool changed = false;
	//Apply rules to each element in turn, track for finish
	VarIterator it = current.begin();
	VarList *productions = new VarList();

	while (it != current.end()) {
		if (applyRule(it, productions)) {
			//Production has occured, add this to the list
			toInsert.emplace_back(it, VarList(*productions));
			productions->clear();
			changed = true;
		}

		it++;
	}

	delete productions;

	afterIteration();

	finished = !changed;
	iterationCount++;

	streets.getScene()->update();
}

//
void StreetGen::initialise() {
	streets.reset();

	current.clear();
	toInsert.clear();
	current = getInitialProduction();

	ready = true;
	finished = false;
	iterationCount = 0;
}

bool StreetGen::isReady() {
	return ready;
}

bool StreetGen::isFinished() {
	return finished;
}

void StreetGen::printProductions() {

	std::string varString;

	qDebug() << "Iteration: " << iterationCount << ", nodes: " << current.size();

	for (VarIterator it = current.begin(); it != current.end(); it++) {
		varString += it->toString();
	};
	qDebug() << varString.data();

	if (current.size() == 0)
		qDebug() << "NO PRODUCTIONS";

	qDebug() << "";
	qDebug() << "Roads generated: " << streets.roadCount();
	qDebug() << "Verts generated: " << streets.vertCount();
	qDebug() << "ITEMS COUNT: " << streets.getScene()->items().count();
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
	genX = boost::random::uniform_int_distribution<>(0, size.x());
	genY = boost::random::uniform_int_distribution<>(0, size.y());
}

QGraphicsScene *StreetGen::getScene() {
	return streets.getScene();
}
