#include "cityview3d.h"

const int CityView3D::moveLeftKey = Qt::Key::Key_A;
const int CityView3D::moveRightKey = Qt::Key::Key_D;
const int CityView3D::moveForwardKey = Qt::Key::Key_W;
const int CityView3D::moveBackKey = Qt::Key::Key_S;
const int CityView3D::moveUpKey = Qt::Key::Key_Q;
const int CityView3D::moveDownKey = Qt::Key::Key_E;
const int CityView3D::speedUpKey = Qt::Key::Key_Shift;
const int CityView3D::speedDownKey = Qt::Key::Key_Control;

const float CityView3D::defaultMoveSpeed = 0.1f;
const float CityView3D::slowMoveSpeed = defaultMoveSpeed * 0.1f;
const float CityView3D::fastMoveSpeed = defaultMoveSpeed * 4.0f;
const float CityView3D::rotSpeed = 0.2f;

static const GLfloat quadVertexBufferData[] = {
	-0.5f, -0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	0.5f,  0.5f, 0.0f,
};

static const GLfloat cubeVertexBufferData[] = {
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

//layout(location = 3) 
static const char *vertexShaderSource =
"in vec3 verts;\n"
"in vec3 pos;\n"
"in vec3 scale;\n"
"varying lowp vec4 col;\n"
"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 CameraUp_worldspace;\n"
"uniform mat4 VP;\n"
"void main() {\n"
"   vec3 tmp = pos.xyz / 2048.0f;\n"
"   col = vec4(pos.x, pos.y, pos.z, 1.0);\n"
"	vec3 wsp = pos.xyz + verts;\n"
"	gl_Position = VP * vec4(wsp, 1.0f);\n"
"}\n";

static const char *fragmentShaderSource =
"varying lowp vec4 col;\n"
"void main() {\n"
"   gl_FragColor = col;\n"
"}\n";

CityView3D::CityView3D(QWindow *parent) :
    QWindow(parent)
	, m_update_pending(false)
	, m_animating(false)
	, m_context(0)
	, m_device(0)
	, initialised(false)
	, hasCity(false)
{
	setSurfaceType(QWindow::OpenGLSurface);
	this->setCursor(Qt::BlankCursor);

	isLeftHeld = isRightHeld = isBackHeld = isForwardHeld = false;
	initaliseCamera();

	numCubeRenderMax = 1000;
}


CityView3D::~CityView3D() {
	delete m_device;
	
	if (initialised) {
		delete cube_scales;
		delete cube_positions;
	}
}

void CityView3D::render(QPainter *painter) {
	Q_UNUSED(painter);
}

void CityView3D::initialize() {

	//Set instanced rendering program
	m_InstancedBuildingProg = new QOpenGLShaderProgram(this);

	m_InstancedBuildingProg->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_InstancedBuildingProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_InstancedBuildingProg->link();

	cubeInstanceCamUpID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "CameraUp_worldspace");
	cubeInstanceCamRightID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "CameraRight_worldspace");
	cubeInstanceViewProjMatID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "VP");

	//Set hMap render program
	m_hMapRenderProg = new QOpenGLShaderProgram(this);

	m_hMapRenderProg->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_hMapRenderProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_hMapRenderProg->link();

	m_posAttr = m_hMapRenderProg->attributeLocation("posAttr");
	m_colAttr = m_hMapRenderProg->attributeLocation("colAttr");
	m_matrixUniform = m_hMapRenderProg->uniformLocation("matrix");

	//Create data arrays
	cube_positions = new GLfloat[3 * numCubeRenderMax];
	cube_scales = new GLfloat[3 * numCubeRenderMax];

	//Initialise data buffers, still need to be set however.

	//Cube vertices
	glGenBuffers(1, &cubeVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexBufferData), cubeVertexBufferData, GL_STREAM_DRAW);

	//Cube positions
	glGenBuffers(1, &cubePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//Cube scales
	glGenBuffers(1, &cubeScaleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	initialised = true;
}

void CityView3D::renderTerrain() {
	//Sets as active program
	m_InstancedBuildingProg->bind();
	m_InstancedBuildingProg->setUniformValue(m_matrixUniform, viewMatrix);

	//Test city plan
	GLfloat vertices[] = {
		0.0f, 0.707f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	GLfloat colors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f
	};

	glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_InstancedBuildingProg->release();

}

void CityView3D::renderBuildings() {

	//move to a separate update function?
	updateBuildingInstances();

	//Update Pos array
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numCubeRender * sizeof(GLfloat) * 3, cube_positions);

	//Update scale array
	glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, numCubeRender * sizeof(GLfloat) * 3, cube_scales);

	glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Sets as active program
	m_InstancedBuildingProg->bind();

	QVector4D fstRow = viewMatrix.row(0);
	QVector4D sndRow = viewMatrix.row(1);

	glUniform3f(cubeInstanceCamRightID, fstRow.x(), fstRow.y(), fstRow.z());
	glUniform3f(cubeInstanceCamUpID, sndRow.x(), sndRow.y(), sndRow.z());
	glUniformMatrix4fv(cubeInstanceViewProjMatID, 1, GL_FALSE, VP.constData());

	//Bind instance variables

	//Vertex attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Position attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Scale attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	funcs->glVertexAttribDivisor(0, 0); //Reuse the same vertices
	funcs->glVertexAttribDivisor(1, 1);
	funcs->glVertexAttribDivisor(2, 1);
	
	//Draw instanced
	//funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 12 * 3, numCubeRender);
	funcs->glDrawArraysInstanced(GL_TRIANGLES, 0, 12 * 3, numCubeRender);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_InstancedBuildingProg->release();

}

