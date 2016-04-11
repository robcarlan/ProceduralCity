#ifndef CITYVIEW3D_H
#define CITYVIEW3D_H

#include <QtGui/QGuiApplication>
#include <QtGui/QMatrix4x4>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLBuffer>
#include <qopenglfunctions.h>
#include <QtGui/QScreen>
#include <QtGui/QWindow>
#include <QtGui/QOpenGLFunctions>
#include <QtGui/qopenglcontext.h>
#include <QtGui/qopenglfunctions_4_3_core.h>
#include <QtGui/qopenglfunctions_4_3_compatibility.h>
#include <QtGui>
#include <QMouseEvent>
#include <QtCore/qmath.h>

#include "City.h"
#include "BuildingRegion.h"
#include "BuildingLot.h"

namespace Ui {
class CityView3D;
}

class CityView3D : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    explicit CityView3D(QWindow *parent = 0);
    ~CityView3D();
	virtual void render();
	virtual void render(QPainter* painter);

	virtual void initialize();

	void setData(City *city);
	void setAnimating(bool animating);
	void initaliseCamera();

public slots:
	void renderLater();
	void renderNow();

protected:

	//Render
	//Draws the base heightmap
	void renderTerrain();
	void renderBuildings();
	void renderRoads();

	//Update
	void updateBuildingInstances();
	void updateCamera();

	//Input
	bool event(QEvent *event) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
	void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
	//------------------- Model Variables -----------------------
	City *city;
	bool hasCity;

	//------------------- Render Variables -----------------------
	bool m_update_pending;
	bool m_animating;
	bool initialised;

	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;
	QOpenGLFunctions_4_3_Core *funcs;
	QSurfaceFormat format;

	GLuint loadShader(GLenum type, const char *source);

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	QOpenGLShaderProgram *m_InstancedBuildingProg;
	QOpenGLShaderProgram *m_hMapRenderProg;
	QOpenGLShaderProgram *m_roadMapRenderProg;
	int m_frame;

	int numCubeRender;
	int numTerrainQuadRender;
	int numCubeRenderMax;
	int numTerrainQuadRenderMax;
	GLfloat *cube_positions; //Array of 3D vectors
	GLfloat *cube_scales; //Array of 3D vectors

	GLuint cubeVertBuffer; 
	GLuint cubePosBuffer; //To be filled by cube_positions
	GLuint cubeScaleBuffer; //To be filled by cube_scales

	GLuint cubeInstanceCamUpID;
	GLuint cubeInstanceCamRightID;
	GLuint cubeInstanceViewProjMatID;

	//Camera Variables 
	//Is it worth having a camera class? (Handles visibility etc)
	QMatrix4x4 projection;
	QMatrix4x4 viewMatrix;
	QMatrix4x4 VP;
	QVector3D forward;
	QVector3D up;
	QVector3D eye;
	QVector3D right, dir;
	QVector3D position;
	QVector3D rotation;
	GLfloat moveSpeed;

	//Mouse Controls
	QPointF prev;
	QPointF dif;

	//Keyboard controls
	static const int moveLeftKey;
	static const int moveRightKey;
	static const int moveForwardKey;
	static const int moveBackKey;
	static const int moveUpKey;
	static const int moveDownKey;
	static const int speedUpKey;
	static const int speedDownKey;
	bool isLeftHeld, isRightHeld, isBackHeld, isForwardHeld;

	static const float defaultMoveSpeed, fastMoveSpeed, slowMoveSpeed;
	static const float rotSpeed;

	std::string readFile(char* fname);
};

#endif // CITYVIEW3D_H
