#include "cityview3d.h"

const int CityView3D::moveLeftKey = Qt::Key::Key_A;
const int CityView3D::moveRightKey = Qt::Key::Key_D;
const int CityView3D::moveForwardKey = Qt::Key::Key_W;
const int CityView3D::moveBackKey = Qt::Key::Key_S;
const int CityView3D::moveUpKey = Qt::Key::Key_Q;
const int CityView3D::moveDownKey = Qt::Key::Key_E;
const int CityView3D::speedUpKey = Qt::Key::Key_Shift;
const int CityView3D::speedDownKey = Qt::Key::Key_Control;
const int CityView3D::renderNumUpKey = Qt::Key::Key_1;
const int CityView3D::renderNumDownKey = Qt::Key::Key_2;
const int CityView3D::renderViewUpKey = Qt::Key::Key_3;
const int CityView3D::renderViewDownKey = Qt::Key::Key_4;

const float CityView3D::defaultMoveSpeed = 0.1f;
const float CityView3D::slowMoveSpeed = defaultMoveSpeed * 4.0f;
const float CityView3D::fastMoveSpeed = defaultMoveSpeed * 20.0f;
const float CityView3D::rotSpeed = 0.2f;

static const GLfloat quadVertexBufferData[] = {
	0.5f,  0.0f, 0.5f,
	-0.5f,  0.0f, 0.5f,
	-0.5f, 0.0f, -0.5f,
	0.5f, 0.0f, -0.5f,
};

static const GLfloat cubeVertexBufferData[] = {
	0.5, 1, 0.5,	-0.5, 1, 0.5,	-0.5, 0, 0.5,	0.5, 0, 0.5,        // v0-v1-v2-v3
	0.5, 1, 0.5,	0.5, 0, 0.5,	0.5, 0, -0.5,	0.5, 1, -0.5,        // v0-v3-v4-v5
	0.5, 1, 0.5,	0.5, 1, -0.5,	-0.5, 1, -0.5,	-0.5, 1, 0.5,        // v0-v5-v6-v1
	-0.5, 1, 0.5,	-0.5, 1, -0.5,	-0.5, 0, -0.5,		-0.5, 0, 0.5,    // v1-v6-v7-v2
	-0.5, 0, -0.5,	0.5, 0, -0.5,	0.5, 0, 0.5,	-0.5, 0, 0.5,    // v7-v4-v3-v2
	0.5, 0, -0.5,	-0.5, 0, -0.5,	-0.5, 1, -0.5,	0.5, 1, -0.5};   // v4-v7-v6-v5

/*static const GLfloat cubeNormalsData[] = {
	0.0f,0.0f,1.0f,  0.0f,0.0f,1.0f,   0.0f,0.0f,1.0f,
	0.0f,0.0f,1.0f,	 0.0f,0.0f,-1.0f,  0.0f,0.0f,-1.0f,
	0.0f,0.0f,-1.0f, 0.0f,0.0f,-1.0f, -1.0f,0.0f,0.0f,
	-1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f, -1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,	 0.0f,-1.0f,0.0f,  0.0f,-1.0f,0.0f,
	0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f,  0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f,
	1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,   1.0f,0.0f,0.0f,
	1.0f,0.0f,0.0f,	 0.0f,-1.0f,0.0f,  0.0f,-1.0f,0.0f,
	0.0f,-1.0f,0.0f, 0.0f,-1.0f,0.0f,  0.0f,1.0f,0.0f,
	0.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,   0.0f,1.0f,0.0f
};*/

//static const GLfloat cubeVertexBufferData[] = {
//	-1.0f,0.0f,-1.0f, // triangle 1 : begin
//	-1.0f,0.0f, 1.0f,
//	-1.0f, 1.0f, 1.0f, // triangle 1 : end
//	1.0f, 1.0f,-1.0f, // triangle 2 : begin
//	-1.0f,0.0f,-1.0f,
//	-1.0f, 1.0f,-1.0f, // triangle 2 : end
//	1.0f,0.0f, 1.0f, //3
//	-1.0f,0.0f,-1.0f,
//	1.0f,0.0f,-1.0f, // 3
//	1.0f, 1.0f,-1.0f, //4
//	1.0f,0.0f,-1.0f,
//	-1.0f,0.0f,-1.0f, //4 
//	-1.0f,0.0f,-1.0f, //5
//	-1.0f, 1.0f, 1.0f,
//	-1.0f, 1.0f,-1.0f, //5
//	1.0f,0.0f, 1.0f, //6 
//	-1.0f,0.0f, 1.0f,
//	-1.0f,0.0f,-1.0f, //6
//	-1.0f, 1.0f, 1.0f, //7
//	-1.0f,0.0f, 1.0f,
//	1.0f,0.0f, 1.0f, //7
//	1.0f, 1.0f, 1.0f, //8
//	1.0f,0.0f,-1.0f,
//	1.0f, 1.0f,-1.0f, //8
//	1.0f,0.0f,-1.0f, //9
//	1.0f, 1.0f, 1.0f,
//	1.0f,0.0f, 1.0f, //9
//	1.0f, 1.0f, 1.0f, //10
//	1.0f, 1.0f,-1.0f, 
//	-1.0f, 1.0f,-1.0f, //10 
//	1.0f, 1.0f, 1.0f,  //11
//	-1.0f, 1.0f,-1.0f,
//	-1.0f, 1.0f, 1.0f, //11
//	1.0f, 1.0f, 1.0f, //12
//	-1.0f, 1.0f, 1.0f,
//	1.0f,0.0f, 1.0f //12
//};

