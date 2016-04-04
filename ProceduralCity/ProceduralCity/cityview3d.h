#ifndef CITYVIEW3D_H
#define CITYVIEW3D_H

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QMouseEvent>
#include <QtCore/qmath.h>

namespace Ui {
class CityView3D;
}

class CityView3D : public QWindow, public QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CityView3D(QWindow *parent = 0);
    ~CityView3D();
	virtual void render();
	virtual void render(QPainter* painter);

	virtual void initialize();

	void setAnimating(bool animating);

public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
	bool m_update_pending;
	bool m_animating;
	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;

	//Triangle
	GLuint loadShader(GLenum type, const char *source);

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_program;
	int m_frame;

	//Mouse Controls
	QPointF prev;
	QPointF dif;

};

#endif // CITYVIEW3D_H
