#pragma once
#include <list>

#include "Road.h"
#include "RoadIntersection.h"
#include "BuildingRegion.h"
#include "CityView2D.h"

#ifndef Q_MOC_RUN 
#include <boost/geometry/index/rtree.hpp>
#include <boost/geometry.hpp>
#include <boost/foreach.hpp>
#include <boost/function_output_iterator.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#endif

using namespace boost::geometry;

///Used by StreetGenerator to handle storing roads and crossings. Allows methods to add / remove / connect these items, as well as spatial queries.
class StreetManager {
private:

	static const float nearbyIntersectionRange;
	static const int numSubdivide;
	static const float angleCurveThreshold;
	//Manage connected roads

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
	std::list<Road*> roadList;
	std::list<RoadIntersection*> intersectionsList;

	CityView2D *view;
	Point bounds;

public:
	typedef std::pair<Point, Road*> intersectionRec;

	int roadCount();
	int vertCount();

	StreetManager(Point bounds, CityView2D *view);
	StreetManager();
	~StreetManager();

	///Reset state
	void reset();
	void setView(CityView2D *view);

	//Returns all vertices within radius of queryPoint, returns in vector nearby
	void getNearbyVertices(Point queryPoint, double radius, std::vector<RoadIntersection*>& nearby);
	intersectionRec* getClosest(Point queryPoint, std::vector<intersectionRec>& nearby);

	//Returns all edges which collide with edge. Returns the colliding edges with their intersection, in vector nearby.
	void getIntersectingEdges(Road edge, std::vector<intersectionRec>& nearby);

	//Returns the intersection which this road attaches to, if any. Else returns nullptr
	RoadIntersection *getExistingIntersections(Road * query, Road * target);
	//Returns true if this road should be split into multiple new ones.
	bool shouldSubdivide(Road* road);
	//To be called instead of the usual insertRoad procedure
	void subdivideRoad(Road* road);

	//Creates a road from an existing intersection, and creates a new intersection at the end point
	void branchRoad(RoadIntersection *start, Road *newRoad);
	//Creates a road from an existing intersection, and extends the road to an intersection
	void connectToExistingIntersection(RoadIntersection *start, Road *newRoad, RoadIntersection *crossing);
	void connectToNewIntersection(RoadIntersection *start, Road *newRoad, Road *target);
	void connectToRoad(RoadIntersection * start, Road *toAdd, Road *target);

	void insertRoad(Road *toAdd);
	void insertIntersection(RoadIntersection *toAdd);

	std::list<Road*> getRoads();
	std::list<RoadIntersection*> getIntersections();
};

