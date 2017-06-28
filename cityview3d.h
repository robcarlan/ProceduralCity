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
#include <QtOpenGLExtensions/QtOpenGLExtensions>
#include <QGLFormat>

#include "City.h"
#include "BuildingRegion.h"
#include "BuildingLot.h"
#include "Building.h"
#include "Shapes/Cube.h"

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

	void setData(City *city, QImage* height, QImage* geog,
		float minHeight, float heightScale);
	void setRoadData(float streetWidth, float mainroadWidth);
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
	void updateHMapInstances();
	void updateRoadInstances();
	void updateBuildingInstances();
	void updateCamera();
	float getHeightSample(int x, int y);
	float getHeightValue(float heightFactor);

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

	float minHeight;
	float heightScale;
	float streetWidth, mainroadWidth;

	QImage geogMap;
	QImage heightMap;

	//------------------- Render Variables -----------------------
	bool m_update_pending;
	bool m_animating;
	bool initialised;

	bool roadsNeedUpdateVertexData;
	bool needUpdateRoads;
	bool buildingsNeedUpdateVertexData;
	bool needUpdateBuildings;
	bool hMapNeedsUpdate;
	bool hMapReady;
	float hMapDimension;
	int hMapGridCount;

	QOpenGLContext *m_context;
	QOpenGLPaintDevice *m_device;
	QOpenGLFunctions_4_3_Core *funcs;
	QSurfaceFormat format;

	GLuint loadShader(GLenum type, const char *source);

	GLuint m_posAttr;
	GLuint m_colAttr;
	GLuint m_matrixUniform;

	std::string roadVertexShader;
	std::string hMapVertexShaderSource;
	std::string vertexShaderSource;
	std::string texturedQuadVertShader;
	std::string fragmentShaderSource;
	std::string fragmentShaderSourceHMap;

	QOpenGLShaderProgram *m_InstancedBuildingProg;
	QOpenGLShaderProgram *m_hMapRenderProg;
	QOpenGLShaderProgram *m_roadRenderProg;
	QOpenGLShaderProgram *m_TexturedQuadProg;
	int m_frame;

	int numCubeRender;
	int numTerrainQuadRender;
	int numRoadPieceRender;
	int numCubeRenderMax;
	int numTerrainQuadRenderMax;
	int numRoadPieceRenderMax;
	int renderDistance;

	GLuint quadVertBuffer; //Vertices to form a quad

	//Building Cubes
	GLfloat *cube_positions; //Array of 4D vectors, 4th element is rot around y
	GLfloat *cube_scales; //Array of 3D vectors

	//Building Cubes
	GLuint cubeVertBuffer;
	GLuint cubeNormBuffer;
	GLuint cubePosBuffer; //To be filled by cube_positions
	GLuint cubeScaleBuffer; //To be filled by cube_scales

	//Heightmap quads
	GLfloat *hMap_Posiitons; //Array of 4D vectors - hmap and normal
	GLfloat *hMap_Normals; // One normal per qaud

	//HMap quads
	GLuint hMapZValuesBuffer;
	GLuint hMapNormBuffer;

	//Road Quads
	GLfloat *road_positions; //Array of 4D
	GLfloat *road_scales; //Array of floats - road length

	//Road Quads
	GLuint roadPosBuffer;
	GLuint roadScaleBuffer;

	GLuint cubeInstanceCamID;
	GLuint cubeInstanceCamRightID;
	GLuint cubeInstanceViewProjMatID;

	GLuint hMapInstanceCamUpID;
	GLuint hMapInstanceCamRightID;
	GLuint hMapInstanceViewProjMatID;
	GLuint hMapInstanceDimID;
	GLuint hMapInstanceGridDimID;

	GLuint roadsInstanceCamUpID;
	GLuint roadsInstanceCamRightID;
	GLuint roadsInstanceViewProjMatID;
	GLuint roadsCamPosID;

	GLuint quadTexCamUpID;
	GLuint quadTexCamRightID;
	GLuint quadTexViewProjMatID;
	GLuint quadTexQuadColourID;

	GLuint cubeLightVecID;
	GLuint roadLightVecID;
	GLuint heightLightVecID;

	// Textures
	GLuint buildingTextureID;
	GLuint skyboxTextID;
	GLuint waterTexID;
	GLuint landTexID;
	GLuint roadTexID;

	// ---------------Camera Variables ---------------------
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

	QVector3D lightPos;

	//Mouse Controls
	QPointF prev;
	QPointF dif;

	// ---------------------------- Input Variables -------------------------

	//Keyboard controls
	static const int moveLeftKey;
	static const int moveRightKey;
	static const int moveForwardKey;
	static const int moveBackKey;
	static const int moveUpKey;
	static const int moveDownKey;
	static const int speedUpKey;
	static const int speedDownKey;
	static const int renderViewUpKey;
	static const int renderViewDownKey;
	static const int renderNumUpKey;
	static const int renderNumDownKey;
	bool isLeftHeld, isRightHeld, isBackHeld, isForwardHeld;

	static const float defaultMoveSpeed, fastMoveSpeed, slowMoveSpeed;
	static const float rotSpeed;

	std::string readFile(const char* fname);
	std::string readShader(const char *fname);

	QImage loadTexture(QString fname);
	void bindTexture(QImage &tex, GLuint texID);

};

#endif // CITYVIEW3D_H
