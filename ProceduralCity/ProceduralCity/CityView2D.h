#pragma once
#include <QGraphicsScene>
#include <QImage>
#include <QLine>

#include "Road.h"
#include "RoadIntersection.h"
#include "BuildingRegion.h"
#include "BuildingLot.h"
#include "Point.h"

#ifndef Q_MOC_RUN 
#include <boost\foreach.hpp>
#endif

class CityView2D {
	QGraphicsScene *scene;
	QImage pop, height, geog, pattern;
	QPixmap pixPop, pixHeight, pixGeog, pixPattern;
	QGraphicsItem *bg;
	QPen roadPen;
	QPen mainRoadPen;

	QGraphicsItemGroup *intersectionsRender;
	QGraphicsItemGroup *roadsRender;
	QGraphicsItemGroup *lotsRender, *regionRender;

	bool renderVerts, drawLots, drawRegions;
	QPen getRoadPen(Road *road);
	Point size;

public:
	void setPop(QImage *pop);
	void setHeight(QImage *height);
	void setGeog(QImage *geog);
	void setPattern(QImage *pattern);
	void renderPop();
	void renderHeight();
	void renderGeog();
	void renderPattern();
	void renderNone();

	void renderVertices(bool renderVerts);
	void setDrawLots(bool drawLots);
	void setDrawRegions(bool drawRegions);

	QGraphicsScene *getScene();
	void Reset(Point Size);
	void Update();

	void addRegions(std::vector<BuildingRegion> *regions);
	void addLots(std::vector<BuildingLot> *lots);
	void addRoad(Road * toAdd);
	void addIntersection(RoadIntersection * toAdd);

	CityView2D();
	~CityView2D();
};

