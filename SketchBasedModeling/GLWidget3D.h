#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "RenderManager.h"
#include <QPen>
#include <QGLWidget>
#include <QtGui>
#include "Camera.h"
#include "PointCloud.h"

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
	PointList points;
	std::vector<std::pair<int, int> > edges;

public:
	GLWidget3D(QWidget *parent = 0);

	void keyPressEvent(QKeyEvent* e);
	void keyReleaseEvent(QKeyEvent* e);
	void drawScene(int drawMode);
	void drawLineTo(const QPoint &endPoint);
	void resizeSketch(int width, int height);
	void compute3dCoordinates(Stroke* stroke);
	glm::vec3 unprojectByPlane(const glm::vec2& point, const glm::vec3& face_point, const glm::vec3& face_normal);
	glm::vec3 unprojectByLine(const glm::vec2& point, const glm::vec3& reference_point, const glm::vec3& vec);
	glm::vec2 normalizeScreenCoordinates(const glm::vec2& point);
	glm::vec3 viewVector(const glm::vec2& point, const glm::mat4& mvMatrix, float focalLength, float aspect);
	bool isStraightLine(Stroke* stroke);

protected:
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
	void initializeGL();
	void paintEvent(QPaintEvent *e);
};

#endif