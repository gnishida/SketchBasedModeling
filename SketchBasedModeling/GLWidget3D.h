#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderManager.h"
#include <QPen>
#include <QGLWidget>
#include <QtGui>
#include "Camera.h"

class Stroke {
public:
	std::vector<glm::vec2> points;

public:
	Stroke() {}
	Stroke(const glm::vec2& point) { points.push_back(point); }
};

class GLWidget3D : public QGLWidget {
	Q_OBJECT

public:
	static enum { MODE_SKETCH = 0, MODE_3DVIEW };

public:
	Camera camera;
	glm::vec3 light_dir;
	glm::mat4 light_mvpMatrix;
	RenderManager renderManager;

	bool ctrlPressed;
	QPoint lastPoint;
	std::vector<Stroke> strokes;
	Stroke* currentStroke;

public:
	GLWidget3D(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void drawScene(int drawMode);
	void drawLineTo(const QPoint &endPoint);
	void resizeSketch(int width, int height);
	void compute3dCoordinates(Stroke* stroke);
	glm::vec3 unproject(const glm::vec2& point);

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void initializeGL();
	void paintEvent(QPaintEvent *e);
};

#endif