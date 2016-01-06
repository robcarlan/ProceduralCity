#include "StreetManager.h"

void StreetManager::setPop(QImage *pop) {
	this->pop = *pop;
	//Create the pixmap now for the scene
	pixPop = new QPixmap();
	pixPop->convertFromImage(*pop);
}

void StreetManager::setHeight(QImage *height) {
	this->height = *height;
	//Create the pixmap now for the scene
	pixHeight = new QPixmap();
	pixHeight->convertFromImage(*height);
}

void StreetManager::setGeog(QImage *geog) {
	this->geog = *geog;
	//Create the pixmap now for the scene
	pixGeog = new QPixmap();
	pixGeog->convertFromImage(*geog);
}

void StreetManager::renderPop() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(*pixPop);
	bg->setZValue(-1.0f);
}

void StreetManager::renderHeight() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(*pixHeight);
	bg->setZValue(-1.0f);
}

void StreetManager::renderGeog() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(*pixGeog);
	bg->setZValue(-1.0f);
}

void StreetManager::renderNone() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = nullptr;
}

void StreetManager::renderVertices(bool renderVerts) {
	this->renderVerts = renderVerts;
}

int StreetManager::roadCount() {
	return roads.size();
}

int StreetManager::vertCount() {
	return intersections.size();
}

StreetManager::StreetManager() {
	scene = new QGraphicsScene();
	pixPop = new QPixmap();
	pixHeight = pixGeog = pixPop;
	scene->setBackgroundBrush(QBrush(QColor::fromRgb(100, 100, 100)));
}


StreetManager::~StreetManager() {
}

QGraphicsScene * StreetManager::getScene() {
	return scene;
}

void StreetManager::reset() {
	scene->clear();

	bg = nullptr;

	intersections.clear();
	roads.clear();

	QList<QGraphicsItem*> emptyRoads = QList<QGraphicsItem*>();
	QList<QGraphicsItem*> emptyIntersections = QList<QGraphicsItem*>();
	intersectionsRender = scene->createItemGroup(emptyIntersections);
	roadsRender = scene->createItemGroup(emptyRoads);
	
	intersectionsRender->setVisible(renderVerts);
}

//Naive method, but it's okay at the moment
void StreetManager::getNearbyVertices(RoadIntersection queryPoint, double radius, std::vector<RoadIntersection*>& nearby) {
	BOOST_FOREACH(RoadIntersection *val, intersections) {
		if (queryPoint.location.getDistanceSq(val->location) < radius)
			nearby.push_back(val);
	}
}

//Naive method, but it's alright at the moment
void StreetManager::getIntersectingEdges(Road edge, std::vector<intersectionRec>& nearby) {
	BOOST_FOREACH(Road *val, roads) {
		if (edge.boundingRect().intersects(val->boundingRect())) {
			//Possible collision, check further
			QPointF * intersectPoint = new QPointF;
			QLineF::IntersectType intersectType = val->intersect(edge, intersectPoint);

			if (intersectType == QLineF::IntersectType::BoundedIntersection) 
				nearby.push_back(intersectionRec(Point(*intersectPoint), val));
		}
	}
}

void StreetManager::branchRoad(RoadIntersection *start, Road *newRoad) {
	//Add new road into the structure
	insertRoad(newRoad);
	
	//Create the new intersection, add to structure
	RoadIntersection *newIntersection = new RoadIntersection(newRoad->getEnd(), newRoad);
	insertIntersection(newIntersection);
}

void StreetManager::connectToExistingIntersection(RoadIntersection * start, Road * newRoad, RoadIntersection * crossing) {
	//Add new road into the structure
	insertRoad(newRoad);
	start->attachRoad(newRoad);
	newRoad->addIntersection(start);

	//Add the intersection into the structure, connect road onto the crossing
	crossing->attachRoad(newRoad);
	newRoad->addIntersection(crossing);
	insertIntersection(crossing);
}

void StreetManager::connectToNewIntersection(RoadIntersection * start, Road * newRoad, Road * target) {
	//Add new road into structure
	insertRoad(newRoad);
	start->attachRoad(newRoad);
	newRoad->addIntersection(start);

	//Create new Intersection, attach to road
	RoadIntersection *crossing = new RoadIntersection(Point(newRoad->getEnd()), newRoad, target);
	crossing->attachRoad(newRoad);
	newRoad->addIntersection(crossing);
	insertIntersection(crossing);

}
void StreetManager::insertRoad(Road * toAdd) {
	roads.push_back(toAdd);

	QGraphicsLineItem *road = scene->addLine(QLineF(toAdd->getStart(), toAdd->getEnd()));
	roadsRender->addToGroup(road);
}

void StreetManager::insertIntersection(RoadIntersection * toAdd) {
	intersections.push_back(toAdd);
	//intersectionsRender->addToGroup(toAdd);
	scene->addRect(toAdd->x(), toAdd->y(), 10, 10);
}
