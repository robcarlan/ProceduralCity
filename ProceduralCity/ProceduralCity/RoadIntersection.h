#pragma once
#include "Road.h"
#include <list>
#include <qgraphicsitem.h>

class Road;

class RoadIntersection : public QGraphicsItem {
protected:
	QRectF boundingBox;

	void calculateBoundingBox();
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
	//Crossings are created to join to a pre existing road
	bool isCrossing;
	Road *parent; 
	Point location;
	std::list<Road*> connected;
	void setParent(Road *road);
	void setIncident(Road *road);
	//Ensure road connects, add to list
	void attachRoad(Road *road);
	//Returns true if no other intersection exists
	bool removeRoad(Road *road);
	QRectF boundingRect() const;
	static const float boxWidth;

	///Create a crossing from creator, and attach it onto the incident road.
	RoadIntersection(Point Position, Road *creator, Road *incidentOnto);
	///Create an intersection which isn't a crossing
	RoadIntersection(Point Position, Road *creator);
	RoadIntersection(Point Position);
	RoadIntersection(const RoadIntersection &road);
	RoadIntersection();
	~RoadIntersection();
};

