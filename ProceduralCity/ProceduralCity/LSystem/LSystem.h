#pragma once
#include <list>
#include "Variable.h"
#include "RoadVariable.h"

class LSystem {
public:
	///Performs one iteration of the L-System
	void nextIteration();
	void initialiseLSystem();
	void Run();

protected:
	typedef std::list<RoadVariable> VarList;
	typedef std::list<RoadVariable>::iterator VarIterator;

	bool finishedIteration;
	int iterationCount;

	///Returns true if a production has been applied
	virtual bool applyRule(VarIterator currentVar, VarList *productions);
	///Returns the initial production
	virtual VarList getInitialProduction();

	///Default behaviour :: System finished when no productions remain.
	virtual bool isLSystemFinished(bool changed);
	virtual void LSystemBeforeIterationBegin();
	virtual void LSystemAfterIteration(bool changed);
	virtual void printOutput();

	//Manage l sys
	VarList current;
	std::list<std::pair<VarIterator, VarList>> toInsert;
	bool finished;

	LSystem();
	~LSystem();

private:
	void replaceVariables();
};

