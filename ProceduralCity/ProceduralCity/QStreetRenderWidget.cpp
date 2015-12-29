#include "QStreetRenderWidget.h"
#include "streetgenerator.h"

const float QStreetRenderWidget::SCALE_EXPONENT = 1.2f;

QStreetRenderWidget::QStreetRenderWidget(QFrame * parent) : QOpenGLWidget(parent) {
}

QStreetRenderWidget::QStreetRenderWidget() {
}

QStreetRenderWidget::~QStreetRenderWidget() {
}

void QStreetRenderWidget::initialiseRenderer(StreetGen * generator) {
	this->generator = generator;
	streetRenderWidth = 3;

	//Initialise Rect
	scaleFactor = 0.1f;
	inverseScale = 1.0f / scaleFactor;
	size = QPointF(2048.0f, 2048.0f);
	bounds.setRect(-100, -100, 2248, 2248);
}


///Assume between 1 and 2048 for now
void QStreetRenderWidget::onHorizontalScroll(int scrollPosition) {
	center.setX(size.x() * (scrollPosition / 2048.0f));
	calculateViewRectangle();
	repaint();
}

void QStreetRenderWidget::onVerticalScroll(int scrollPosition) {
	center.setY(size.y() * (scrollPosition / 2048.0f));
	calculateViewRectangle();
	repaint();
}

///Assume max scroll is 100
void QStreetRenderWidget::onScaleChange(int scrollPosition) {
	//Recalculate road pen width. + Box width

	//scale is exponential to scroll position 
	scaleFactor = 0.1f * std::powf(SCALE_EXPONENT, scrollPosition);
	inverseScale = 1.0f / scaleFactor;
	calculateViewRectangle();
	repaint();
}

///Calculates the bounds in world space of the viewing rectangle
void QStreetRenderWidget::calculateViewRectangle() {
	float wspLeft = center.x() - scaleFactor * this->width() / 2.0f;
	float wspTop = center.y() + scaleFactor * this->height() / 2.0f;;
	bounds.setRect(wspLeft, wspTop,
		this->width() * scaleFactor, this->height() * scaleFactor);
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

	//Draw rectangle to represent canvas
	QRectF canvas = QRectF(
		//QPointF(- bounds.left() * scaleFactor, bounds.top() * scaleFactor), 
		toScreenSpace(QPointF(0,0)), toScreenSpace(size)
		//QPointF( (size.getX() - bounds.left()) * scaleFactor, (size.getY() - bounds.top()) * scaleFactor)
			);

	painter.fillRect(canvas, QBrush(Qt::white));

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
		paintRoad(painter, it._Ptr);
	}

}

void QStreetRenderWidget::paintVertices(QPainter &painter) {
}

void QStreetRenderWidget::paintRoad(QPainter &painter, Road *toDraw) {
	float lspX1 = toDraw->getStart().getX() - bounds.x();
	float lspY1 = toDraw->getStart().getY() - bounds.y();
	float lspX2 = toDraw->getEnd().getX() - bounds.x();
	float lspY2 = toDraw->getEnd().getY() - bounds.y();
	painter.drawLine(QLine(lspX1 * scaleFactor, lspY1 * scaleFactor,
		lspX2 * scaleFactor, lspY2 * scaleFactor));
}

///Transforms a world space point, i.e. road coordinate into screen space.
QPointF QStreetRenderWidget::toScreenSpace(QPointF worldPoint) {
	return QPointF((worldPoint.x() - bounds.x()) * scaleFactor,
		(worldPoint.y() - bounds.y()) * scaleFactor);
}
