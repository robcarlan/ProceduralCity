#pragma once
#include <qwidget.h>
#include <qopenglwidget.h>
#include <qopengltexture.h>
#include <qstring.h>
#include <qpainter.h>

class GLImagePreview : public QOpenGLWidget {
	Q_OBJECT

public:
	GLImagePreview(QWidget *parent);
	GLImagePreview();
	~GLImagePreview();

	QImage* getImage();
	void setImage(QString filePath);
	void clear();
	bool isSet();

	void paintEvent(QPaintEvent * e);

protected:

	bool imgSet;
	QImage *img;
};

