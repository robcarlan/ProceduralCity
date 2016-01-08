#pragma once
#include <QGraphicsScene>
#include <QImage>
#include <QLine>

#include "Road.h"
#include "RoadIntersection.h"

#ifndef Q_MOC_RUN 
#include <boost\geometry\index\rtree.hpp>
#include <boost\geometry.hpp>
#include <boost\foreach.hpp>
#include <boost\function_output_iterator.hpp>
#include <boost\geometry\geometries\point_xy.hpp>
#endif

using namespace boost::geometry;

///Used by StreetGenerator to handle storing roads and crossings. Allows methods to add / remove / connect these items, as well as spatial queries.
class StreetManager {
private:
	//Manage connected roads

	QGraphicsScene *scene;
	QImage pop, height, geog;
	QPixmap *pixPop, *pixHeight, *pixGeog;
	QGraphicsItem *bg;

	QGraphicsItemGroup *intersectionsRender;
	QGraphicsItemGroup *roadsRender;

	typedef boost::geometry::model::d2::point_xy<float> BOOST_POINT;
	typedef boost::geometry::model::segment<BOOST_POINT> BOOST_SEGMENT;
	typedef std::pair<BOOST_POINT, RoadIntersection*> intersectionIndex;
	typedef std::pair<BOOST_SEGMENT, Road*> roadIndex;

	BOOST_POINT toBoostPoint(Point val);
	BOOST_SEGMENT toBoostSegment(Road *val);

	//Rtree<Point>
	boost::geometry::index::rtree<intersectionIndex, index::linear<16>> intersectionTree;
	//Rtree<Segment>
	boost::geometry::index::rtree<roadIndex, index::linear<16>> roadTree;

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
	void getNearbyVertices(Point queryPoint, double radius, std::vector<RoadIntersection*>& nearby);
	intersectionRec* getClosest(Point queryPoint, std::vector<intersectionRec>& nearby);

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

