#include "StreetManager.h"

StreetManager::BOOST_POINT StreetManager::toBoostPoint(Point val) {
	return  boost::geometry::model::d2::point_xy<float>(val.x(), val.y());
}

StreetManager::BOOST_SEGMENT StreetManager::toBoostSegment(Road *val) {
	return boost::geometry::model::segment<BOOST_POINT>(toBoostPoint(val->getStart()), toBoostPoint(val->getEnd()));
}

QPen StreetManager::getRoadPen(Road * road) {
	if (road->rType == roadType::STREET) return roadPen;
	else return mainRoadPen;
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

void StreetManager::setPattern(QImage * pattern) {
	this->pattern = *pattern;
	//Create the pixmap now for the scene
	pixPattern = new QPixmap();
	pixPattern->convertFromImage(*pattern);
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

void StreetManager::renderPattern() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(*pixPattern);
	bg->setZValue(-1.0f);
}

void StreetManager::renderNone() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = nullptr;
}

void StreetManager::renderVertices(bool renderVerts) {
	this->renderVerts = renderVerts;

	intersectionsRender->setVisible(renderVerts);
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

	//Create pens
	roadPen = QPen(QBrush(QColor::fromRgb(50, 50, 50)), 2.0f);
	mainRoadPen = QPen(QBrush(QColor::fromRgb(0, 0, 0)), 4.0f);
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
		if (queryPoint.isWithinBounds(pt, 5.0f))
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

RoadIntersection * StreetManager::getExistingIntersections(Road * query, Road * target) {
	Point roadEnd = query->getEnd();

	for (std::vector<RoadIntersection*>::iterator it = target->intersections.begin(); it != target->intersections.end(); it++) {
		//Look for intersection within a threshold.
		Point intersectionVal = (*it)->location;

		if (abs(roadEnd.x() - intersectionVal.x()) <= 5.0f && abs(roadEnd.y() - intersectionVal.y()) <= 5.0f) {
			return (*it);
		}

	}

	return nullptr;
}

void StreetManager::branchRoad(RoadIntersection *start, Road *newRoad) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);

	//Add new road into the structure
	insertRoad(newRoad);
	
	//Create the new intersection, add to structure
	RoadIntersection *newIntersection = new RoadIntersection(newRoad->getEnd(), newRoad);
	insertIntersection(newIntersection);
}

void StreetManager::connectToExistingIntersection(RoadIntersection * start, Road * newRoad, RoadIntersection * crossing) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);
	assert(newRoad->getEnd().getDistance(crossing->location) < 6.0f);

//	newRoad->setEnd(crossing->location);
	newRoad->addStartIntersection(start);
	newRoad->addEndIntersection(crossing);
	crossing->attachRoad(newRoad);

	//Add new road into the structure, connect to existing crossing
	insertRoad(newRoad);
}

void StreetManager::connectToNewIntersection(RoadIntersection * start, Road * newRoad, Road * target) {
	assert(newRoad->getStart().getDistance(start->location) < 6.0f);

	//Add new road into structure
	newRoad->addStartIntersection(start);

	//Create new Intersection, attaches to road in constructor
	RoadIntersection *crossing = new RoadIntersection(Point(newRoad->getEnd()), newRoad, target);

	insertIntersection(crossing);
	insertRoad(newRoad);
}
void StreetManager::connectToRoad(RoadIntersection * start, Road * toAdd, Road * target) {
	RoadIntersection *intersection = getExistingIntersections(toAdd, target);

	if (intersection != nullptr)
		connectToExistingIntersection(start, toAdd, intersection);
	else	//No nearby crossings exist, so lets add a new one
		connectToNewIntersection(start, toAdd, target);
}

void StreetManager::insertRoad(Road * toAdd) {
	//Vector style
	//roads.push_back(toAdd);

	//Rtree
	roadTree.insert(std::make_pair(toBoostSegment(toAdd), toAdd));

	QGraphicsLineItem *road = scene->addLine(QLineF(toAdd->getStart(), toAdd->getEnd()), getRoadPen(toAdd));
	road->setZValue(toAdd->rType == roadType::MAINROAD ? 2.0f : 3.f);
	roadsRender->addToGroup(road);
}

void StreetManager::insertIntersection(RoadIntersection * toAdd) {
	//Vector style
	//intersections.push_back(toAdd);

	//Rtree
	intersectionTree.insert(std::make_pair(toBoostPoint(toAdd->location), toAdd));

	QGraphicsRectItem *vert = scene->addRect(toAdd->boundingRect());
	vert->setZValue(1.0f);
	intersectionsRender->addToGroup(vert);
	intersectionsRender->setZValue(1.0f);
}