static const GLfloat cubeNormalsData[] =
{
	0, 0, 1,	 0, 0, 1,	0, 0, 1,	0, 0, 1,				// v0-v1-v2-v3 
	1, 0, 0,	1, 0, 0,	1, 0, 0,	1, 0, 0,				// v0-v3-v4-v5
	0, 1, 0,	0, 1, 0,	0, 1, 0,	0, 1, 0,				// v0-v5-v6-v1
	-1, 0, 0,	-1, 0, 0,	-1, 0, 0,	-1, 0, 0,				// v1-v6-v7-v2
	0, -1, 0,	0, -1, 0,	0, -1, 0,	0, -1, 0,				// v7-v4-v3-v2
	0, 0, -1,	0, 0, -1,	0, 0, -1,	0, 0, -1			// v4-v7-v6-v5
};

static const char *roadVertexShader =
"#version 330\n"
"#define M_PI 3.1415926535897932384626433832795\n"
"layout(location = 0) in vec3 verts;\n"
"layout(location = 1) in vec4 pos;\n"  //x pos, y pos, z pos start, z pos end
"layout(location = 2) in vec3 scale;\n" //x scale, y scale, rot
"flat out vec4 normalOut;\n"
"flat out vec4 color;\n"
"uniform vec3 lightVec;\n"
"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 CameraUp_worldspace;\n"
"uniform vec3 camPos;\n"
"uniform mat4 VP;\n"
"void main() {\n"
"   vec3 tmp = pos.xyz / 2048.0f;\n"
"	float theta = scale.z - M_PI / 2.0f;"
"	vec2 scaled = verts.xz * scale.xy;\n"
"	vec2 rotated = vec2(scaled.x * cos(theta) + scaled.y * sin(theta), scaled.y * cos(theta) - scaled.x * sin(theta));\n"
"	vec2 wsp2d = pos.xy + rotated;\n"
"	vec3 wsp = vec3(wsp2d.x, 0.2f, wsp2d.y);\n"
"	if (camPos.y > 50.0) wsp.y += camPos.y * 0.04;"
"	wsp.y = verts.x < 0.0 ? pos.z : pos. w;\n"
"	gl_Position = VP * vec4(wsp, 1.0f);\n"
"	vec3 lightVector = normalize(lightVec - wsp.xyz);\n"
"	float diffuse = 0.5f;\n"
"   normalOut = vec4(1.0, 0.0, 0.0, 1.0);\n"
"	color = vec4(0.3, 0.3, 0.3, 1.0);"
"}\n";

static const char *hMapVertexShaderSource =
"#version 330\n"
"layout(location = 0) in vec3 verts;\n"
"layout(location = 1) in vec4 pos;\n"  //z value of this vertex
"layout(location = 2) in vec3 norm;\n" //x scale, y scale, rot
"flat out vec4 normalOut;\n"
"flat out vec4 color;\n"
"uniform vec3 lightVec;\n"
"uniform int gridSize;\n"
"uniform float quadScale;\n"
"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 CameraUp_worldspace;\n"
"uniform mat4 VP;\n"
"void main() {\n"
"	vec2 translated = verts.xz + vec2(0.5, 0.5);\n" //Warp to origin, forms a quad of unit length
"	translated *= quadScale;\n"
// x = / gridDimension, y = % gridDimension
"	vec2 wsp2d = translated;\n"
"	wsp2d.x += quadScale * (gl_InstanceID / gridSize);\n"
"	wsp2d.y += quadScale * mod(gridSize - gl_InstanceID, gridSize);\n"
"	vec3 wsp = vec3(wsp2d.x, - 0.1f, wsp2d.y);\n"
"	if (verts.x == -0.5f && verts.z == -0.5f) wsp.y = pos.z;" //bl
"	else if (verts.x == 0.5f && verts.z == -0.5f) wsp.y = pos.w;" //br
"	else if (verts.x == -0.5f && verts.z == 0.5f) wsp.y = pos.x;" //tl
"	else if (verts.x == 0.5f && verts.z == 0.5f) wsp.y = pos.y;" //tr
"	gl_Position = VP * vec4(wsp, 1.0f);\n"
"	vec3 lightVector = normalize(lightVec - wsp.xyz);\n"
"	float diffuse = min(0.8, max(dot(norm, lightVector), 0.2));\n"
"   normalOut = vec4(norm, 1.0);\n"
"	color = vec4(0.4f, 1.0f, 0.4f, 1.0f);"
"}\n";

