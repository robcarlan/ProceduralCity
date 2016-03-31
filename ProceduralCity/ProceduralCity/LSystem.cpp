#include "LSystem.h"

LSystem::LSystem() {
	finished = false;
}


LSystem::~LSystem() {
}

void LSystem::Run() {
	//Iteration generates highways before streets
	while (!finished) {
		nextIteration();
	}
}

void LSystem::nextIteration() {
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

	//TODO :: applyRule sometimes returns true and doesn't push back a production
	//Also deletes a road but not insertion
	//Doesnt insert all the vars either

	while (it != current.end()) {
		if (applyRule(it, &productions)) {
			//Production has occured, add this to the list
			//Empty production => delete current variable.
			if (productions.size() == 0) {
				productions.push_back(RoadVariable(variableType::EMPTY));
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

void LSystem::initialiseLSystem() {
	current.clear();
	toInsert.clear();
	current = getInitialProduction();
	finishedIteration = true;
	iterationCount = 0;
	finished = false;
}

bool LSystem::applyRule(VarIterator currentVar, VarList * productions) {
	return false;
}

LSystem::VarList LSystem::getInitialProduction() {
	return VarList();
}

bool LSystem::isLSystemFinished(bool changed) {
	return !changed;
}

void LSystem::LSystemBeforeIterationBegin() {
}

void LSystem::LSystemAfterIteration(bool changed) {
}

void LSystem::printProductions() {
}

//Splice new variables into position, then remove all empty variables
void LSystem::replaceVariables() {
	//Only replace if toInsert isn't empty
	//if (toInsert.size() == 0) return;

	VarIterator it = current.begin();
	std::list<std::pair<VarIterator, VarList>>::iterator replaceItr = toInsert.begin();

	while (it != current.end()) {
		//Delete all e variables
		//TODO :: hopefully redundant

		//GENERIC emptyVariable case

		if (it->varType == variableType::EMPTY) {
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
