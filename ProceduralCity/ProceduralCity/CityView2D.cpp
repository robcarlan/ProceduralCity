#include "CityView2D.h"

void CityView2D::addRegions(std::vector<BuildingRegion> *regions) {
	BOOST_FOREACH(BuildingRegion region, *regions) {
		QPolygonF poly = QPolygonF();
		BOOST_FOREACH(Point p, region.getPoints()) {
			poly.append(p);
		}

		//TODO
		QGraphicsItem* polyItem;

		if (region.isValid()) {
			polyItem = scene->addPolygon(poly, QPen(QBrush(QColor(100, 0, 0)), 2.0f), QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
			regionRender->addToGroup(polyItem);
		}
		else {
			//polyItem = scene->addPolygon(poly, QPen(QBrush(QColor(100, 0, 0)), 2.0f), QBrush(QColor(0,0,0)));
			//regionRender->addToGroup(polyItem);
		}

	}
}

void CityView2D::addLots(std::vector<BuildingLot*> &lots) {
	BOOST_FOREACH(BuildingLot *lot, lots) {
		QPolygonF poly = QPolygonF();
		BOOST_FOREACH(Point p, lot->getPoints()) {
			poly.append(p);
		}

		QGraphicsItem* polyItem;

		//if (lot->getOwner()->isConvex()) {
		//	polyItem = scene->addPolygon(poly, QPen(QBrush(QColor(0, 0, 0)), 0.5f), QBrush(QColor(rand() % 256, rand() % 256, rand() % 256)));
		//}
		//else {
			polyItem = scene->addPolygon(poly, QPen(QBrush(QColor(0, 0, 0)), 0.5f), QBrush(QColor(255, 255, 255)));
		//}

		lotsRender->addToGroup(polyItem);
	}
}

QPen CityView2D::getRoadPen(Road * road) {
	if (road->rType == roadType::STREET) return roadPen;
	else return mainRoadPen;
}

QPen CityView2D::getRoadPen(roadPtr road) {
	if (road->rtype == roadType::STREET) return roadPen;
	else return mainRoadPen;
}

void CityView2D::setPop(QImage *pop) {
	this->pop = *pop;
	//Create the pixmap now for the scene
	pixPop.convertFromImage(*pop);
}

void CityView2D::setHeight(QImage *height) {
	this->height = *height;
	//Create the pixmap now for the scene
	pixHeight.convertFromImage(*height);
}

void CityView2D::setGeog(QImage *geog) {
	this->geog = *geog;
	//Create the pixmap now for the scene
	pixGeog.convertFromImage(*geog);
}

void CityView2D::setPattern(QImage * pattern) {
	this->pattern = *pattern;
	//Create the pixmap now for the scene
	pixPattern.convertFromImage(*pattern);
}

void CityView2D::renderPop() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(pixPop);
	bg->setZValue(-1.0f);
}

void CityView2D::renderHeight() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(pixHeight);
	bg->setZValue(-1.0f);
}

void CityView2D::renderGeog() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(pixGeog);
	bg->setZValue(-1.0f);
}

void CityView2D::renderPattern() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = scene->addPixmap(pixPattern);
	bg->setZValue(-1.0f);
}

void CityView2D::renderNone() {
	if (bg != nullptr)
		scene->removeItem(bg);
	bg = nullptr;
}

void CityView2D::renderVertices(bool renderVerts) {
	this->renderVerts = renderVerts;
	intersectionsRender->setVisible(renderVerts);
}

void CityView2D::setDrawLots(bool drawLots) {
	this->drawLots = drawLots;
	lotsRender->setVisible(drawLots);
}

void CityView2D::setDrawRegions(bool drawRegions) {
	this->drawRegions = drawRegions;
	regionRender->setVisible(drawRegions);
}

QGraphicsScene * CityView2D::getScene() {
	return scene;
}