void CityView3D::renderRoads() {
}

void CityView3D::updateCamera() {

	//Angle in degrees
	projection.perspective(45.0f, 4.0f / 3.0f, 0.1f, 512.0f);
	float fact = 3.14159 / 180.0f;

	dir = QVector3D(
		cosf(rotation.y()* fact) * sinf(rotation.x() * fact),
		sinf(rotation.y()* fact),
		cosf(rotation.y()* fact) * cosf(rotation.x()* fact)).normalized();

	//dir = QVector3D(rotation.y(), rotation.z(), rotation.x()).normalized();

	right = QVector3D(	
		sinf(rotation.x() * fact - 3.14f / 2.0f),
		0,
		cosf(rotation.x() * fact - 3.14f / 2.0f)
		).normalized();

	up = QVector3D(0, 0, 1);
	right = -QVector3D::crossProduct(dir, up).normalized();

	if (moveSpeed == slowMoveSpeed) {
		qDebug() << dir.x() << " " << dir.y() << " " << dir.z();
	}

	forward = position + dir;
	eye = QVector3D(dir.x(), dir.y(), dir.z());
	//up = QVector3D(0, 0, 0);
	//forward = QVector3D(0, 1, 0);
	//eye = QVector3D(0, 0, 0);
	//viewMatrix.lookAt(eye, forward, up);

	////Update camera  (Old)
	viewMatrix = QMatrix4x4();
	viewMatrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 1024.0f);
	viewMatrix.rotate(rotation.y(), 1, 0, 0);
	viewMatrix.rotate(rotation.x(), 0, 0, 1);
	viewMatrix.translate(-position);
	// mouse up should be l / r
	// mouse l /r should be z

	//VP = viewMatrix * projection;
	
	//Old
	VP = viewMatrix;

}

void CityView3D::updateBuildingInstances() {
	//
	numCubeRender = 0;
	int numRegionRender = 0;

	cube_positions[0] = 1024.0f;	//X
	cube_positions[1] = 1024.0f;	//Y
	cube_positions[2] = 0.0f;	//Z

	cube_scales[0] = 1.0f;	//X
	cube_scales[1] = 1.0f;	//Y
	cube_scales[2] = 1.0f;	//Z

	numRegionRender++;

	BOOST_FOREACH(BuildingRegion &region, *city->getRegions()) {
		//Visibility test base on current camera bounds
		bool isRegionVisible = region.getPoints().front().getDistanceSq(position.toVector2D().toPoint()) < 1000 * 1000;

		if (isRegionVisible) {
			BOOST_FOREACH(BuildingLot &lot, region.getLots()) {
				//Visibility test on each lot
				bool isLotVisible = true;

				if (isLotVisible) {
					//Now add this lot's geometry to the buffer

					int basePos = numRegionRender * 3;
					cube_positions[basePos] = lot.getPoints().front().x();		//X
					cube_positions[basePos + 1] = lot.getPoints().front().y();	//Y
					cube_positions[basePos + 2] = 0.0f;							//Z

					cube_scales[basePos] = 4.0f;		//X
					cube_scales[basePos + 1] = 4.0f;	//Y
					cube_scales[basePos + 2] = 10.0f;	//Z

					numRegionRender++;
				}
			}
		}
	}

	numCubeRender = numRegionRender;
}

