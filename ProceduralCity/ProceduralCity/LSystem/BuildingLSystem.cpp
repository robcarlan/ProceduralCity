#include "BuildingLSystem.h"

BuildingLSystem::BuildingLSystem() {
	finished = false;
}

BuildingLSystem::BuildingLSystem(BuildingLot * lot) {
	this->lot = lot;
}

BuildingLSystem::~BuildingLSystem() {
}

QRect &BuildingLSystem::getConstructionArea() {
	return constructionArea;
}

void BuildingLSystem::getGeometry(std::vector<Cube>& out) {
	for (auto sysItr = current.begin(); sysItr != current.end(); sysItr++) {
		if (sysItr->vtype == BuildingVariableType::EMPTY) continue;

		sysItr->getGeometry(out);
	}
}

void BuildingLSystem::Run() {
	//Iteration generates highways before streets
	while (!finished) {
		nextIteration();
	}
}

void BuildingLSystem::setDesiredLOD(int LOD) {
	desiredLOD = LOD;
}

void BuildingLSystem::nextIteration() {
	if (!finishedIteration) return;
	finishedIteration = false;

	assert(toInsert.empty());

	printProductions();

	bool changed = false;

	//Generic method to allow processing before the iteration
	LSystemBeforeIterationBegin();

	//Apply rules to each element in turn, track for finish
	VarIterator it = current.begin();
	VarList productions = VarList();

	while (it != current.end()) {
		if (applyRule(it, &productions)) {
			//Production has occured, add this to the list
			//Empty production => delete current variable.
			if (productions.size() == 0) {
				productions.push_back(BuildingVariable(BuildingVariableType::EMPTY));
				toInsert.emplace_back(it, VarList(productions));
			}
			else {
				toInsert.emplace_back(it, VarList(productions));
			}
			productions.clear();
			changed = true;
		}

		it++;
	}

	replaceVariables();

	//Generic to allow process after iteration end
	LSystemAfterIteration(changed);

	finished = isLSystemFinished(changed);

	assert(toInsert.empty());

	iterationCount++;
	finishedIteration = true;
}


void BuildingLSystem::initialiseLSystem() {
	current.clear();
	toInsert.clear();
	current = getInitialProduction();
	finishedIteration = true;
	iterationCount = 0;
	finished = false;

	//
	currentLOD = 0;
	desiredLOD = 1;
}


bool BuildingLSystem::applyRule(VarIterator currentVar, VarList * productions) {
	currentVar->getNextProductions(*productions);

	return (productions->size() > 0);
}


BuildingLSystem::VarList BuildingLSystem::getInitialProduction() {
	//Create lot where building is constructed
	VarList initial = std::list<BuildingVariable>();
	initial.push_back(BuildingVariable(lot));
	return initial;
}


bool BuildingLSystem::isLSystemFinished(bool changed) {
	return currentLOD >= desiredLOD;
}


void BuildingLSystem::LSystemBeforeIterationBegin() {
}


void BuildingLSystem::LSystemAfterIteration(bool changed) {
	currentLOD++;
}


void BuildingLSystem::printProductions() {
}

//Splice new variables into position, then remove all empty variables

void BuildingLSystem::replaceVariables() {
	//Only replace if toInsert isn't empty
	//if (toInsert.size() == 0) return;

	VarIterator it = current.begin();
	std::list<std::pair<VarIterator, VarList>>::iterator replaceItr = toInsert.begin();

	while (it != current.end()) {
		//Delete all e variables
		//TODO :: hopefully redundant

		//GENERIC emptyVariable case

		if (it->vtype == BuildingVariableType::EMPTY) {
			it = current.erase(it);
			//TODO :: This?
			continue;
		}

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
			}
			else {
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