static const char *vertexShaderSource =
"#version 330\n"
"#define M_PI 3.1415926535897932384626433832795\n"
"layout(location = 0) in vec3 verts;\n"
"layout(location = 1) in vec3 norm;\n"
"layout(location = 2) in vec4 pos;\n"
"layout(location = 3) in vec3 scale;\n"
"out vec4 normalOut;\n"
"out vec4 color;\n"
"uniform vec3 lightVec;\n"
"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 Camera_worldspace;\n"
"uniform mat4 VP;\n"
"void main() {\n"
"   vec3 scaled = verts * scale;\n"
"	float theta = (pos.w + M_PI / 2.0f);\n"
"	vec3 rotatedVerts = vec3(scaled.x * cos(theta) + scaled.z * sin(theta), scaled.y, scaled.z * cos(theta) - scaled.x * sin(theta)); \n"
"	vec3 rotatedNorm = vec3(norm.x * cos(theta) + norm.z * sin(theta), norm.y, norm.z * cos(theta) - norm.x * sin(theta)); \n"
"	vec3 wsp = pos.xyz + rotatedVerts;\n"
"	gl_Position = VP * vec4(wsp, 1.0f);\n"
"	vec3 lightVector = normalize(vec3(0.577f, 0.577f, 0.577f) - wsp.xyz);\n"
"	vec3 lookVec = normalize(Camera_worldspace - wsp.xyz);\n"
"	float diffuse = max(0.0f, abs(dot(norm, lightVector)));\n"
"   normalOut = vec4(diffuse, diffuse, diffuse, 1.0);\n"
"	color = vec4(0.4, 0.4, 0.4, 1.0);"
"}\n";

static const char *texturedQuadVertShader =
"#version 330\n"
"layout(location = 0) in vec3 verts;\n"
"layout(location = 1) in vec3 norm;\n"
"layout(location = 2) in vec4 pos;\n"
"layout(location = 3) in vec3 scale;\n"
"out vec4 normalOut;\n"
"out vec4 color;\n"
"uniform vec3 lightVec;\n"
"uniform vec3 CameraRight_worldspace;\n"
"uniform vec3 CameraUp_worldspace;\n"
"uniform vec3 quadColour;\n"
"uniform mat4 VP;\n"
"void main() {\n"
"	vec3 wsp = pos.xyz + (verts * scale);\n"
"	gl_Position = VP * vec4(wsp, 1.0f);\n"
"	vec3 lightVector = normalize(lightVec - wsp.xyz);\n"
"	float diffuse = max(dot(norm, lightVector), 0.2);\n"
"   normalOut = vec4(diffuse, diffuse, diffuse, 1.0);\n"
"	color = vec4(0.5, 0.5, 0.8, 0.7);"
"}\n";

static const char *fragmentShaderSource =
"flat in lowp vec4 normalOut;\n"
"flat in lowp vec4 color;\n"
"void main() {\n"
"   gl_FragColor = vec4(0.1, 0.1, 0.1, 1.0) + abs(dot(normalOut, vec4(1))) * color;\n"
"}\n";

static const char *fragmentShaderSourceHMap =
"flat in lowp vec4 normalOut;\n"
"flat in lowp vec4 color;\n"
"void main() {\n"
"   gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0) + abs(dot(normalOut, vec4(0, 1, 0, 1.0))) * color * 0.2;\n"
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
	setRoadData(5.0f, 7.0f);
	numCubeRenderMax = 1000000;
	numRoadPieceRenderMax = 100000;
	numTerrainQuadRenderMax = 512 * 512;
}


CityView3D::~CityView3D() {
	delete m_device;
	
	if (initialised) {
		delete cube_scales;
		delete cube_positions;

		delete road_positions;
		delete road_scales;

		delete hMap_Normals;
		delete hMap_Posiitons;

		delete m_InstancedBuildingProg;
		delete m_hMapRenderProg;
		delete m_roadRenderProg;
		delete m_TexturedQuadProg;
	}
}