void CityView3D::render() {
	if (!m_device)
		m_device = new QOpenGLPaintDevice();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	m_device->setSize(size());

	QPainter painter(m_device);
	render(&painter);
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	funcs->glDepthFunc(GL_LESS);

	updateCamera();

	//SkyBox

	//Set hMap program, render hMap
	//renderTerrain();

	//Set road program, render roads
	//renderRoads();

	//Set building program, render buildings
	renderBuildings();

	++m_frame;
}

void CityView3D::renderLater() {
	if (!m_update_pending) {
		m_update_pending = true;
		QCoreApplication::postEvent(this, new QEvent(QEvent::UpdateRequest));
	}
}

bool CityView3D::event(QEvent *event) {
	switch (event->type()) {
	case QEvent::UpdateRequest:
		m_update_pending = false;
		renderNow();
		return true;
	default:
		return QWindow::event(event);
	}
}

void CityView3D::mouseMoveEvent(QMouseEvent * event) {
	dif = prev - event->localPos();
	dif.setX(dif.x() * -1);
	prev = event->localPos();

	QPoint glob = mapToGlobal(QPoint(width() / 2, height() / 2));
	QCursor::setPos(glob);
	prev = QPoint(width() / 2, height() / 2);

	rotation.setX(rotation.x() + dif.x() * rotSpeed);
	rotation.setY(rotation.y() + dif.y() * rotSpeed);

}

void CityView3D::keyPressEvent(QKeyEvent * event) {
	bool accepted = true;
	switch (event->key()) {
	case moveLeftKey :
		position -= right * moveSpeed;
	case moveRightKey : 
		position += right * moveSpeed;
	case moveBackKey:
		position -= dir * moveSpeed;
	case moveForwardKey:
		position += dir * moveSpeed;
	case moveUpKey:
		position += up * moveSpeed;
	case moveDownKey:
		position -= up * moveSpeed;
	case speedDownKey:
		moveSpeed = slowMoveSpeed;
	case speedUpKey:
		moveSpeed = fastMoveSpeed;
	default:
		accepted = false;
	}

	accepted ? event->accept() : event->ignore();
}

void CityView3D::keyReleaseEvent(QKeyEvent * event) {
	bool accepted = true;
	switch (event->key()) {
	case speedDownKey:
		moveSpeed = defaultMoveSpeed;
		break;
	case speedUpKey:
		moveSpeed = defaultMoveSpeed;
		break;
	default:
		accepted = false;
	}

	accepted ? event->accept() : event->ignore();
}

void CityView3D::exposeEvent(QExposeEvent *event) {
	Q_UNUSED(event);

	if (isExposed())
		renderNow();
}

void CityView3D::renderNow() {
	if (!isExposed())
		return;

	bool needsInitialize = false;

	if (!m_context) {
		m_context = new QOpenGLContext(this);
		m_context->setFormat(requestedFormat());
		m_context->create();
		m_context->makeCurrent(this);

		//Required to access new features
		funcs = m_context->versionFunctions<QOpenGLFunctions_4_3_Core>();
		funcs->initializeOpenGLFunctions();

		needsInitialize = true;
	}

	m_context->makeCurrent(this);

	if (needsInitialize) {
		initializeOpenGLFunctions();
		initialize();
	}

	render();

	m_context->swapBuffers(this);

	if (m_animating)
		renderLater();
}

void CityView3D::setData(City * city) {
	this->city = city;
	hasCity = true;
}

void CityView3D::setAnimating(bool animating) {
	m_animating = animating;

	if (animating)
		renderLater();
}

void CityView3D::initaliseCamera() {
	position = QVector3D(1024, 1024, 2);
	rotation = QVector3D(0, 0, 0);

	moveSpeed = defaultMoveSpeed;
}

GLuint CityView3D::loadShader(GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	return shader;
}

std::string CityView3D::readFile(char * fname) {
	QFile file(fname);
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		return NULL;
	}

	QByteArray total;
	QByteArray line;
	while (!file.atEnd()) {
		line = file.read(1024);
		total.append(line);
	}

	return QString(total).toStdString();
}

