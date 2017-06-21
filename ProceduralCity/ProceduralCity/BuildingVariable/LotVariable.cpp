#include "LotVariable.h"
#include "BuildingVariable.h"
#include "HouseVariable.h"
#include "UrbanHouse.h"
#include "CubeVariable.h"
#include "IndustrialVariable.h"

LotVariable::LotVariable() {
}

LotVariable::~LotVariable() {
}

bool LotVariable::getNextProductions(std::list<BuildingVariable> &out) {
	switch (style) 
	{
	case COMMERCIAL: {
		createCommercial(out);
		break;
	}
	case INDUSTRIAL: {
		createIndustrial(out);
		break;
	}
	case RESIDENTIAL: {
		createResidential(out);
		break;
	}

	default: {
		createCommercial(out);
	}
	}

	return true;
}

void LotVariable::createIndustrial(std::list<BuildingVariable>& out) {
	//At the moment, just create a simple block
	height = getBetween(rand(), industrialMinHeight, industrialMaxHeight);

	BuildingVariable variable = BuildingVariable(BuildingVariableType::INDUSTRIALBUILDING);
	IndustrialVariable * industrialVar = new IndustrialVariable(heightStart, heightStart + height, origin, width, depth, rot);
	variable.setVariableData(dynamic_cast<GeometryVariable*>(industrialVar));
	industrialVar->setSeed(seed);

	int numChimneys = rand() % 3;

	float chimneyHeight = getBetween(rand(), chimneyMinHeight, chimneyMaxHeight);
	float chimenyWidth = width / 10.0f;
	float spacing = chimenyWidth + chimenyWidth / 2;

	Point chimneyStart = origin;

	for (int i = 0; i < numChimneys; i++) {

		BuildingVariable chimneyGeom = BuildingVariable(BuildingVariableType::CUBE);
		CubeVariable * chimney = new CubeVariable(heightStart + height, heightStart + height + chimneyHeight, chimneyStart, chimenyWidth, chimenyWidth, rot);
		chimneyGeom.setVariableData(dynamic_cast<GeometryVariable*>(chimney));
		chimney->setSeed(seed);

		out.push_back(chimneyGeom);

		chimneyStart.setX(chimneyStart.x() - spacing);
	}

	out.push_back(variable);

}

void LotVariable::createResidential(std::list<BuildingVariable>& out) {
	//Dependent on height also
	bool useBlock = false;
	bool isHighrise = false;
	bool hasGarden = false;

	//Have a house variables which is a bbox.
	//Refines to 3 / 4 cubes to give some structure. Houses on the same street can be given the same seed also.

	if (popDensity > urbanHouseMinPopDensity) {
		//Create an urban house
		height = getBetween(rand(), urbanHouseHeightMin, urbanHouseHeightMax);

		BuildingVariable variable = BuildingVariable(BuildingVariableType::URBANHOUSE);
		UrbanHouse * houseVar = new UrbanHouse(heightStart, heightStart + height, origin, width, depth, rot);
		variable.setVariableData(dynamic_cast<GeometryVariable*>(houseVar));
		houseVar->setSeed(seed);

		out.push_back(variable);
	}
	else {
		//Create a suburban house
		height = suburbanHouseHeight;
		depth = depth / 2; //Leave second half for garden
		origin.setY(origin.y() - depth / 4); //Push back into front half

		BuildingVariable variable = BuildingVariable(BuildingVariableType::HOUSE);
		HouseVariable * houseVar = new HouseVariable(heightStart, heightStart + height, origin, width, depth, rot);
		variable.setVariableData(dynamic_cast<GeometryVariable*>(houseVar));
		houseVar->setSeed(seed);

		out.push_back(variable);
	}
}