void CityView3D::initialize() {

	//Set instanced rendering program
	m_InstancedBuildingProg = new QOpenGLShaderProgram(this);

	m_InstancedBuildingProg->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_InstancedBuildingProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_InstancedBuildingProg->link();

	cubeInstanceCamID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "Camera_worldspace");
	cubeInstanceCamRightID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "CameraRight_worldspace");
	cubeInstanceViewProjMatID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "VP");
	cubeLightVecID = funcs->glGetUniformLocation(m_InstancedBuildingProg->programId(), "lightVec");

	//Set hMap render program
	m_hMapRenderProg = new QOpenGLShaderProgram(this);

	m_hMapRenderProg->addShaderFromSourceCode(QOpenGLShader::Vertex, hMapVertexShaderSource);
	m_hMapRenderProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceHMap);
	m_hMapRenderProg->link();

	hMapInstanceCamUpID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "CameraUp_worldspace");
	hMapInstanceCamRightID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "CameraRight_worldspace");
	hMapInstanceViewProjMatID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "VP");
	hMapInstanceDimID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "quadScale");
	hMapInstanceGridDimID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "gridSize");
	heightLightVecID = funcs->glGetUniformLocation(m_hMapRenderProg->programId(), "lightVec");

	//Set road render program
	m_roadRenderProg = new QOpenGLShaderProgram(this);

	m_roadRenderProg->addShaderFromSourceCode(QOpenGLShader::Vertex, roadVertexShader);
	m_roadRenderProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_roadRenderProg->link();

	roadsInstanceCamUpID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "CameraUp_worldspace");
	roadsInstanceCamRightID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "CameraRight_worldspace");
	roadsInstanceViewProjMatID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "VP");
	roadLightVecID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "lightVec");
	roadsCamPosID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "camPos");

	//Set tex quad prog
	m_TexturedQuadProg = new QOpenGLShaderProgram(this);

	m_TexturedQuadProg->addShaderFromSourceCode(QOpenGLShader::Vertex, texturedQuadVertShader);
	m_TexturedQuadProg->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_TexturedQuadProg->link();

	quadTexCamUpID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "CameraUp_worldspace");
	quadTexCamRightID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "CameraRight_worldspace");
	quadTexViewProjMatID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "VP");
	quadTexQuadColourID = funcs->glGetUniformLocation(m_roadRenderProg->programId(), "quadColor");

	//Create data arrays
	cube_positions = new GLfloat[4 * numCubeRenderMax];
	cube_scales = new GLfloat[3 * numCubeRenderMax];

	road_positions = new GLfloat[4 * numRoadPieceRenderMax];
	road_scales = new GLfloat[3 * numRoadPieceRenderMax];

	hMap_Normals = new GLfloat[numTerrainQuadRenderMax * 3];
	hMap_Posiitons = new GLfloat[numTerrainQuadRenderMax * 4];

	//Initialise data buffers, still need to be set however.

	// CUBE 

	//Cube vertices
	glGenBuffers(1, &cubeVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexBufferData), cubeVertexBufferData, GL_STREAM_DRAW);

	//Cube normals
	glGenBuffers(1, &cubeNormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexBufferData), cubeVertexBufferData, GL_STREAM_DRAW);

	//Cube positions
	glGenBuffers(1, &cubePosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//Cube scales
	glGenBuffers(1, &cubeScaleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	// HMAP

	//Quad vertices
	glGenBuffers(1, &quadVertBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertexBufferData), quadVertexBufferData, GL_STREAM_DRAW);

	//HMap positions
	glGenBuffers(1, &hMapZValuesBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hMapZValuesBuffer);
	glBufferData(GL_ARRAY_BUFFER, numTerrainQuadRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//HMap normals
	glGenBuffers(1, &hMapNormBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, hMapNormBuffer);
	glBufferData(GL_ARRAY_BUFFER, numTerrainQuadRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//ROADS

	//Road positions
	glGenBuffers(1, &roadPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, roadPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, numRoadPieceRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	//Road scales
	glGenBuffers(1, &roadScaleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, roadScaleBuffer);
	glBufferData(GL_ARRAY_BUFFER, numRoadPieceRenderMax * 3 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

	initialised = true;

	updateRoadInstances();
	updateBuildingInstances();
}

void CityView3D::renderTerrain() {
	QVector4D fstRow = viewMatrix.row(0);
	QVector4D sndRow = viewMatrix.row(1);

	// ------------ Begin Water Render ---------------

	////Sets as active program
	m_TexturedQuadProg->bind();
	m_TexturedQuadProg->setUniformValue(m_matrixUniform, viewMatrix);

	//glUniform3f(quad, lightPos.x(), lightPos.y(), lightPos.z());
	glUniform4f(quadTexQuadColourID, 0.5f, 0.5f, 0.5f, 0.5f);
	glUniform3f(quadTexCamRightID, fstRow.x(), fstRow.y(), fstRow.z());
	glUniform3f(quadTexCamUpID, sndRow.x(), sndRow.y(), sndRow.z());
	glUniformMatrix4fv(quadTexViewProjMatID, 1, GL_FALSE, VP.constData());

	////Test city plan
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

	glVertexAttribPointer(m_posAttr, sizeof(quadVertexBufferData), GL_FLOAT, GL_FALSE, 0, quadVertexBufferData);
	glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glDrawArrays(GL_QUADS, 0, 4);
	
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);

	m_TexturedQuadProg->release();

	// Begin HMap render

	if (!hMapReady)
		updateHMapInstances();

	if (!hMapReady) return;

	if (hMapNeedsUpdate) {

		//Update Pos array
		glBindBuffer(GL_ARRAY_BUFFER, hMapZValuesBuffer);
		glBufferData(GL_ARRAY_BUFFER, numTerrainQuadRenderMax * 4 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numTerrainQuadRenderMax * sizeof(GLfloat) * 4, hMap_Posiitons);

		//Update scale array
		glBindBuffer(GL_ARRAY_BUFFER, hMapNormBuffer);
		glBufferData(GL_ARRAY_BUFFER, numTerrainQuadRenderMax * 3 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numTerrainQuadRenderMax * sizeof(GLfloat) * 3, hMap_Normals);

		hMapNeedsUpdate = false;
	}

	//Sets as active program
	m_hMapRenderProg->bind();

	glUniform1f(hMapInstanceDimID, hMapDimension);
	glUniform1i(hMapInstanceGridDimID, hMapGridCount);
	glUniform3f(heightLightVecID, lightPos.x(), lightPos.y(), lightPos.z());
	glUniform3f(hMapInstanceCamRightID, fstRow.x(), fstRow.y(), fstRow.z());
	glUniform3f(hMapInstanceCamUpID, sndRow.x(), sndRow.y(), sndRow.z());
	glUniformMatrix4fv(hMapInstanceViewProjMatID, 1, GL_FALSE, VP.constData());

	//Bind instance variables

	//Vertex attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertBuffer);
	glVertexAttribPointer(
		0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Z value
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, hMapZValuesBuffer);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Quad normal
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, hMapNormBuffer);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	funcs->glVertexAttribDivisor(0, 0); //Reuse the same vertices
	funcs->glVertexAttribDivisor(1, 1); //Z value updated per instance
	funcs->glVertexAttribDivisor(2, 1); //Normal updated per instance

	//Draw instance
	funcs->glDrawArraysInstanced(GL_QUADS, 0, 4, numTerrainQuadRenderMax);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_hMapRenderProg->release();

}

void CityView3D::renderBuildings() {

	//move to a separate update function?

	if (needUpdateBuildings) updateBuildingInstances();

	if (buildingsNeedUpdateVertexData) {

		//Update Pos array
		glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
		glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numCubeRender * sizeof(GLfloat) * 4, cube_positions);

		//Update scale array
		glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
		glBufferData(GL_ARRAY_BUFFER, numCubeRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numCubeRender * sizeof(GLfloat) * 3, cube_scales);
		buildingsNeedUpdateVertexData = false;
	}
	//Sets as active program
	m_InstancedBuildingProg->bind();


	QVector4D fstRow = viewMatrix.row(0);
	QVector4D sndRow = viewMatrix.row(1);

	glUniform3f(cubeLightVecID, lightPos.x(), lightPos.y(), lightPos.z());
	glUniform3f(cubeInstanceCamRightID, fstRow.x(), fstRow.y(), fstRow.z());
	glUniform3f(cubeInstanceCamID, position.x(), position.y(), position.z());
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

	//Normal attribute
	glEnableVertexAttribArray(3);
	glBindBuffer(GL_ARRAY_BUFFER, cubeNormBuffer);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_TRUE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Position attribute
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cubePosBuffer);
	glVertexAttribPointer(
		2,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Scale attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, cubeScaleBuffer);
	glVertexAttribPointer(
		3,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	funcs->glVertexAttribDivisor(0, 0); //Reuse the same vertices
	funcs->glVertexAttribDivisor(1, 0);
	funcs->glVertexAttribDivisor(2, 1);
	funcs->glVertexAttribDivisor(3, 1);

	//Draw instanced
	funcs->glDrawArraysInstanced(GL_QUADS, 0, 12 * 3, numCubeRender);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);

	m_InstancedBuildingProg->release();

}

