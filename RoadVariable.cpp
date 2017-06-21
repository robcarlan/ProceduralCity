#include "RoadVariable.h"

RoadVariable::RoadVariable(variableType vType, int delay, ruleAttr rules, roadAttr road) {
	varType = vType;
	this->delay = delay;
	this->rules = rules;
	this->road = road;
	this->state = solutionState::UNASSIGNED;
}

RoadVariable::RoadVariable(variableType vType) {
	this->varType = vType;
	this->state = solutionState::UNASSIGNED;
}

RoadVariable::RoadVariable(variableType vType, int delay) {
	this->varType = vType;
	this->delay = delay;
	this->state = solutionState::UNASSIGNED;
}

RoadVariable::RoadVariable() {
	this->state = solutionState::UNASSIGNED;
}


RoadVariable::~RoadVariable() {
}

const char * enumToString[] = { "UNASSIGNED", "SUCCEED", "FAILED" };

std::string RoadVariable::toString() {
	switch (varType) {
		case (variableType::EMPTY) :
			return std::string("e");
		case (variableType::INSERTION) : {
			std::string res = "?I(";
			res.append(enumToString[(int)state]);
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

void roadAttr::recalculateEndPoint() {
	float xIncr = cosf(angle);
	float yIncr = sinf(angle);
	end.setX(start.x() + xIncr * length);
	end.setY(start.y() + yIncr * length);
};

