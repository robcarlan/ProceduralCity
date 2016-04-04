#include "cityview3d.h"

static const char *vertexShaderSource =
"attribute highp vec4 posAttr;\n"
"attribute lowp vec4 colAttr;\n"
"varying lowp vec4 col;\n"
"uniform highp mat4 matrix;\n"
"void main() {\n"
"   col = colAttr;\n"
"   gl_Position = matrix * posAttr;\n"
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
{
	setSurfaceType(QWindow::OpenGLSurface);
}


CityView3D::~CityView3D() {
	delete m_device;
}

void CityView3D::render(QPainter *painter) {
	Q_UNUSED(painter);
}

void CityView3D::initialize() {
	//Triangle
	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
	m_program->link();
	m_posAttr = m_program->attributeLocation("posAttr");
	m_colAttr = m_program->attributeLocation("colAttr");
	m_matrixUniform = m_program->uniformLocation("matrix");
}

void CityView3D::render() {
	//if (!m_device)
	//	m_device = new QOpenGLPaintDevice();

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	//m_device->setSize(size());

	//QPainter painter(m_device);
	//render(&painter);
	const qreal retinaScale = devicePixelRatio();
	glViewport(0, 0, width() * retinaScale, height() * retinaScale);

	glClear(GL_COLOR_BUFFER_BIT);

	//m_program->bind();

	//QMatrix4x4 matrix;
	//matrix.perspective(60.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	//matrix.translate(0, 0, -2);

	//matrix.rotate(prev.x(), 0, 1, 0);
	//matrix.rotate(prev.y(), 1, 0, 0);

	//m_program->setUniformValue(m_matrixUniform, matrix);

	////Test city plan
	//GLfloat vertices[] = {
	//	0.0f, 0.707f,
	//	-0.5f, -0.5f,
	//	0.5f, -0.5f
	//};

	//GLfloat colors[] = {
	//	1.0f, 0.0f, 0.0f,
	//	0.0f, 1.0f, 0.0f,
	//	0.0f, 0.0f, 1.0f,
	//	0.0f, 1.0f, 0.0f
	//};

	//glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, vertices);
	//glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, colors);

	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//glDisableVertexAttribArray(1);
	//glDisableVertexAttribArray(0);
	glBegin(GL_QUADS); // Start drawing a quad primitive  

	glVertex3f(-1.0f, 1.0f, -1.0f); // The bottom left corner  
	glVertex3f(1.0f, 1.0f, -1.0f); // The top left corner  
	glVertex3f(1.0f, -1.0f, -1.0f); // The top right corner  
	glVertex3f(-.0f, -1.0f, -1.0f); // The bottom right corner  
	glEnd();

	//m_program->release();

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
	dif = event->screenPos() - prev;
	prev = event->screenPos();
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

void CityView3D::setAnimating(bool animating) {
	m_animating = animating;

	if (animating)
		renderLater();
}

GLuint CityView3D::loadShader(GLenum type, const char *source) {
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);
	return shader;
}

