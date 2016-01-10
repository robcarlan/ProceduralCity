#include "StreetManager.h"

StreetManager::BOOST_POINT StreetManager::toBoostPoint(Point val) {
	return  boost::geometry::model::d2::point_xy<float>(val.x(), val.y());
}

StreetManager::BOOST_SEGMENT StreetManager::toBoostSegment(Road *val) {
	return boost::geometry::model::segment<BOOST_POINT>(toBoostPoint(val->getStart()), toBoostPoint(val->getEnd()));
}

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
	return roadTree.size();
}

int StreetManager::vertCount() {
	return intersectionTree.size();
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

	roadTree.clear();
	intersectionTree.clear();

	QList<QGraphicsItem*> emptyRoads = QList<QGraphicsItem*>();
	QList<QGraphicsItem*> emptyIntersections = QList<QGraphicsItem*>();
	intersectionsRender = scene->createItemGroup(emptyIntersections);
	roadsRender = scene->createItemGroup(emptyRoads);
	scene->setSceneRect(QRectF(-100, -100, 2248, 2248));

	intersectionsRender->setVisible(renderVerts);
}

//Simple search on intersections to find the closest intersection
StreetManager::intersectionRec* StreetManager::getClosest(Point queryPoint, std::vector<intersectionRec>& nearby) {
	StreetManager::intersectionRec* smallest = nearby.begin().operator->();

	for (std::vector<intersectionRec>::iterator it = nearby.begin(); it != nearby.end(); it++) {
		if (queryPoint.getDistanceSq(it->first) < queryPoint.getDistanceSq(smallest->first) && !queryPoint.isWithinBounds(it->first, 1.0f))
			smallest = it.operator->();
	}

	return smallest;
}

void StreetManager::getNearbyVertices(Point queryPoint, double radius, std::vector<RoadIntersection*>& nearby) {

	std::vector<intersectionIndex> result;

	//Perform knn to find 3 nearest neighbours.
	intersectionTree.query(boost::geometry::index::nearest(toBoostPoint(queryPoint), 3), std::back_inserter(result));

	for (std::vector<intersectionIndex>::iterator it = result.begin(); it != result.end(); it++) {
		QPointF pt = QPointF(it->first.x(), it->first.y());

		//The result is only knn, so may not be within desired bounds at all.
		if (queryPoint.isWithinBounds(pt, 10.0f))
			nearby.push_back(it->second);
	}
}

void StreetManager::getIntersectingEdges(Road edge, std::vector<intersectionRec>& nearby) {

	std::vector<roadIndex> result;

	//Query road tree to find intersects with edge. Store in result.
	roadTree.query(boost::geometry::index::intersects(toBoostSegment(&edge)), std::back_inserter(result));

	for (std::vector<roadIndex>::iterator it = result.begin(); it != result.end(); it++) {
		std::unique_ptr<QPointF> intersectPoint(new QPointF);
		QLineF::IntersectType intersectType = it->second->intersect(edge, intersectPoint.get());

		//Find all intersect points, check whether they are bounded and at least some distance away from the start.
		if (intersectType == QLineF::IntersectType::BoundedIntersection)
			if (!edge.getStart().isWithinBounds(*intersectPoint, 1.0f))
				nearby.push_back(intersectionRec(Point(*intersectPoint), it->second));
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

	if (start->connected.size() == 1) insertIntersection(start);

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

	if (start->connected.size() == 1) insertIntersection(start);

	//Create new Intersection, attach to road
	RoadIntersection *crossing = new RoadIntersection(Point(newRoad->getEnd()), newRoad, target);
	crossing->attachRoad(newRoad);
	newRoad->addIntersection(crossing);
	insertIntersection(crossing);

}
void StreetManager::insertRoad(Road * toAdd) {
	//Vector style
	//roads.push_back(toAdd);

	//Rtree
	roadTree.insert(std::make_pair(toBoostSegment(toAdd), toAdd));

	QGraphicsLineItem *road = scene->addLine(QLineF(toAdd->getStart(), toAdd->getEnd()));
	roadsRender->addToGroup(road);
}

void StreetManager::insertIntersection(RoadIntersection * toAdd) {
	//Vector style
	//intersections.push_back(toAdd);

	//Rtree
	intersectionTree.insert(std::make_pair(toBoostPoint(toAdd->location), toAdd));

	scene->addRect(toAdd->boundingRect());
}
