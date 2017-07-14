#pragma once
#include <list>
#include <vector>
#include <QRect>

#include "BuildingVariable/BuildingVariable.h"
#include "Enums.cpp"

class BuildingLot;

//Copying LSystem because no decent generic programming in this language
class BuildingLSystem {
public:
	///Performs one iteration of the L-System
	void setDesiredLOD(int LOD);
	void nextIteration();
	void initialiseLSystem();
	void Run();

	BuildingLSystem();
	BuildingLSystem(BuildingLot *lot);
	~BuildingLSystem();
	
	QRect &getConstructionArea();
	void getGeometry(std::vector<Cube> &out);

protected:
	typedef std::list<BuildingVariable> VarList;
	typedef std::list<BuildingVariable>::iterator VarIterator;

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
	virtual void printProductions();

	//Manage l sys
	VarList current;
	std::list<std::pair<VarIterator, VarList>> toInsert;
	bool finished;

	BuildingLot *lot;
	QRect constructionArea;

	int currentLOD;
	int desiredLOD;

private:
	void replaceVariables();
};

