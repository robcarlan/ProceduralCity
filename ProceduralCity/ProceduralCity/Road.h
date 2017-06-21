#pragma once

#include "Point.h"
#include "RoadIntersection.h"

#include <vector>
#include <qpainter.h>
#include <qline.h>
#include <qgraphicsitem.h>

#include "Enums.h"

//typedef boost::geometry::model::d2::point_xy<float> Point;
class RoadIntersection;

///Abstract class defining a simple road. 
class Road : public QGraphicsItem, public QLineF {
public:
	//Constructs road, attaches to intersection.
	Road(Point start, Point end, RoadIntersection* intersection, Road *parent = nullptr, roadType rType = roadType::STREET);
	Road(Point start, Point end, Road *parent = nullptr, roadType rType = roadType::STREET);
	Road(const Road &road);
	~Road();

	std::vector<RoadIntersection*> intersections;
	RoadIntersection* roadStartIntersection;
	RoadIntersection* roadEndIntersection;

	Point getStart() const;
	Point getEnd() const;
	float getAngle() const;
	float getDot(Road &other) const;
	bool isInBounds(int sizeX, int sizeY) const;

	QPointF lerp(float amount);
	void setStart(Point start);
	void setEnd(Point end);

	static const float PEN_WIDTH;
	QRectF bounds;
	roadType rType;
	Road *parent;

	bool isBranch;
	bool isBridge;

	std::string printRoad();

	//std::vector<Road, Point> connected;
	std::vector<RoadIntersection*>* getIntersections();
	//Remove an intersection from this. i.e. when a road is deleted.
	void deleteIntersection(RoadIntersection* toDelete);
	//Add an intersection incident on to this road
	void addIntersection(RoadIntersection* toCreate);
	void addStartIntersection(RoadIntersection* start);
	void addEndIntersection(RoadIntersection* end);

	bool operator== (const Road compare);
	QRectF boundingRect() const;
	void setIsBranch(bool isBranch);
	void setIsBridge(bool isBridge);

protected:

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundsFromPointPair(Point s, Point e);
	Point start, end;
	float angle;
	void calcAngle();

	//TODO :: Main / Minor Roads
	//TODO :: Roads to be of various types, i.e. Normal roads, Overground Roads (which ignore clipping over roads), Underground, Bridges ...
};

///Road defined by two points. The road is a straight line between them.
class StraightRoad
	: public Road {
public:
	StraightRoad(Point start, Point end);

	std::string printRoad();
	//Point start, end;
};

//Circular / Splines
