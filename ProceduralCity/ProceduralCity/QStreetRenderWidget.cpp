#include "QStreetRenderWidget.h"
#include "streetgenerator.h"

QStreetRenderWidget::QStreetRenderWidget(QWidget * parent) : QFrame(parent) {

	//Initialise Graphics View
	view = new QGraphicsView(this);
	view->setRenderHint(QPainter::Antialiasing);
	view->setDragMode(QGraphicsView::DragMode::ScrollHandDrag);
	view->setOptimizationFlags(QGraphicsView::DontSavePainterState);
	view->setViewportUpdateMode(QGraphicsView::SmartViewportUpdate);
	view->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
	view->fitInView(0, 0, 2048, 2048);

	//Initialise Slider
	zoomSlider = new QSlider;
	zoomSlider->setMinimum(0);
	zoomSlider->setMaximum(500);
	zoomSlider->setValue(250);
	zoomSlider->setTickPosition(QSlider::TicksRight);

	QHBoxLayout *layout = new QHBoxLayout();
	layout->addWidget(view);
	layout->addWidget(zoomSlider);
	setLayout(layout);

	//Connect slots to signals
	connect(zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(onScaleChange(int)));

	onViewChange();
}

QStreetRenderWidget::QStreetRenderWidget() {
}

QStreetRenderWidget::~QStreetRenderWidget() {
}

void QStreetRenderWidget::initialiseRenderer(StreetGen * generator) {
	this->generator = generator;
	
	view->setScene(generator->getScene());
}


///Assume between 1 and 2048 for now
void QStreetRenderWidget::onHorizontalScroll(int scrollPosition) {
}

void QStreetRenderWidget::onVerticalScroll(int scrollPosition) {
}

///Assume max scroll is 100
void QStreetRenderWidget::onScaleChange(int scrollPosition) {
	onViewChange();
}

///Calculates the bounds in world space of the viewing rectangle
void QStreetRenderWidget::onViewChange() {
	qreal scale = qPow(qreal(2), (zoomSlider->value() - 250) / qreal(50));

	QMatrix matrix;
	matrix.scale(scale, scale);

	//Could possibly include rotation
	view->setMatrix(matrix);
}

void QStreetRenderWidget::setDrawVertices(bool drawVertices) {
	drawStreetVertices = drawVertices;
	generator->streets.renderVertices(drawStreetVertices);
}

//OpenGL Rendering

void QStreetRenderWidget::initializeGL() {
}

void QStreetRenderWidget::paintEvent(QPaintEvent * e) {

}

void QStreetRenderWidget::resizeGL(int width, int height) {
}

void QStreetRenderWidget::paintRoads(QPainter &painter) {
	////Eventually we can replace this with a query to get Roads which intersect with Rectangle(toWorldSpace(viewingRect))
	//std::vector<Road> &toDraw = *generator->getGenerated();
	//painter.setPen(roadPen);

	//for (std::vector<Road>::iterator it = toDraw.begin(); it != toDraw.end(); it++) {
	//	paintRoad(painter, it._Ptr);
	//}

}

void QStreetRenderWidget::paintVertices(QPainter &painter) {
}

void QStreetRenderWidget::paintRoad(QPainter &painter, Road *toDraw) {
	//float lspX1 = toDraw->getStart().getX() - bounds.x();
	//float lspY1 = toDraw->getStart().getY() - bounds.y();
	//float lspX2 = toDraw->getEnd().getX() - bounds.x();
	//float lspY2 = toDraw->getEnd().getY() - bounds.y();
	//painter.drawLine(QLine(lspX1 * scaleFactor, lspY1 * scaleFactor,
	//	lspX2 * scaleFactor, lspY2 * scaleFactor));
}

