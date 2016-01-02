#include "GLImagePreview.h"

GLImagePreview::GLImagePreview(QWidget * parent) : QOpenGLWidget(parent) {
	imgSet = false;
}

GLImagePreview::GLImagePreview() {
	imgSet = false;
}


GLImagePreview::~GLImagePreview() {
	if (imgSet)
		delete img;
}

void GLImagePreview::setImage(QString filePath) {
	img = new QImage(filePath);
	imgSet = true;
	repaint();
}

void GLImagePreview::clear() {
	imgSet = false;
	update();
}

bool GLImagePreview::isSet() {
	return imgSet;
}

QImage* GLImagePreview::getImage() {
	return img;
}

void GLImagePreview::paintEvent(QPaintEvent * e) {
	//No image to paint


	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);

	if (!imgSet) 
		painter.fillRect(this->rect(), Qt::GlobalColor::black);
	else  
		painter.drawImage(this->rect(), *img);

	painter.end();
}
