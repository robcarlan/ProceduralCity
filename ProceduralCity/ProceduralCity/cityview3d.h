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
	void initaliseCamera();

public slots:
	void renderLater();
	void renderNow();

protected:
	bool event(QEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
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

	//Camera Variables 
	//Is it worth having a camera class? (Handles visibility etc)
	float posX, posY, posZ;
	float xRot, yRot, zRot;

	//Mouse Controls
	QPointF prev;
	QPointF dif;

	//Keyboard controls
	static const int moveLeftKey;
	static const int moveRightKey;
	static const int moveForwardKey;
	static const int moveBackKey;
	bool isLeftHeld, isRightHeld, isBackHeld, isForwardHeld;

};

#endif // CITYVIEW3D_H
