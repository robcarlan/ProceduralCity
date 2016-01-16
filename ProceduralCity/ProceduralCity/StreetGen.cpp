#include "StreetGen.h"

using namespace boost::geometry;

const float StreetGen::roadBranchProb = 0.9f;

//Global constraints
const float StreetGen::maxAngleSearch = 1.72f;
const float StreetGen::popDensityRadiusSearch = 200.0f;
const float StreetGen::d2rFactor = 3.14159265f / 180.0f;
const float StreetGen::r2dFactor = 180.0f / 3.14159265f;

const float StreetGen::manhattanBlockWidth = 30.0f;
const float StreetGen::manhattanBlockHeight = 20.0f;

//Geography Fitting
const float StreetGen::maxRoadRotate = 0.7f;
const float StreetGen::roadRotateInterval = 0.1f;
const float StreetGen::roadSampleInterval = 2;
const float StreetGen::maxWaterTraverse = 35.0f;
const float StreetGen::maxPruneFactor = 0.75f;

//Intersection fitting
const float StreetGen::extendRadius = 50.0f;
const float StreetGen::minDistanceSq = 225.0f;
const float StreetGen::minLength = 10.0f;

const int StreetGen::BRANCH1 = 0;
const int StreetGen::BRANCH2 = 1;
const int StreetGen::ROAD = 2;

const int StreetGen::NATURAL_PATTERN_INDEX = 0;
const int StreetGen::SF_PATTERN_INDEX = 1;
const int StreetGen::MANHATTAN_PATTERN_INDEX = 2;
const int StreetGen::RADIAL_PATTERN_INDEX = 3;

void StreetGen::setSeed(int seed) {
	rng.seed(seed);
}

void StreetGen::setSeed() {
	rng.seed(static_cast<unsigned int>(std::time(0)));
}


