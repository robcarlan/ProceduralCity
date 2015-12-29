#pragma once
#include "StreetGen.h"
#include "Road.h"

#include <qopenglwidget.h>
#include <QtOpenGL>
#include <qopengltexture.h>
#include <qrect.h>
#include <qpainter.h>

class QStreetRenderWidget : public QOpenGLWidget {
	Q_OBJECT

public:
	QStreetRenderWidget(QFrame * parent);
	QStreetRenderWidget();
	~QStreetRenderWidget();

	void initialiseRenderer(StreetGen *generator);

private slots:
	//Controls handling scroll
	void onHorizontalScroll(int scrollPosition);
	void onVerticalScroll(int scrollPosition);
	void onScaleChange(int scrollPosition);

	void setDrawVertices(bool drawVertices);

protected:
	
	//Local Space 
	QRectF bounds;
	static const float SCALE_EXPONENT;
	float scaleFactor;
	float inverseScale;
	QPointF center;
	QPointF size;

	QOpenGLTexture *background;
	StreetGen *generator;

	float streetRenderWidth;
	bool drawStreetVertices;

	QPen roadPen;
	QColor roadColour;

	void calculateViewRectangle();

	///Called on start up, initialise variables
	void initializeGL();

	///Called when we need to rerender the scene
	void paintEvent(QPaintEvent * e);

	///Shouldn't be called
	void resizeGL(int width, int height);

	void paintRoads(QPainter &painter);
	void paintVertices(QPainter &painter);

	//Specific drawing
	void paintRoad(QPainter &painter, Road *toDraw);
	QPointF toScreenSpace(QPointF worldPoint);
};

