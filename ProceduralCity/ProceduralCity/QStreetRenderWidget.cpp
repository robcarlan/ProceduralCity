#include "QStreetRenderWidget.h"



QStreetRenderWidget::QStreetRenderWidget(QFrame * parent) : QOpenGLWidget(parent) {
}

QStreetRenderWidget::QStreetRenderWidget() {

}

QStreetRenderWidget::~QStreetRenderWidget() {
}

void QStreetRenderWidget::initialiseRenderer(StreetGen * generator) {
	this->generator = generator;
	streetRenderWidth = 3;
}

void QStreetRenderWidget::onHorizontalScroll(int scrollPosition) {
}

void QStreetRenderWidget::onVerticalScroll(int scrollPosition) {
}

void QStreetRenderWidget::onScaleChange(int scrollPosition) {
	//Recalculate road pen width. + Box width
}

void QStreetRenderWidget::setDrawVertices(bool drawVertices) {
	drawStreetVertices = drawVertices;
}

//OpenGL Rendering

void QStreetRenderWidget::initializeGL() {
	//Initially, let the background image be nothing
	background = 0;

	roadPen = QPen(roadColour);
	roadPen.setWidth((int)streetRenderWidth);
	roadPen.setCapStyle(Qt::PenCapStyle::RoundCap);
}

void QStreetRenderWidget::paintEvent(QPaintEvent * e) {
	QPainter painter;

	//painter.beginNativePainting();
	//QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
	//f->glClearColor(0.0f, 0.0f, 0.5f, 1.0f);
	//painter.endNativePainting();
	
	painter.begin(this);
	painter.fillRect(this->rect(), QBrush(Qt::blue));

	//Draw Underlying maps, if set
	if (background != 0) {
		
	}

	//Draw out of bounds area as a seperate background colour

	//Draw street geometry, road edges first
	paintRoads(painter);

	//Draw a box for each street vertex
	if (drawStreetVertices) {
		//paintVertices(painter);
	}

	painter.end();
}

void QStreetRenderWidget::resizeGL(int width, int height) {
}

void QStreetRenderWidget::paintRoads(QPainter &painter) {
	//Eventually we can replace this with a query to get Roads which intersect with Rectangle(toWorldSpace(viewingRect))
	std::vector<Road> &toDraw = *generator->getGenerated();
	painter.setPen(roadPen);

	for (std::vector<Road>::iterator it = toDraw.begin(); it != toDraw.end(); it++) {
		painter.drawLine(QLine(it->getStart().getX(), it->getStart().getY(),
			it->getEnd().getX(), it->getEnd().getY()));
	}

}

void QStreetRenderWidget::paintVertices(QPainter &painter) {
}