void CityView3D::renderRoads() {

	if (needUpdateRoads) updateRoadInstances();

	if (roadsNeedUpdateVertexData) {

		//Update Pos array
		glBindBuffer(GL_ARRAY_BUFFER, roadPosBuffer);
		glBufferData(GL_ARRAY_BUFFER, numRoadPieceRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numRoadPieceRender * sizeof(GLfloat) * 4, road_positions);

		//Update scale array
		glBindBuffer(GL_ARRAY_BUFFER, roadScaleBuffer);
		glBufferData(GL_ARRAY_BUFFER, numRoadPieceRenderMax * 4 * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, numRoadPieceRender * sizeof(GLfloat) * 3, road_scales);

		roadsNeedUpdateVertexData = false;
	}

	//Sets as active program
	m_roadRenderProg->bind();

	QVector4D fstRow = viewMatrix.row(0);
	QVector4D sndRow = viewMatrix.row(1);

	glUniform3f(roadsCamPosID, position.x(), position.y(), position.z());
	glUniform3f(roadLightVecID, lightPos.x(), lightPos.y(), lightPos.z());
	glUniform3f(roadsInstanceCamRightID, fstRow.x(), fstRow.y(), fstRow.z());
	glUniform3f(roadsInstanceCamUpID, sndRow.x(), sndRow.y(), sndRow.z());
	glUniformMatrix4fv(roadsInstanceViewProjMatID, 1, GL_FALSE, VP.constData());

	//Bind instance variables

	//Vertex attribute
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, quadVertBuffer);
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
	glBindBuffer(GL_ARRAY_BUFFER, roadPosBuffer);
	glVertexAttribPointer(
		1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
		4,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
		);

	//Scale attribute
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, roadScaleBuffer);
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

	//Draw instance
	funcs->glDrawArraysInstanced(GL_QUADS, 0, 4, numRoadPieceRender);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	m_roadRenderProg->release();
}

