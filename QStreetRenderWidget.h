#pragma once
#include "CityView2D.h"
#include "Road.h"

#include <qgraphicsview.h>
#include <QtOpenGL>
#include <qopengltexture.h>
#include <qrect.h>
#include <qpainter.h>

class QStreetRenderWidget : public QFrame {
	Q_OBJECT

public:
	QSlider *zoomSlider;
	QGraphicsView *view;

	QStreetRenderWidget(QWidget * parent);
	QStreetRenderWidget();
	~QStreetRenderWidget();

	void initialiseRenderer(CityView2D *cityView);

private slots:
	//Controls handling scroll
	void onHorizontalScroll(int scrollPosition);
	void onVerticalScroll(int scrollPosition);
	void onScaleChange(int scrollPosition);

	void setDrawVertices(bool drawVertices);

protected:
	CityView2D *cityView;

	bool drawStreetVertices;

	void onViewChange();
	void setBackground(QImage *bg);
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
};

