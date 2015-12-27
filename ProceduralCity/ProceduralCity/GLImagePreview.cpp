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

QImage* GLImagePreview::getImage() {
	return img;
}

void GLImagePreview::paintEvent(QPaintEvent * e) {
	//No image to paint
	if (!imgSet) return;

	QPainter painter;
	painter.begin(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.drawImage(this->rect(),*img);
	painter.end();
}
