#pragma once
#include <QGraphicsScene>
#include <QImage>
#include <QLine>

#include "Road.h"
#include "RoadIntersection.h"

#ifndef Q_MOC_RUN 
#include <boost\geometry\index\rtree.hpp>
#include <boost\foreach.hpp>
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

	QGraphicsItemGroup *intersectionsRender;
	QGraphicsItemGroup *roadsRender;

	//Just to make it work init
	std::vector<Road*> roads;
	std::vector<RoadIntersection*> intersections;
	bool renderVerts;

public:
	typedef std::pair<Point, Road*> intersectionRec;

	void setPop(QImage *pop);
	void setHeight(QImage *height);
	void setGeog(QImage *geog);
	void renderPop();
	void renderHeight();
	void renderGeog();
	void renderNone();
	void renderVertices(bool renderVerts);

	int roadCount();
	int vertCount();

	StreetManager();
	~StreetManager();
	QGraphicsScene *getScene();

	///Reset state
	void reset();

	//Returns all vertices within radius of queryPoint, returns in vector nearby
	void getNearbyVertices(RoadIntersection queryPoint, double radius, std::vector<RoadIntersection*>& nearby);

	//Returns all edges which collide with edge. Returns the colliding edges with their intersection, in vector nearby.
	void getIntersectingEdges(Road edge, std::vector<intersectionRec>& nearby);

	//Creates a road from an existing intersection, and creates a new intersection at the end point
	void branchRoad(RoadIntersection *start, Road *newRoad);
	//Creates a road from an existing intersection, and extends the road to an intersection
	void connectToExistingIntersection(RoadIntersection *start, Road *newRoad, RoadIntersection *crossing);
	void connectToNewIntersection(RoadIntersection *start, Road *newRoad, Road *target);

	void insertRoad(Road *toAdd);
	void insertIntersection(RoadIntersection *toAdd);
};