void LotVariable::createCommercial(std::list<BuildingVariable>& out) {
	//Dependent on height, is it a skyscraper etc.
	//Calculate 'bounding boxes' for each part
	srand(seed);

	bool useBase = rand() % 100 < createBaseProb * 100.0f;
	useBase = true;
	bool createRoof = rand() % 100 < createRoofProb * 100.0f;
	bool multiSplit = rand() % 100 < doubleTowerPossibility * 100.0f;
	multiSplit = false; //Disabled right now
	bool createTower = (rand() % 100 < createTowerProb * 100.0f);

	float baseHeight = useBase ? getBetween(rand(), minBaseHeight, maxBaseheight) : 0.0f;
	float towerHeight = getBetween(rand(), minCommercialHeight, maxCommercialHeight);

	float towerWidth = bounds.width();
	float towerBreadth = bounds.height();

	if (useBase) {
		towerWidth *= getBetween(rand(), minTowerSizeReduceFactor, 1.0f);
		towerBreadth *= getBetween(rand(), minTowerSizeReduceFactor, 1.0f);
	}

	float roofHeight = getBetween(rand(), minRoofHeight, maxRoofHeight);

	Point RoofSize;

	if (isSkyscraper) {
		//Skyscraper specific adjustments
		towerHeight += skyscraperHeightAddition;
	}

	if (createTower) {
		RoofSize.setX(towerWidth);
		RoofSize.setY(towerBreadth);
	}
	else {
		//Scale roof size

		//TODO :: Chance of square roof
		float minRoofReduceX = towerWidth * minRoofSizeFactor;
		float minRoofReduceY = towerBreadth * minRoofSizeFactor;
		RoofSize.setX(getBetween(rand(), minRoofReduceX, towerWidth));
		RoofSize.setY(getBetween(rand(), minRoofReduceY, towerBreadth));
	}

	float buildingBottomHeight = boundBox.getBase().z();
	float wspBaseEnd = buildingBottomHeight + baseHeight;
	float wspRoofStart = buildingBottomHeight + towerHeight;

	//Create new variables
	if (useBase) {
		BuildingVariable base = BuildingVariable(BuildingVariableType::BASE);
		BaseVariable * baseVar = new BaseVariable(buildingBottomHeight, wspBaseEnd, origin, Point(bounds.x(), bounds.y()),
			bounds.width(), bounds.height(), rot);
		base.setVariableData(dynamic_cast<GeometryVariable*>(baseVar));
		baseVar->setSeed(seed);

		out.push_back(base);
	}

	//If create tower, we build one large block the tower variable, wihch doesn't need to refine
	if (multiSplit) {
		//Use the same seed, probably
	}
	else {
		//Create just the one tower
		BuildingVariable tower = BuildingVariable(BuildingVariableType::TOWER);
		TowerVariable * towerVar = new TowerVariable(
			wspBaseEnd, wspRoofStart, origin, towerWidth, towerBreadth, rot);

		tower.setVariableData(dynamic_cast<GeometryVariable*>(towerVar));
		towerVar->setSeed(seed);
		out.push_back(tower);
	}

	if (createRoof) { //Check for multisplit
		BuildingVariable roof = BuildingVariable(BuildingVariableType::ROOF);
		RoofVariable *roofVar = new RoofVariable(
			wspRoofStart, wspRoofStart + roofHeight, origin, RoofSize.x(), RoofSize.y(), rot);

		roof.setVariableData(dynamic_cast<GeometryVariable*>(roofVar));
		roofVar->setSeed(seed);
		out.push_back(roof);
	}
}

void LotVariable::getGeometry(std::vector<Cube>& out) {
	out.push_back(boundBox);
}

QRect & LotVariable::getBounds() {
	return bounds;
}

float LotVariable::getHeight() {
	return height;
}

