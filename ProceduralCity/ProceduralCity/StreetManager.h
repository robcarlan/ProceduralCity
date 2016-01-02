#pragma once
#include <QGraphicsScene>
#include <QImage>
#include "Road.h"

#ifndef Q_MOC_RUN 
#include <boost\geometry\index\rtree.hpp>
#endif
///Used by StreetGenerator to handle storing roads and crossings. Allows methods to add / remove / connect these items, as well as spatial queries.
class StreetManager {
private:
	//Internally managed by an rtree
	//boost::geometry::index::rtree<
	//Manage connected roads

	QGraphicsScene *scene;
	QImage pop, height, geog;
	QPixmap *pixPop, *pixHeight, *pixGeog;
	QGraphicsItem *bg;

public:
	void setPop(QImage *pop);
	void setHeight(QImage *height);
	void setGeog(QImage *geog);
	void renderPop();
	void renderHeight();
	void renderGeog();
	void renderNone();

	StreetManager();
	~StreetManager();
	QGraphicsScene *getScene();

	///Reset state
	void clearRoads();

	void addRoad(Road *toAdd);
};