void CityView3D::updateCamera() {

	//Angle in degrees
	projection.perspective(45.0f, 4.0f / 3.0f, 1.0f, 1024.0f);
	float fact = 3.14159 / 180.0f;

	dir = QVector3D(
		- cosf(rotation.y()* fact) * sinf(rotation.x() * fact),
		sinf(rotation.y()* fact),
		cosf(rotation.y()* fact) * cosf(rotation.x()* fact)).normalized();

	right = QVector3D(	
		-sinf(rotation.x() * fact - 3.14f / 2.0f),
		0,
		cosf(rotation.x() * fact - 3.14f / 2.0f)
		).normalized();

	up = QVector3D(0, 1, 0);

	if (moveSpeed == slowMoveSpeed) {
		qDebug() << dir.x() << " " << dir.y() << " " << dir.z();
	}

	forward = position + dir;

	viewMatrix = QMatrix4x4();
	viewMatrix.perspective(60.0f, 16.0 / 9.0f, 0.1f, 200 + 2048.0f);
	viewMatrix.rotate(rotation.y(), 1, 0, 0);
	viewMatrix.rotate(rotation.x(), 0, 1, 0);
	viewMatrix.translate(-position);
	VP = viewMatrix;

	lightPos = QVector3D(0, 1000, 0);
}

void CityView3D::updateBuildingInstances() {
	//
	numCubeRender = 0;
	int numRegionRender = 0;

	BOOST_FOREACH(BuildingRegion &region, *city->getRegions()) {
		//Visibility test base on current camera bounds
		bool isRegionVisible = region.getPoints().front().getDistanceSq(position.toVector2D().toPoint()) < renderDistance * renderDistance;

		if (isRegionVisible) {
			BOOST_FOREACH(BuildingLot &lot, region.getLots()) {
				//Visibility test on each lot
				bool isLotVisible = true;


				if (isLotVisible) {
					//Now add this lot's geometry to the buffer
					Building *toRender = lot.getBuilding();
					numRegionRender++;

					//TODO :: Add LOD functionality here
					BOOST_FOREACH(Cube& cube, toRender->getCubes()) {
						int basePos = numCubeRender * 4;
						int basePosScale = numCubeRender * 3;

						cube_positions[basePos] = cube.getBase().x();		//X
						cube_positions[basePos + 1] = cube.getBase().z();	//Y
						cube_positions[basePos + 2] = cube.getBase().y();	//Z
						cube_positions[basePos + 3] = cube.getRot();	//Rot

						cube_scales[basePosScale] = cube.getScale().x();		//X
						cube_scales[basePosScale + 1] = cube.getScale().z();	//Y
						cube_scales[basePosScale + 2] = cube.getScale().y();	//Z

						numCubeRender++;
						if (numCubeRender >= numCubeRenderMax) return;
					}
				}
			}
		}
	}

	buildingsNeedUpdateVertexData = true;
}