LotVariable::LotVariable(BuildingLot * base) : GeometryVariable() {
	//Create lot by creating a rectangle from the centroid
	Point centroid = base->getCentroid();

	//Creates bounding box of building
	height = 4.0f * base->getPopDensity() + 1.0f;
	popDensity = base->getPopDensity();

	isSkyscraper = base->getIsSkyscraper();


	float width = 8.0f;
	float depth = 6.0f;
	float rot = 0.0f;
	auto points = base->getPoints();
	bool found = false;

	if (base->getIsQuad()) {
		//Four points to check
		auto pItr = points.begin();
		auto p1 = *(pItr++);
		auto p2 = *(pItr++);
		auto p3 = *(pItr++);
		auto p4 = *(pItr++);

		float vDif1 = (p1.y() - p2.y());
		float vDif2 = (p2.y() - p3.y());
		float vDif3 = (p3.y() - p4.y());
		float vDif4 = (p4.y() - p1.y());
		float xDif1 = (p1.x() - p2.x());
		float xDif2 = (p2.x() - p3.x());
		float xDif3 = (p3.x() - p4.x());
		float xDif4 = (p4.x() - p1.x());

		if (abs(xDif1) < 1.0f) {
			//First line is vertical
			if (abs(vDif2) < 1.0f && abs(xDif3) < 1.0f && abs(vDif4) < 1.0f) {
				//Lines are relatively straight

				if (abs(vDif1 + vDif3) < 1.0f && abs(xDif1 + xDif3) < 1.0f) {
					//Lines are relatively same length
					rot = 0.0f;
					width = abs(xDif2);
					depth = abs(vDif1);
					//found = true;
				}
			}
		}
		else if (abs(vDif1) < 1.0f) {
			//First line is horizontal
			if (abs(xDif2) < 1.0f && abs(vDif3) < 1.0f && abs(xDif4) < 1.0f) {
				//Lines are relatively straight

				if (abs(xDif1 + xDif3) < 1.0f && abs(vDif1 + vDif3) < 1.0f) {
					//Lines are relatively same length
					rot = 0.0f;
					width = abs(vDif1);
					depth = abs(xDif2);
					//found = true;
				}
			}
		}


		if (!found) {
			//More work necessary (4 points - brute forceable for sure fam)
			float l1 = p1.getDistance(p2);
			float l2 = p2.getDistance(p3);
			float l3 = p3.getDistance(p4);
			float l4 = p4.getDistance(p1);

			if (abs(l1 - l3) < 1.0f && abs(l2 - l4) < 1.0f) {
				//We have a rotated cube
				rot = atan2f(p2.x() - p1.x(), p2.y() - p1.y());
				width = l1;
				depth = l2;
				found = true;
			}
			else {
				//Pick minimum of thses, form a tight fitting rotated cube.
				Point a, b;
				if (l1 < l3) {
					width = l1;
					a = p1;
				}
				else {
					width = l3;
					a = p3;
				}

				//Pick height
				if (l1 < l3) {
					height = l2;
					b = p2;
				}
				else {
					height = l4;
					b = p4;
				}

				rot = atan2f(p2.x() - p1.x(), p2.y() - p1.y());
				found = true;
			}
		}
	}
	else if (base->getIsTriangle()) {
		auto pItr = points.begin();
		auto p1 = *(pItr++);
		auto p2 = *(pItr++);
		auto p3 = *(pItr++);

		QLineF first = QLineF(p1, p2);
		QLineF second = QLineF(p2, p3);
		QLineF third  = QLineF(p3, p1);
		 
		QLineF toMid = first.unitVector();
		toMid.setLength(first.length() / 2);
		//Calc mid of p1, p2
		Point midFirst = toMid.p2();

		toMid = second.unitVector();
		toMid.setLength(second.length() / 2);
		//Calc mid of p2, p3
		Point midSecond = toMid.p2();

		//Now we have mid of l1, and l2. Calculate rectangle by getting the intersection with l3's normal vector.
		QLineF thirdNormal = third.normalVector();
		QLineF firstNormal = QLineF(
			midFirst.x(), midFirst.y(), midFirst.x() + thirdNormal.dx(), midFirst.y() + thirdNormal.dy());
		QLineF secondNormal = QLineF(
			midSecond.x(), midSecond.y(), midSecond.x() + thirdNormal.dx(), midSecond.y() + thirdNormal.dy());

		Point intersectBL;
		firstNormal.intersect(third, &intersectBL);
		Point intersectBR;
		secondNormal.intersect(third, &intersectBR);

		//Now we have a rotated quad, we can get rotation by angle of normal vector.
		//Rot is in degrees
		rot = thirdNormal.angle();

		//Depth is the length of intersectBL to midFirst. This should be the same as dist (intersectBR, midSecond)
		depth = midFirst.getDistance(intersectBL);
		//Width is the length from intersectBL to intersectBR
		width = intersectBL.getDistance(intersectBR);
	}

	//Heuristic - grow a rectangle along the longest edge?
	//Accept first rectangle within a threshold area, growing from each edge.

	if (!found) {
		QPolygon fit;

		for (auto itr = base->getPoints().begin(); itr != base->getPoints().end(); itr++) {
			fit.append(QPoint(itr->x(), itr->y()));
		}

		//Get longest edge?
		for (int degItr = 0; degItr < 360; degItr += 30) {
			
		}
	}

	//TODO :: Proper bounds calculation
	bounds = QRect(centroid.x() - width / 2, centroid.y() - depth / 2, width, depth);

	boundBox = Cube(QVector3D(centroid.x(), centroid.y(), base->getHeight()), QVector3D(width, depth, height), rot);
	heightStart = boundBox.getBase().z();

	style = base->getStyle();
	seed = base->getSeed();
	origin = centroid;

	this->width = width;
	this->depth = depth;
	this->rot = rot; // -1 * rot + 90;
}