StreetGen::VarList* StreetGen::applyGlobalConstraints(ruleAttr rules, roadAttr roads) {
	//Create 3 sets of new variables

	if (roads.connected) {
		VarList *result = new VarList();
		result->push_back(Variable(variableType::EMPTY));
		return result;
	}

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

		//Calculate new position (end, angle, length) for each road pattern. (Apart from if value = 0
		float endx[4];
		float endy[4];
		float angle[4];
		float length[4];

		//Apply road pattern
		float scale = uniform(rng);
		nroads[i].start = roads.end;
		delay[i] = i;
		nroads[i].angle = roads.angle;

		//Road branches become streets with a certain probability
		if (roads.rtype == roadType::MAINROAD) {
			float branchProb1 = uniform(rng);
			float branchProb2 = uniform(rng);

			if (branchProb1 > roadBranchProb)
				nroads[BRANCH1].rtype = roadType::MAINROAD;
			else nroads[BRANCH1].rtype = roadType::STREET;
			if (branchProb2 > roadBranchProb)
				nroads[BRANCH2].rtype = roadType::MAINROAD;
			else nroads[BRANCH1].rtype = roadType::STREET;
		}
	}

	//Set branches to point at right angle
	nroads[BRANCH1].angle += math::half_pi<float>();
	nroads[BRANCH2].angle -= math::half_pi<float>();
	delay[BRANCH1] = 5;
	delay[BRANCH2] = 5;

	//Update roots

	//Set all roads to have some new length
	float newLength = 20.0f;
	nroads[BRANCH1].length = newLength;
	nroads[BRANCH2].length = newLength;
	nroads[ROAD].length = newLength;

	//Calculate new end positions
	for (int i = 0; i < 3; i++) {
		//Create unit vector in direction
		nroads[i].angle = math::mod<float>(nroads[i].angle, math::two_pi<float>());
		Point unit = Point(cosf(nroads[i].angle), sinf(nroads[i].angle));
		unit *= nroads[i].length;
		nroads[i].end = nroads[i].start + unit;

		//Apply street patterns

		//Get each weighting by sampling
		float values[4];
		getPatternWeightings(roads.end.x(), roads.end.y(), values);
		Point ends[4];
		int patternCount = 0;

		if (values[NATURAL_PATTERN_INDEX] - 0.01f > 0) {
			ends[NATURAL_PATTERN_INDEX] = naturalRule(&nroads[i]);
		}

		if (values[MANHATTAN_PATTERN_INDEX] - 0.01f > 0) {
			ends[MANHATTAN_PATTERN_INDEX] = manhattanRule(&nroads[i]);
		}

		if (values[SF_PATTERN_INDEX] - 0.01f > 0) {
			ends[SF_PATTERN_INDEX] = sanFransiscoRule(&nroads[i]);
		}

		if (values[RADIAL_PATTERN_INDEX] - 0.01f > 0) {
			ends[RADIAL_PATTERN_INDEX] = radialRule(&rules, &roads);
		}

		nroads[i].end = weighValues(values, ends, &roads.end);
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

void StreetGen::applyLocalConstraints(Variable *toCheck) {
	Road temp = Road(toCheck->road.start, toCheck->road.end);
	bool connectedToIntersection = false;
	bool legalPlacement = tryMakeLegal(toCheck, &temp);
	bool legalIntersection = tryConnectToExisting(toCheck, &temp, connectedToIntersection);

	if (legalPlacement && legalIntersection) toCheck->state = solutionState::SUCCEED;
	else toCheck->state = solutionState::FAILED;

	//Stop growing street
	if (connectedToIntersection) 
		toCheck->road.connected = true;
}

bool StreetGen::tryMakeLegal(Variable * toCheck, Road * tempRoad) {

	bool isLegal = false;
	bool withinBounds;
	bool legalSegment;
	bool endsLegal;
	//Others to check are pop density, gradient etc.
	float startAngle = (tempRoad->angle()) * math::two_pi<float>() / 180.0f;
	float angleChange = 0.0f;
	float angleToCheck = startAngle;
	QPointF newEnd = toCheck->road.end;
	float pruneFactor = 1.0f;

	endsLegal = sampleGeog(newEnd.x(), newEnd.y()) == geogType::LAND;
	withinBounds = (tempRoad->getEnd().x() > 0) && (tempRoad->getEnd().y() > 0) &&
		(tempRoad->getEnd().x() < size.x()) && (tempRoad->getEnd().y() < size.y());
	getIllegalSegment(*tempRoad, legalSegment);
	
	isLegal = endsLegal && withinBounds && legalSegment;

	while (!isLegal) {
		//Update step
		if (angleChange > maxRoadRotate) return false;
		if (pruneFactor > maxPruneFactor) pruneFactor -= 0.04f;
		else {
			//Update angle once all prunes have been tried (yum)
			pruneFactor = 1.0f;
			if (angleChange > 0) angleChange = -angleChange;
			else angleChange = -angleChange + roadRotateInterval;
		}

		//Rotate line, set line length
		if (pruneFactor == 1.0f) {
			//Calculate new end position by rotating line
			angleToCheck = startAngle + angleChange;				
			QLineF newLine = QLineF(tempRoad->getStart(), tempRoad->getEnd());
			newLine.setAngle(angleToCheck);
			newEnd = newLine.p2();
		}
		else {
			//update position by shortening
			newEnd = tempRoad->lerp(pruneFactor);
		}

		Road segment = Road(tempRoad->getStart(), newEnd);
		//recalculate booleans
		endsLegal = (sampleGeog(newEnd.x(), newEnd.y()) == geogType::LAND);
		withinBounds = (tempRoad->getEnd().x() > 0) && (tempRoad->getEnd().y() > 0) &&
			(tempRoad->getEnd().x() < size.x()) && (tempRoad->getEnd().y() < size.y());

		getIllegalSegment(segment, legalSegment);

		isLegal = endsLegal && withinBounds && legalSegment;
	}

	tempRoad->setEnd(newEnd);
	toCheck->road.end = newEnd;
	toCheck->road.angle = angleToCheck;
	toCheck->road.length *= pruneFactor;
	return true;
}

void StreetGen::getIllegalSegment(Road segment, bool &legal) {
		float rLength = segment.length();

		float numSamples = rLength / roadSampleInterval;
		float xInc = cosf(segment.angle()) * roadSampleInterval;
		float yInc = sinf(segment.angle()) * roadSampleInterval;
		int begin = -1;
		int end = 0;
		int maxSamples = maxWaterTraverse / roadSampleInterval;

		Point curPos = segment.getStart();

		for (int i = 0; i < numSamples; i++) {
			curPos.setX(curPos.x() + xInc);
			curPos.setY(curPos.y() + yInc);
			geogType geog = sampleGeog(curPos.x(), curPos.y());

			if (geog != geogType::LAND) {
				//Illegal, update boundaries
				if (begin == -1) {
					begin = i;

				}
				else if (end == i - 1) end++;
				else {
					//We have found another piece of land / water, we should deal with this
					end++;
				}
			}

			if ((end - begin) > maxSamples) {
				legal = false;
				return;
			}
		}

		legal = true;
		return;
}

bool StreetGen::tryConnectToExisting(Variable * toCheck, Road *tempRoad, bool &connectedToIntersection) {
	std::vector<StreetManager::intersectionRec> intersections;
	streets.getIntersectingEdges(*tempRoad, intersections);

	bool legalPlacement = false;

	//Road intersects with other roads in the system
	if (intersections.size() > 0) {
		auto minIntersect = streets.getClosest(toCheck->road.start, intersections);

		toCheck->road.end = minIntersect->first;
		//Add a crossing, leave target as null as we need to create this
		toCheck->road.targetRoad = minIntersect->second;
		toCheck->state = solutionState::SUCCEED;
		legalPlacement = true;
		connectedToIntersection = true;
	}
	else { //Road does not intersect, so lets see if it is near to an existing crossing.

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
					connectToIntersection = true; legalPlacement = true;
					break;
				}
			}
		}

		//Extend the road by a maximum radius, then see if it intersects.
		if (!connectToIntersection) {
			legalPlacement = true;
			QLineF norm = tempRoad->normalVector();
			norm.setLength(extendRadius);
			Point extended = Point(toCheck->road.start.x() + norm.dx(), toCheck->road.start.y() + norm.dy());
			streets.getIntersectingEdges(Road(toCheck->road.start, extended), intersections);

			if (intersections.size() > 0 && toCheck->road.start.getDistanceSq(extended) > minDistanceSq) {
				toCheck->road.end = extended;
				connectToIntersection = true;

			}
		}
	}

	return legalPlacement;
}

