#pragma once

#include "Point.h"
#include "RoadIntersection.h"

#include <vector>
#include <qpainter.h>
#include <qline.h>
#include <qgraphicsitem.h>

//typedef boost::geometry::model::d2::point_xy<float> Point;
class RoadIntersection;

enum class geogType {
	LAND,
	WATER,
	PARK
};

enum class roadType {
	STREET,
	MAINROAD
};

enum class roadPattern {
	NOPATTERN,
	MANHATTAN,
	RADIAL,
	SANFRAN
};

///Abstract class defining a simple road. 
class Road : public QGraphicsItem, public QLineF {
public:
	Road(Point start, Point end, roadType rType = roadType::STREET);
	Road(const Road &road);
	~Road();

	std::vector<RoadIntersection*> intersections;
	Point getStart() const;
	Point getEnd() const;
	QPointF lerp(float amount);
	void setStart(Point start);
	void setEnd(Point end);

	static const float PEN_WIDTH;
	QRectF bounds;
	roadType rType;

	std::string printRoad();

	//std::vector<Road, Point> connected;
	std::vector<RoadIntersection*>* getIntersections();
	//Remove an intersection from this. i.e. when a road is deleted.
	void deleteIntersection(RoadIntersection* toDelete);
	//Add an intersection incident on to this road
	void addIntersection(RoadIntersection* toCreate);

	bool operator== (const Road compare);
	QRectF boundingRect() const;

protected:

	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QRectF boundsFromPointPair(Point s, Point e);
	Point start, end;

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