void CityView3D::updateHMapInstances() {
	int dim = int(floor(sqrtf(numTerrainQuadRenderMax)));

	//Create dim quads in each direction, each world size / dim long
	float scale = 2048.0f / dim;
	hMapDimension = scale;
	hMapGridCount = dim;

	for (int xItr = 0; xItr < dim; xItr++) {
		for (int yItr = 0; yItr < dim; yItr++) {
			int thisX = scale * xItr;
			int thisY = scale * yItr;
			int nextX = thisX + scale;
			int nextY = thisY + scale;
			int basePos = (xItr * dim + yItr) * 4;

			float tl = hMap_Posiitons[basePos] = getHeightValue(getHeightSample(thisX, thisY)); //TL Quad Z
			float tr = hMap_Posiitons[basePos + 1] = getHeightValue(getHeightSample(nextX, thisY)); //TR Quad Z
			float bl = hMap_Posiitons[basePos + 2] = getHeightValue(getHeightSample(thisX, nextY)); //BL Quad Z
			float br = hMap_Posiitons[basePos + 3] = getHeightValue(getHeightSample(nextX, nextY)); //BR Quad Z

			if (nextY == 2048) {
				hMap_Posiitons[basePos + 2] = hMap_Posiitons[basePos + 3] = 0;
			}

			if (nextX == 2048) {
				hMap_Posiitons[basePos + 1] = hMap_Posiitons[basePos + 3] = 0;
			}

			//Calculate normal
			QVector3D top = QVector3D(scale, 0, tr - tl);
			QVector3D left = QVector3D(0, scale, bl - tl);
			QVector3D right = QVector3D(0, scale, br - tr);
			QVector3D bottom = QVector3D(scale, 0, br - bl);
			QVector3D toprev = QVector3D(scale, 0, -tr - tl);
			QVector3D leftrev = QVector3D(0, scale,- bl - tl);
			QVector3D rightrev = QVector3D(0, scale, -br - tr);
			QVector3D bottomrev = QVector3D(scale, 0, -br - bl);

			QVector3D normTL = QVector3D::crossProduct(top, left).normalized();
			QVector3D normTR = QVector3D::crossProduct(toprev, right).normalized();
			QVector3D normBL = QVector3D::crossProduct(toprev, right).normalized();
			QVector3D normBR = QVector3D::crossProduct(leftrev, bottomrev).normalized();

			QVector3D norm = normTL + normTR + normBL + normBR;
			norm /= 4.0f;
			norm.normalize();

			int base = (xItr * dim + yItr) * 3;
			hMap_Normals[base] = norm.x();  //Quad norm x
			hMap_Normals[base + 1] = norm.y(); //Quad norm y
			hMap_Normals[base + 2] = norm.z(); //Quad norm z
		}
	}

	hMapReady = true;

	hMapNeedsUpdate = true;
}

void CityView3D::updateRoadInstances() {
	numRoadPieceRender = 0;
	auto rItr = city->roads.begin();
	bool finished = false;

	Point pos2d = Point(position.x(), position.y());

	while (!finished) {
		//Create each road, split if necessary
		
		bool isRoadVisible = true;

		if (isRoadVisible) {
			int dist = std::min((*rItr)->getEnd()->getIntersectionPoint().getDistanceSq(pos2d),
				(*rItr)->getStart()->getIntersectionPoint().getDistanceSq(pos2d));
			int numSubdivide = 1;
			if (dist < 500 *500) numSubdivide = 2;
			if (dist < 100 * 100) numSubdivide = 5;

			RoadGeometry *road = (rItr->operator->());
			float splitLength = road->getLength() / numSubdivide;
			float length = road->getLength();

			float xCenter = road->getStart()->getIntersectionPoint().x();
			float yCenter = road->getStart()->getIntersectionPoint().y();
			float incrX = road->getNorm().dx() * splitLength;
			float incrY = road->getNorm().dy() * splitLength;

			xCenter += incrX / 2.0f; //To start at the middle of the road
			yCenter += incrY / 2.0f;

			//Adds one road piece
			for (int i = 0; i < numSubdivide; i++) {
				int basePosPos = numRoadPieceRender * 4;
				int basePosScale = numRoadPieceRender * 3;

				road_scales[basePosScale] = splitLength; // X scale
				road_scales[basePosScale + 1] = road->getRoadType() == roadType::MAINROAD ? mainroadWidth : streetWidth; // Y scale = road width
				road_scales[basePosScale + 2] = road->getAngleToEnd(); // Rot

				road_positions[basePosPos] = xCenter; // X Pos (center)
				road_positions[basePosPos + 1] = yCenter; // Y Pos (center)

				float sampleX = xCenter - incrX / 2.0f;
				float sampleY = yCenter - incrY / 2.0f;

				assert(sampleX <= 2048);
				assert(sampleX >= 0);
				assert(sampleY <= 2048);
				assert(sampleY >= 0);
				//Idk why these y values are flipped 
				road_positions[basePosPos + 2] = getHeightValue(getHeightSample(sampleX, 2048 - sampleY)); // Z Pos begin
				sampleX += incrX;
				sampleY += incrY;
				road_positions[basePosPos + 3] = getHeightValue(getHeightSample(sampleX, 2048 - sampleY)); // Z Pos end

				numRoadPieceRender++;

				//Increment startPos
				xCenter += incrX;
				yCenter += incrY;
			}

		}

		if (numRoadPieceRender > numRoadPieceRenderMax) finished = true;

		rItr++;
		if (rItr == city->roads.end()) finished = true;
	}

	roadsNeedUpdateVertexData = true;
}