void StreetGen::addRoadToSystem(roadAttr roads) {
	//Create first intersection if necessary
	std::vector<RoadIntersection*> nearby;

	//If nearby just attach to existing one
	//TODO ie when starting at an existing intersection
	RoadIntersection *start = new RoadIntersection(roads.start, roads.parentRoad);
	//streets.getNearbyVertices(*start, 5.0f, nearby); 
	Road *road = new Road(roads.start, roads.end, roads.rtype);

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
	road.end = Point(800, 800);
	road.start = Point(800, 700);
	road.length = math::sqrt(road.start.getDistanceSq(road.end));
	road.rtype = roadType::MAINROAD;
	road.connected = false;

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

float StreetGen::sampleHeight(int x, int y) {
	QColor val = QColor(hMap.pixel(x, y));
	return (val.red() % 256) / 256.0f;
}

geogType StreetGen::sampleGeog(int x, int y) {
	QRgb val = gMap.pixel(x, y);
	if (val == landCol) return geogType::LAND;
	else if (val == waterCol) return geogType::WATER;
	else if (val == parkCol) return geogType::PARK;
	else return geogType::LAND; 
}

float StreetGen::samplePop(int x, int y) {
	QRgb lol = pMap.pixel(x, y);
	return lol % 256;
	QColor val = QColor(pMap.pixel(x, y));
	return (val.red() % 256) / 256.0f;
}

QColor StreetGen::sampleStreet(int x, int y) {
	return QColor(sMap.pixel(x, y));
}

void StreetGen::getPatternWeightings(int x, int y, float weights[]) {
	//Sample pattern map
	QColor val = sampleStreet(x, y);
	
	weights[MANHATTAN_PATTERN_INDEX] = val.blueF();
	weights[NATURAL_PATTERN_INDEX] = val.blackF();
	weights[SF_PATTERN_INDEX] = val.redF();

	//TODO :: IMPLEMENT RADIAL
	weights[RADIAL_PATTERN_INDEX] = 0.0f;
	return;
}

float StreetGen::maxPopDensity(float startAngle, Point &start, Point &end) {
	//Sum weight of rays along center, return max angle
	int maxXVal = end.x();
	int maxYVal = end.y();
	float maxSum = 0.0f;
	
	float curAngle = startAngle - maxAngleSearch;
	curAngle = fmodf(curAngle, math::two_pi<float>());
	float endAngle = startAngle + maxAngleSearch;
	endAngle = fmodf(endAngle, math::two_pi<float>());
	float sectorSearched = 0;
	float roadLength = start.getDistance(end);
	float length = popDensityRadiusSearch;

	Point curPoint = start;

	//Keep track of sector searched so we don't deal with wrap around issues.
	while (sectorSearched < maxAngleSearch) {
		QLineF temp = QLineF::fromPolar(length, curAngle * r2dFactor);
		temp.translate(start);
		
		float sampleQuant = roadSampleInterval / length;
		float sampleInterval = sampleQuant;
		float xIncr = cosf(temp.dx()) * roadSampleInterval;
		float yIncr = sinf(temp.dy()) * roadSampleInterval;

		int xPos = temp.p1().x() + xIncr;
		int yPos = temp.p2().y() + yIncr;
		int cumulativeVal = 0;

		//Sum along the ray , sampling every roadSampleInterval untis
		while (sampleInterval < 1.0f) {
			if (xPos < 0 || xPos > size.x() || yPos < 0 || yPos > size.y()) 
				break;

			cumulativeVal += samplePop(xPos, yPos);
			xPos += xIncr;
			yPos += yIncr;

			sampleInterval += sampleQuant;
		}

		//Scale by distance from desired (guess via angle dif)
		QPoint dif = QPoint(temp.x2() - end.x(), temp.y2() - end.y());
		//cumulativeVal /= abs(fmodf(startAngle - curAngle, math::two_pi<float>()));

		//Update current max
		if (cumulativeVal > maxSum) {
			temp.setLength(roadLength);
			maxXVal = temp.x2();
			maxYVal = temp.y2();
			maxSum = cumulativeVal;
		}

		curAngle += roadRotateInterval;
		sectorSearched += roadRotateInterval;
	}

	//Now update road to point to new direction
	end = Point(maxXVal, maxYVal);
	return maxSum;
}

float StreetGen::getLowestGradient(float startAngle, Point start, float length) {
	float curAngle = startAngle - maxAngleSearch;
	curAngle = fmodf(curAngle, math::two_pi<float>());
	float endAngle = startAngle + maxAngleSearch;
	endAngle = fmodf(endAngle, math::two_pi<float>());

	float minAngle = startAngle;
	float minDensity = 100000.0f;
	float sectorSearched = 0;

	//get density at each sample point, compare to max
	while (sectorSearched < maxAngleSearch) {
		float xAdd = cosf(curAngle) * length;
		float yAdd = sinf(curAngle) * length;
		float sampleDensity = sampleHeight(start.x() + xAdd, start.y() + yAdd);

		//Weigh more to keeping the same direction
		if (curAngle - startAngle < 0.01f) sampleDensity *= 0.7f;

		if (sampleDensity < minDensity) {
			minAngle = curAngle;
			minDensity = sampleDensity;
		}

		sectorSearched += roadRotateInterval;
	}

	return minAngle;
}

Point StreetGen::manhattanRule(const roadAttr* road) {
	//Ensure paths follow same angle - if branch, set length to block width. Otherwise set to block height.
	bool useHeight = useBlockHeight(0, road->angle);

	float lineLength = useHeight ? manhattanBlockHeight : manhattanBlockWidth;
	float xPos = road->start.x() + cosf(road->angle) * lineLength;
	float yPos = road->start.y() + sinf(road->angle) * lineLength;

	return Point(xPos, yPos);
}

bool StreetGen::useBlockHeight(float blockAngle, float curAngle) {
	float dif = fmodf(abs(curAngle - blockAngle), math::two_pi<float>());
	//Return true if angle is close to block angle
	return (dif < math::pi<float>() / 2.0f);
}

Point StreetGen::naturalRule(const roadAttr* road ) {
	//Set path to point to maximum density 
	Point start = road->start;
	Point end = road->end;
	float angle = road->angle;
	maxPopDensity(angle, start, end);
	return end;
}

Point StreetGen::sanFransiscoRule(const roadAttr* road) {
	//Set path to point to minimum gradient of heightmap
	float angleToUse = getLowestGradient(road->angle, road->start, road->length);

	float xPos = road->start.x() + cosf(angleToUse) * road->length;
	float yPos = road->start.y() + sinf(angleToUse) * road->length;

	return Point(xPos, yPos);
}

Point StreetGen::radialRule(const ruleAttr * rules, const roadAttr * roads) {
	//Set path to rotate around centre point. (TODO)
	return Point();
}

//Linearly scale points based on weights
Point StreetGen::weighValues(float weights[], Point * vals, Point *start) {
	float weightSum = weights[0] + weights[1] + weights[2] + weights[3];
	Point cumulativeDistance = Point();

	//Add direction to cumulative
	for (int i = 0; i < 4; i++) {
		if (weights[i] - 0.01f < 0) continue;

		//Scale by weighting
		cumulativeDistance += QPointF(vals[i].rx() * weights[i], vals[i].ry() * weights[i]);
	}

	//Divide by total weighting
	cumulativeDistance /= weightSum;
	return cumulativeDistance;
}

void StreetGen::Run() {
	
	while (!finished) {
		nextIteration();
	}
}

void StreetGen::nextIteration() {
	assert(toInsert.empty());

	//printProductions();

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

	finished = (current.size() == 0);
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

	parkCol = qRgb(100, 170, 100);
	landCol = qRgb(200, 200, 200);
	waterCol = qRgb(140, 140, 220);
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

void StreetGen::setPatternMap(QImage & sMap, bool use) {
	this->sMap = QImage(sMap);
	streets.setPattern(&sMap);
	usePattern = use;
}

void StreetGen::setSize(Point newSize) {
	size = newSize;
	genX = boost::random::uniform_int_distribution<>(0, size.x());
	genY = boost::random::uniform_int_distribution<>(0, size.y());
}

QGraphicsScene *StreetGen::getScene() {
	return streets.getScene();
}
