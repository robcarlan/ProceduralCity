#include "Variable.h"

Variable::Variable(variableType vType, int delay, ruleAttr rules, roadAttr road) {
	varType = vType;
	this->delay = delay;
	this->rules = rules;
	this->road = road;
	this->state = solutionState::UNASSIGNED;
}

Variable::Variable(variableType vType) {
	this->varType = vType;
	this->state = solutionState::UNASSIGNED;
}

Variable::Variable(variableType vType, int delay) {
	this->varType = vType;
	this->delay = delay;
	this->state = solutionState::UNASSIGNED;
}

Variable::Variable() {
	this->state = solutionState::UNASSIGNED;
}


Variable::~Variable() {
}

const char * enumToString[] = { "UNASSIGNED", "SUCCEED", "FAILED" };

std::string Variable::toString() {
	switch (varType) {
		case (variableType::EMPTY) :
			return std::string("e");
		case (variableType::INSERTION) : {
			std::string res = "?I(";
			res.append(enumToString[state]);
			res.append(")");
			return res;
		}
		case (variableType::ROAD) : {
			std::string res = "R(";
			res += std::to_string(delay);
			res.append(")");
			return res;
		}
		case (variableType::BRANCH) : {
			std::string res = "B(";
			res += std::to_string(delay);
			res.append(")");
			return res;
		} default: return std::string();
	}
};