void CityView3D::render() {
	if (!m_device)
		m_device = new QOpenGLPaintDevice();

	m_device->setSize(size());

	QPainter painter(m_device);
	render(&painter);
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);

	updateCamera();

	//SkyBox

	//Set hMap program, render hMap
	renderTerrain();

	glClear(GL_DEPTH_BUFFER_BIT); // As terrain under all over items to be rendered!

	//Set building program, render buildings
	glEnable(GL_CULL_FACE);
	renderBuildings();


	//Set road program, render roads
	glDisable(GL_CULL_FACE);
	renderRoads();

	++m_frame;
}

float CityView3D::getHeightValue(float heightFactor) {
	return minHeight + (1 - heightFactor) * heightScale;
}

float CityView3D::getHeightSample(int x, int y) {
	QRgb pix = heightMap.pixel(x, y);
	return (pix % 256) / 256.0f;
}

void CityView3D::render(QPainter *painter) {

	std::string lol = std::string("Pos: ") + std::to_string(position.x()) + " " + std::to_string(position.y()) + " " + std::to_string(position.z());

	painter->drawText(QPoint(10, 10),
		QString(lol.c_str()));
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
	dif = event->localPos() - prev;
	prev = event->localPos();

	QPoint glob = mapToGlobal(QPoint(width() / 2, height() / 2));
	QCursor::setPos(glob);
	prev = QPoint(width() / 2, height() / 2);

	rotation.setX(rotation.x() + dif.x() * rotSpeed);
	rotation.setY(rotation.y() + dif.y() * rotSpeed);

	event->accept();
}

void CityView3D::keyPressEvent(QKeyEvent * event) {
	bool accepted = true;
	switch (event->key()) {
	case moveLeftKey :
		position -= right * moveSpeed;
		return;
	case moveRightKey : 
		position += right * moveSpeed;
		return;
	case moveBackKey:
		position += dir * moveSpeed;
		return;
	case moveForwardKey:
		position -= dir * moveSpeed;
		return;
	case moveUpKey:
		position += up * moveSpeed;
		return;
	case moveDownKey:
		position -= up * moveSpeed;
		return;
	case speedDownKey:
		moveSpeed = slowMoveSpeed;
		return;
	case speedUpKey:
		moveSpeed = fastMoveSpeed;
		return;
	case renderViewUpKey:
		renderDistance += 5;
		return;
	case renderViewDownKey:
		renderDistance -= 5;
		renderDistance = std::max(renderDistance, 100);
	case renderNumUpKey:
		renderDistance += 5;
		return;
	case renderNumDownKey:
		renderDistance -= 5;
		renderDistance = std::max(renderDistance, 100);
		return;
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
		auto format = requestedFormat();
		format.setDepthBufferSize(24);
		format.setSamples(4);
		m_context->setFormat(format);
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

void CityView3D::setData(City * city, QImage * height, QImage * geog, float minHeight, float heightScale) {
	this->city = city;
	this->heightMap = QGLWidget::convertToGLFormat(*height);
	this->geogMap = QGLWidget::convertToGLFormat(*geog);
	this->minHeight = minHeight;
	this->heightScale = heightScale;
	hasCity = true;

	hMapReady = false;
}

void CityView3D::setRoadData(float streetWidth, float mainroadWidth) {
	this->streetWidth = streetWidth;
	this->mainroadWidth = mainroadWidth;

	needUpdateRoads = true;
}

void CityView3D::setAnimating(bool animating) {
	m_animating = animating;

	if (animating)
		renderLater();
}

void CityView3D::initaliseCamera() {
	position = QVector3D(1024, 0, 1024);
	rotation = QVector3D(0, 0, 0);

	moveSpeed = defaultMoveSpeed;
	renderDistance = 2048.0f;
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

QImage CityView3D::loadTexture(QString fname) {
	QImage img = QImage(fname);
	return QGLWidget::convertToGLFormat(img);
}

void CityView3D::bindTexture(QImage & tex, GLuint texID) {
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, texID);
}