void CityView2D::Reset(Point size) {
	scene->clear();
	bg = nullptr;

	QList<QGraphicsItem*> emptyRoads = QList<QGraphicsItem*>();
	QList<QGraphicsItem*> emptyIntersections = QList<QGraphicsItem*>();
	intersectionsRender = scene->createItemGroup(emptyIntersections);
	intersectionsRender->setVisible(renderVerts);
	roadsRender = scene->createItemGroup(emptyRoads);
	lotsRender = scene->createItemGroup(QList<QGraphicsItem*>());
	lotsRender->setVisible(drawLots);
	regionRender = scene->createItemGroup(QList<QGraphicsItem*>());
	regionRender->setVisible(drawRegions);

	scene->setSceneRect(QRectF(-100, -100, size.x() + 100, size.y() + 100));
	scene->setSortCacheEnabled(true);
}

void CityView2D::clearGeometry() {
	//Destroy groups
	//scene->destroyItemGroup(intersectionsRender);
	//scene->destroyItemGroup(roadsRender);
	//scene->destroyItemGroup(lotsRender);
	//scene->destroyItemGroup(regionRender);
	scene->clear();

	//Recreate groups, set visibility
	intersectionsRender = scene->createItemGroup(QList<QGraphicsItem*>());
	intersectionsRender->setVisible(renderVerts);
	regionRender = scene->createItemGroup(QList<QGraphicsItem*>());
	regionRender->setVisible(drawRegions);
	lotsRender = scene->createItemGroup(QList<QGraphicsItem*>());
	lotsRender->setVisible(drawLots);
	roadsRender = scene->createItemGroup(QList<QGraphicsItem*>());
	roadsRender->setVisible(true);
}

void CityView2D::Update() {
	scene->update();
}

void CityView2D::addRoad(Road * toAdd) {
	QGraphicsLineItem *road = scene->addLine(QLineF(toAdd->getStart(), toAdd->getEnd()), getRoadPen(toAdd));
	road->setZValue(toAdd->rType == roadType::MAINROAD ? 2.0f : 3.f);
	roadsRender->addToGroup(road);
}

void CityView2D::addIntersection(RoadIntersection * toAdd) {
	QGraphicsRectItem *vert = scene->addRect(toAdd->boundingRect());
	vert->setZValue(1.0f);
	intersectionsRender->addToGroup(vert);
	intersectionsRender->setZValue(1.0f);
}

void CityView2D::addRoads(std::list<roadPtr> roads) {
	BOOST_FOREACH(roadPtr roadItr, roads) {
		QGraphicsLineItem *road = scene->addLine(QLineF(roadItr->getStart()->getIntersectionPoint(), roadItr->getEnd()->getIntersectionPoint()), getRoadPen(roadItr));
		road->setZValue(roadItr->rtype == roadType::MAINROAD ? 2.0f : 3.f);
		roadsRender->addToGroup(road);
	}
}

void CityView2D::addIntersections(std::list<intersectionPtr> intersections) {
	BOOST_FOREACH(intersectionPtr intersectionItr, intersections) {
		float x = intersectionItr->getIntersectionPoint().x();
		float y = intersectionItr->getIntersectionPoint().y();
		QGraphicsRectItem *vert = scene->addRect(x - 5.0f, y - 5.0f, 10.0f, 10.0f);
		vert->setZValue(1.0f);
		intersectionsRender->addToGroup(vert);
		intersectionsRender->setZValue(1.0f);
	}
}

CityView2D::CityView2D() {
	scene = new QGraphicsScene();

	scene->setBackgroundBrush(QBrush(QColor::fromRgb(100, 100, 100)));
	//Create pens
	roadPen = QPen(QBrush(QColor::fromRgb(50, 50, 50)), 2.0f);
	mainRoadPen = QPen(QBrush(QColor::fromRgb(0, 0, 0)), 4.0f);
}

CityView2D::~CityView2D() {
}
